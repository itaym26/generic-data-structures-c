# include <stdio.h>
# include <stdlib.h>
# include "stack.h"
# include "../gen_vector/gen_vector.h"

struct Stack
{
    Vector* m_vector;
};

Stack* StackCreate(size_t _initialCapacity, size_t _blockSize){
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(stack == NULL) return NULL;
    if(_initialCapacity == 0 && _blockSize == 0){
        return NULL;
    }
    stack->m_vector = VectorCreate(_initialCapacity, _blockSize);
    if(stack->m_vector == NULL) {
        free(stack);
        return NULL;
    }
    return stack;
}

void StackDestroy(Stack** _stack, void (*_elementDestroy)(void* _item)){
    if(_stack == NULL || *_stack == NULL) return;
    VectorDestroy(&((*_stack)->m_vector), _elementDestroy);
    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack* _stack, void* _item){
    if(_stack == NULL || _item == NULL) return STACK_UNITIALIZED_ERROR;
    if(VectorAppend(_stack->m_vector, _item) != VECTOR_SUCCESS) return STACK_ALLOCATION_ERROR;
    return STACK_SUCCESS;
}

StackResult StackPop(Stack* _stack, void** _pValue){
    if(_stack == NULL || _pValue == NULL) return STACK_UNITIALIZED_ERROR;
    if(VectorSize(_stack->m_vector) == 0) return STACK_IS_EMPTY;
    return VectorRemove(_stack->m_vector, _pValue) == VECTOR_SUCCESS ? STACK_SUCCESS : STACK_ALLOCATION_ERROR;
}

StackResult StackTop(Stack* _stack, void** _pValue){
    if(_stack == NULL || _pValue == NULL) return STACK_UNITIALIZED_ERROR;
    if(StackIsEmpty(_stack)) return STACK_IS_EMPTY;

    size_t LastIndex = VectorSize(_stack->m_vector) - 1;
    return VectorGet(_stack->m_vector, LastIndex, _pValue) == VECTOR_SUCCESS ? STACK_SUCCESS : STACK_ALLOCATION_ERROR;
}

size_t StackSize(const Stack* _stack){
    if(_stack == NULL) return 0;
    return VectorSize(_stack->m_vector);
}

size_t StackCapacity(const Stack* _stack){
    if(_stack == NULL) return 0;
    return VectorCapacity(_stack->m_vector);
}

int StackIsEmpty(Stack* _stack){
    if(_stack == NULL) return 1;
    return VectorSize(_stack->m_vector) == 0;
}

size_t StackPrint(const Stack* _stack, StackElementAction _action, void* _context){
    if(_stack == NULL || _action == NULL) return 0;
    return VectorForEach(_stack->m_vector, (int (*)(void*, size_t, void*))_action, _context);
}
