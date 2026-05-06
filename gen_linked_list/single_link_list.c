#include <stdio.h>
#include <stdlib.h>

typedef struct Person {
    int m_id;
    char m_name[20];
    int m_age;
    struct Person* m_next;
} Person;

Person* ListInsertHead(Person* _head, Person* _p){
    if (_p == NULL) return _head; // error
    _p->m_next = _head;// insert to head
    return _p;
}

Person* ListRemoveHead(Person* _head,Person** _item){
    if (_head == NULL) return NULL; // empty list

    if (_item != NULL){
        *_item = _head;
    }
    return _head->m_next;
}

Person* ListInsertByKey(Person* _head, int _key, Person* _p){
    if (_p == NULL) return _head;// error
    if (_head == NULL) return ListInsertHead(_head, _p); // empty list

    Person* current = _head;
    Person* prev = NULL;
    // Find the node before the node to insert
    while (current != NULL && current->m_id < _key){
        prev = current;
        current = current->m_next;
    }
    // Not found or current is the node to insert
    if (prev == NULL){ // insert to head
       return ListInsertHead(_head, _p);
    } else { // insert between prev and current
        prev->m_next = _p;
        _p->m_next = current;
    }
    return _head;
}
Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p){
    if (_p == NULL) return _head;// error
    if (_head == NULL || _head->m_id >= _key) return ListInsertHead(_head, _p); // empty list or insert to head

    // Recursive call for the rest of the list
    _head->m_next = ListInsertByKeyRec(_head->m_next, _key, _p);
    return _head;
}
Person* ListRemoveByKey(Person* _head, int _key, Person** _item){
    if (_head == NULL) return NULL; // empty list

    Person* current = _head;
    Person* prev = NULL;
    // Find the node to remove
    while (current != NULL && current->m_id < _key){
        prev = current;
        current = current->m_next;
    }
    // Not found or current is the node to remove
    if (current == NULL || current->m_id != _key) return _head; // not found
    if (_item != NULL){
        *_item = current;   
    }
    if (prev == NULL){ // remove head
        _head = current->m_next;
    } else { // remove between prev and current
        prev->m_next = current->m_next;
    }
    return _head;
}

