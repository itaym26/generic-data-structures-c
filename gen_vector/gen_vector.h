#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

typedef struct Vector Vector;
typedef int (*VectorElementAction)(void* _element, size_t _index, void* _context);

typedef enum Vector_Result {
    VECTOR_SUCCESS = 0,
    VECTOR_UNINITIALIZED_ERROR,
    VECTOR_ALLOCATION_ERROR,
    VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
    VECTOR_OVERFLOW,
    VECTOR_UNDERFLOW
} VectorResult;

Vector*      VectorCreate(size_t _initialCapacity, size_t _blockSize);
void         VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item));
VectorResult VectorAppend(Vector* _vector, void* _item);
VectorResult VectorRemove(Vector* _vector, void** _pValue);
VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue);
VectorResult VectorSet(Vector* _vector, size_t _index, void* _value);
size_t       VectorSize(const Vector* _vector);
size_t       VectorCapacity(const Vector* _vector);
size_t       VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context);

#endif
