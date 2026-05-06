#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h" 

int check_parameters(unsigned initialSize, int blockSize) {
    if (initialSize == 0 && blockSize == 0) return NULL_POINTER;
    if (blockSize < 0) return NULL_POINTER;
    return OK;
}

darr* DynamicArrayCreate(unsigned initialSize, int blockSize) {
    if (check_parameters(initialSize, blockSize) != OK) return NULL; // Check parameters validity

    darr* newArry = (darr*)malloc(sizeof(darr)); // Allocate memory for the dynamic array structure
    if (newArry == NULL) return NULL;

    newArry->dArray = (int*)malloc(initialSize * sizeof(int)); // Allocate memory for the array of integers
    if (newArry->dArray == NULL) {
        free(newArry);
        return NULL;
    }

    newArry->dArraySize = initialSize;
    newArry->dArrayBlockSize = blockSize;
    newArry->NumOfElements = 0;

    return newArry;  
}

void DynamicArrayDestroy(darr* arr) {
    if (arr == NULL) return; // Check for null pointer
    free(arr->dArray); // Free the array of integers
    free(arr); // Free the dynamic array structure
}

int DynamicArrayInsert(darr *DynamicArrayPtr, int data) {
    if (DynamicArrayPtr == NULL) return NULL_POINTER; // Check for null pointer

    if (DynamicArrayPtr->NumOfElements >= DynamicArrayPtr->dArraySize) { // Check if resizing is needed
        if (DynamicArrayPtr->dArrayBlockSize == 0) return Overflow; // No block size defined, cannot resize

        int newSize = DynamicArrayPtr->dArraySize + DynamicArrayPtr->dArrayBlockSize; // Calculate new size
        int* temp = (int*)realloc(DynamicArrayPtr->dArray, newSize * sizeof(int)); // Attempt to resize the array
        if (temp == NULL) return Resize; // Check for successful resizing

        DynamicArrayPtr->dArray = temp; // Update the array pointer
        DynamicArrayPtr->dArraySize = newSize; // Update the size
    } 

    DynamicArrayPtr->dArray[DynamicArrayPtr->NumOfElements] = data; // Insert the new data 
    DynamicArrayPtr->NumOfElements++; // Increment the number of elements
    return OK;
}

int DynamicArrayDelete(darr *DynamicArrayPtr, int* data) {
    if (DynamicArrayPtr == NULL || data == NULL) return NULL_POINTER; // Check for null pointers

    if (DynamicArrayPtr->NumOfElements == 0) return Underflow; // Check if the array is empty

    // Extract the last element and store it in the provided pointer
    *data = DynamicArrayPtr->dArray[DynamicArrayPtr->NumOfElements - 1]; 
    
    DynamicArrayPtr->NumOfElements--; // Decrement the number of elements (logical deletion only)
    
    return OK;
}

