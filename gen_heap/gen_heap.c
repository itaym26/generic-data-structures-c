#include <stdlib.h>
#include "gen_heap.h"
#include "gen_vector.h"

/* Macros for calculating heap indices (assuming a 0-based array) */
#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i)   (2 * (i) + 1)
#define RIGHT(i)  (2 * (i) + 2)
#define HEAP_MAGIC 0xDEADBEEF

struct Heap {
    Vector* m_vec;       // The underlying Vector storing the elements (matching your struct)
    Comparator m_comp;   // The comparison function
    unsigned int m_magic;// Magic number to protect against double free
};


// Swaps two elements in the underlying Vector
static void Swap(Vector* _vec, size_t _i, size_t _j) {
    void *val1, *val2;
    VectorGet(_vec, _i, &val1);
    VectorGet(_vec, _j, &val2);
    VectorSet(_vec, _i, val2);
    VectorSet(_vec, _j, val1);
}

// HeapifyDown: Restores the heap property from a parent down to its children
static void HeapifyDown(Heap* _heap, size_t _index) {
    size_t size = VectorSize(_heap->m_vec);
    size_t left, right, extremeIdx;
    void *currentVal, *extremeVal, *childVal;

    while (1) {
        left = LEFT(_index);
        right = RIGHT(_index);
        extremeIdx = _index;

        VectorGet(_heap->m_vec, extremeIdx, &currentVal);
        extremeVal = currentVal;

        // Check against left child
        if (left < size) {
            VectorGet(_heap->m_vec, left, &childVal);
            // If the child should be above the parent (based on comparator)
            if (_heap->m_comp(extremeVal, childVal)) {
                extremeIdx = left;
                extremeVal = childVal;
            }
        }

        // Check against right child
        if (right < size) {
            VectorGet(_heap->m_vec, right, &childVal);
            if (_heap->m_comp(extremeVal, childVal)) {
                extremeIdx = right;
            }
        }

        // If the parent is already the most extreme value, we are done
        if (extremeIdx == _index) {
            break;
        }

        // Otherwise, swap with the chosen child and continue bubbling down
        Swap(_heap->m_vec, _index, extremeIdx);
        _index = extremeIdx;
    }
}

// HeapifyUp: Bubbles up a newly inserted element from the bottom of the tree
static void HeapifyUp(Heap* _heap, size_t _index) {
    size_t parentIdx;
    void *currentVal, *parentVal;

    while (_index > 0) {
        parentIdx = PARENT(_index);
        VectorGet(_heap->m_vec, _index, &currentVal);
        VectorGet(_heap->m_vec, parentIdx, &parentVal);

        // If the parent is less suitable to be on top than the child, swap them
        if (_heap->m_comp(parentVal, currentVal)) {
            Swap(_heap->m_vec, _index, parentIdx);
            _index = parentIdx;
        } else {
            break; // Reached the correct position
        }
    }
}

Heap* HeapBuild(Vector* _vector, Comparator _pfComp) {
    Heap* heap;
    size_t size, i;

    // Input validation
    if (_vector == NULL || _pfComp == NULL) {
        return NULL;
    }

    heap = (Heap*)malloc(sizeof(Heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->m_vec = _vector;
    heap->m_comp = _pfComp;
    heap->m_magic = HEAP_MAGIC;

    // Transform the existing Vector into a heap by calling HeapifyDown 
    // from the last parent node all the way up to the root
    size = VectorSize(_vector);
    if (size > 1) {
        for (i = PARENT(size - 1); ; --i) {
            HeapifyDown(heap, i);
            if (i == 0) break; // Using break to prevent size_t (unsigned) underflow
        }
    }

    return heap;
}

Vector* HeapDestroy(Heap** _heap) {
    Vector* vec;

    // Check for NULL pointer or already destroyed heap
    if (_heap == NULL || *_heap == NULL || (*_heap)->m_magic != HEAP_MAGIC) {
        return NULL;
    }

    vec = (*_heap)->m_vec;
    (*_heap)->m_magic = 0; // Invalidate the magic number
    free(*_heap);
    *_heap = NULL;

    return vec; // Return the Vector to the user so they can free it
}

HeapResultCode HeapInsert(Heap* _heap, void* _element) {
    if (_heap == NULL) {
        return HEAP_NOT_INITIALIZED;
    }

    // Append the element to the end of the Vector using your Vector API
    if (VectorAppend(_heap->m_vec, _element) != VECTOR_SUCCESS) {
        return HEAP_REALLOCATION_FAILED;
    }

    // Bubble up the new element to maintain the heap property
    HeapifyUp(_heap, VectorSize(_heap->m_vec) - 1);

    return HEAP_SUCCESS;
}

const void* HeapPeek(const Heap* _heap) {
    void* topElement = NULL;

    if (_heap == NULL || VectorSize(_heap->m_vec) == 0) {
        return NULL;
    }

    VectorGet(_heap->m_vec, 0, &topElement); // The root is always at index 0
    return topElement;
}

void* HeapExtract(Heap* _heap) {
    void *topElement = NULL, *lastElement = NULL;
    size_t size;

    if (_heap == NULL || (size = VectorSize(_heap->m_vec)) == 0) {
        return NULL;
    }

    // Save the root element which we will return
    VectorGet(_heap->m_vec, 0, &topElement);

    // If there is more than one element, swap the root with the last element
    if (size > 1) {
        Swap(_heap->m_vec, 0, size - 1);
    }

    // Remove the last element (which is now the old root) from the Vector
    VectorRemove(_heap->m_vec, &lastElement);

    // If elements remain, restore the heap structure from top to bottom
    if (size > 1) {
        HeapifyDown(_heap, 0);
    }

    return topElement;
}

size_t HeapSize(const Heap* _heap) {
    if (_heap == NULL) {
        return 0;
    }
    return VectorSize(_heap->m_vec);
}

size_t HeapForEach(const Heap* _heap, HeapActionFunction _act, void* _context) {
    size_t i, size, count = 0;
    void* elem;

    if (_heap == NULL || _act == NULL) {
        return 0;
    }

    size = VectorSize(_heap->m_vec);
    for (i = 0; i < size; ++i) {
        VectorGet(_heap->m_vec, i, &elem);
        count++; // Count how many times the function was invoked
        if (_act(elem, _context) != 0) {
            break; // Stop iteration if the user action returns non-zero
        }
    }

    return count;
}