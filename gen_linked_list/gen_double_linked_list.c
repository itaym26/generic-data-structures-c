#include <stdlib.h> 
#include <stdio.h>  
#include "list.h"   

/* Internal Node structure */
typedef struct Node Node;

struct Node {
    void* m_data;     /* Pointer to user data */
    Node* m_next;     /* Pointer to the next node */
    Node* m_prev;     /* Pointer to the previous node */
};

/* Internal List structure using Sentinels */
struct List {
    Node m_head;      /* Head Sentinel - doesn't hold data */
    Node m_tail;      /* Tail Sentinel - doesn't hold data */
};

List* ListCreate(void) {
    List* list = (List*)malloc(sizeof(List));
    if (NULL == list) {
        return NULL;
    }

    /* Initialize Sentinels and connect them */
    list->m_head.m_next = &list->m_tail;
    list->m_head.m_prev = NULL;
    list->m_head.m_data = NULL;

    list->m_tail.m_prev = &list->m_head;
    list->m_tail.m_next = NULL;
    list->m_tail.m_data = NULL;

    return list;
}

void ListDestroy(List** _pList, void (*_elementDestroy)(void* _item)) {
    if (NULL == _pList || NULL == *_pList) {
        return;
    }

    /* Pop all elements and optionally destroy their data */
    while (!ListIsEmpty(*_pList)) {
        void* item = ListPopHead(*_pList);
        if (_elementDestroy != NULL) {
            _elementDestroy(item);
        }
    }

    free(*_pList);
    *_pList = NULL;
}

ListItr ListPushHead(List* _list, void* _item) {
    if (NULL == _list) return NULL;
    /* Inserting before the first real element (which is head.next) */
    return ListItrInsertBefore(ListItrBegin(_list), _item);
}

ListItr ListPushTail(List* _list, void* _item) {
    if (NULL == _list) return NULL;
    /* Inserting before the Tail Sentinel effectively puts it at the end */
    return ListItrInsertBefore(ListItrEnd(_list), _item);
}

void* ListPopHead(List* _list) {
    if (NULL == _list || ListIsEmpty(_list)) return NULL;
    /* Remove the first real element */
    return ListItrRemove(ListItrBegin(_list));
}

void* ListPopTail(List* _list) {
    if (NULL == _list || ListIsEmpty(_list)) return NULL;
    /* Remove the last real element (the one before Tail Sentinel) */
    return ListItrRemove(ListItrPrev(ListItrEnd(_list)));
}

ListItr ListItrBegin(const List* _list) {
    if (NULL == _list) return NULL;
    return (ListItr)_list->m_head.m_next;
}

ListItr ListItrEnd(const List* _list) {
    if (NULL == _list) return NULL;
    /* The Tail Sentinel itself represents the 'end' (one past the last element) */
    return (ListItr)&_list->m_tail;
}

ListItr ListItrNext(ListItr _itr) {
    if (NULL == _itr) return NULL;
    Node* node = (Node*)_itr;
    /* If we are at the Tail Sentinel, we cannot go further */
    if (node->m_next == NULL) {
        return _itr;
    }
    return (ListItr)node->m_next;
}

ListItr ListItrPrev(ListItr _itr) {
    if (NULL == _itr) return NULL;
    Node* node = (Node*)_itr;
    /* If previous node is Head Sentinel, we stay at the current (first) node */
    if (node->m_prev->m_prev == NULL) {
        return _itr;
    }
    return (ListItr)node->m_prev;
}

void* ListItrGet(ListItr _itr) {
    if (NULL == _itr) return NULL;
    Node* node = (Node*)_itr;
    /* Return data only if not pointing at Sentinels */
    return node->m_data;
}

void* ListItrSet(ListItr _itr, void* _element) {
    if (NULL == _itr) return NULL;
    Node* node = (Node*)_itr;
    void* oldData = node->m_data;
    node->m_data = _element;
    return oldData;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element) {
    if (NULL == _itr) return NULL;
    Node* nextNode = (Node*)_itr;
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (NULL == newNode) return NULL;

    newNode->m_data = _element;

    /* Linking the new node to its neighbors */
    newNode->m_next = nextNode;
    newNode->m_prev = nextNode->m_prev;

    /* Updating neighbors to point to the new node */
    nextNode->m_prev->m_next = newNode;
    nextNode->m_prev = newNode;

    return (ListItr)newNode;
}

void* ListItrRemove(ListItr _itr) {
    if (NULL == _itr) return NULL;
    Node* nodeToRemove = (Node*)_itr;
    void* data = nodeToRemove->m_data;

    /* Bystanding the node by connecting its neighbors directly */
    nodeToRemove->m_prev->m_next = nodeToRemove->m_next;
    nodeToRemove->m_next->m_prev = nodeToRemove->m_prev;

    free(nodeToRemove);
    return data;
}

size_t ListSize(const List* _list) {
    size_t count = 0;
    ListItr itr;
    if (NULL == _list) return 0;

    for (itr = ListItrBegin(_list); itr != ListItrEnd(_list); itr = ListItrNext(itr)) {
        count++;
    }
    return count;
}

size_t ListIsEmpty(List* _list) {
    if (NULL == _list) return 1;
    /* List is empty if Head points directly to Tail */
    return (_list->m_head.m_next == &_list->m_tail);
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context) {
    ListItr itr = _begin;
    if (NULL == _action) return _end;

    while (itr != _end) {
        /* If action returns 0, we stop the iteration */
        if (_action(ListItrGet(itr), _context) == 0) {
            break;
        }
        itr = ListItrNext(itr);
    }
    return itr;
}