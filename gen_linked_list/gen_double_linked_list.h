#ifndef __NEW_GEN_DLIST_H__
#define __NEW_GEN_DLIST_H__

#include <stddef.h>

typedef struct List List;
typedef void* ListItr;

typedef int (*ListActionFunction)(void* _element, void* _context);

List*   ListCreate(void);
void    ListDestroy(List** _pList, void (*_elementDestroy)(void* _item));
ListItr ListPushHead(List* _list, void* _item);
ListItr ListPushTail(List* _list, void* _item);
void*   ListPopHead(List* _list);
void*   ListPopTail(List* _list);
ListItr ListItrBegin(const List* _list);
ListItr ListItrEnd(const List* _list);
ListItr ListItrNext(ListItr _itr);
ListItr ListItrPrev(ListItr _itr);
void*   ListItrGet(ListItr _itr);
void*   ListItrSet(ListItr _itr, void* _element);
ListItr ListItrInsertBefore(ListItr _itr, void* _element);
void*   ListItrRemove(ListItr _itr);
size_t  ListSize(const List* _list);
size_t  ListIsEmpty(List* _list);
ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context);

#endif /* __NEW_GEN_DLIST_H__ */
