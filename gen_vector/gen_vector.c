#include <stdio.h>
#include <stdlib.h>
#include "gen_vector.h"

typedef struct vector
{
    void** m_items;         
    size_t m_originalSize; 
    size_t m_size;         
    size_t m_nItems;       
    size_t m_blockSize;
} vector;

vector* VectorCreate(size_t _initialCapacity, size_t _blookSize){
    vector* vec = NULL;
    if(_initialCapacity == 0 && _blookSize == 0) return NULL;
    
    vec = (vector*)malloc(sizeof(vector));
    if(vec == NULL) return NULL;

    if (_initialCapacity > 0)
    {
        vec->m_items = (void**)malloc(sizeof(void*) * _initialCapacity);
        if(vec->m_items == NULL)
        {
            free(vec);
            return NULL;
        }
    }
    else
    {
        vec->m_items = NULL;
    }
    vec->m_originalSize = _initialCapacity;
    vec->m_size = _initialCapacity;
    vec->m_nItems = 0;
    vec->m_blockSize = _blookSize;
    return vec;

}

void VectorDestroy(vector** _vector, void (*_elementDestroy)(void* _item)){
    if(_vector == NULL || *_vector == NULL) return;
    if(_elementDestroy != NULL)
    {
        for(size_t i = 0; i < (*_vector)->m_nItems; ++i)
        {
            _elementDestroy((*_vector)->m_items[i]);
        }
    }
    free((*_vector)->m_items);
    free(*_vector);
    *_vector = NULL;
}
 
VectorResult VectorAppend(vector* _vector, void* _item){
    if(_vector == NULL) return VECTOR_UNINITIALIZED_ERROR;
    if(_vector->m_nItems == _vector->m_size)
    {
        if(_vector->m_blockSize == 0) return VECTOR_OVERFLOW;
        size_t newSize = _vector->m_size + _vector->m_blockSize;
        void** newItems = (void**)realloc(_vector->m_items, sizeof(void*) * newSize);
        if(newItems == NULL) return VECTOR_ALLOCATION_ERROR;
        _vector->m_items = newItems;
        _vector->m_size = newSize;
    }
    _vector->m_items[_vector->m_nItems] = _item;
    ++_vector->m_nItems;
    return VECTOR_SUCCESS;
}

VectorResult VectorRemove(vector* _vector, void** _pValue){
    if(_vector == NULL || _pValue == NULL) return VECTOR_UNINITIALIZED_ERROR;
    if(_vector->m_nItems == 0) return VECTOR_UNDERFLOW;
    if(_pValue == NULL) return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;

    *_pValue = _vector->m_items[_vector->m_nItems -1];
     --_vector->m_nItems;

    return VECTOR_SUCCESS;
}

VectorResult VectorGet(const vector* _vector, size_t _index, void** _pValue){
    if(_vector == NULL || _pValue == NULL) return VECTOR_UNINITIALIZED_ERROR;
    if(_index >= _vector->m_nItems) return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;

    *_pValue = _vector->m_items[_index];
    return VECTOR_SUCCESS;
}

VectorResult VectorSet(vector* _vector, size_t _index, void* _item){
    if(_vector == NULL) return VECTOR_UNINITIALIZED_ERROR;
    if(_index >= _vector->m_nItems) return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;

    _vector->m_items[_index] = _item;
    return VECTOR_SUCCESS;
}

size_t VectorSize(const vector* _vector){
    if(_vector == NULL) return 0;
    return _vector->m_nItems;
}

size_t VectorCapacity(const vector* _vector){
    if(_vector == NULL) return 0;
    return _vector->m_size;
}

size_t VectorForEach(const vector* _vector, VectorElementAction _action, void* _context){
    if(_vector == NULL || _action == NULL) return -1;
    size_t count = 0;
    for(size_t i = 0; i < _vector->m_nItems; ++i)
    {
        if(_action(_vector->m_items[i], i, _context) == 0)
        {
            ++count;
        }
        else
        {
            break;
        }
    }
    return count;
}
