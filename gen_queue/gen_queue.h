#ifndef __GENQUEUE_H__
#define __GENQUEUE_H__

#include <stdlib.h>

typedef enum {
    QUEUE_SUCCESS = 0,
    QUEUE_UNINITIALIZED_ERROR,
    QUEUE_OVERFLOW_ERROR,
    QUEUE_DATA_NOT_FOUND_ERROR,
    QUEUE_DATA_UNINITIALIZED_ERROR
} QueueResult;

typedef struct Queue Queue;
typedef void (*DestroyItem)(void* _element);
typedef int  (*ActionFunction)(void* _element, void* _context);

Queue*      QueueCreate(size_t _size);
void        QueueDestroy(Queue** _queue, DestroyItem _itemDestroy);
QueueResult QueueInsert(Queue* _queue, void* _item);
QueueResult QueueRemove(Queue* _queue, void** _item);
size_t      QueueIsEmpty(Queue* _queue);
size_t      QueueForEach(Queue* _queue, ActionFunction _action, void* _context);

#endif
