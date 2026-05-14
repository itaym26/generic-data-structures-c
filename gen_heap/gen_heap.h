#ifndef __GENHEAP_H__
#define __GENHEAP_H__

#include "gen_vector.h"
#include <stddef.h>

typedef enum Heap_ResultCode {
    HEAP_SUCCESS = 0,
    HEAP_NOT_INITIALIZED,
    HEAP_REALLOCATION_FAILED
} HeapResultCode;

typedef struct Heap Heap;
typedef int (*HeapActionFunction)(const void* _elem, void* _context);
typedef int (*Comparator)(const void* _left, const void* _right);

Heap*        HeapBuild(Vector* _vector, Comparator _pfComp);
Vector*      HeapDestroy(Heap** _heap);
HeapResultCode HeapInsert(Heap* _heap, void* _element);
const void*  HeapPeek(const Heap* _heap);
void*        HeapExtract(Heap* _heap);
size_t       HeapSize(const Heap* _heap);
size_t       HeapForEach(const Heap* _heap, HeapActionFunction _act, void* _context);

#endif
