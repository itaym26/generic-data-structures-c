#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

/* Helper function to print integers during VectorForEach */
int PrintInt(void* _element, size_t _index, void* _context) {
    int* num = (int*)_element;
    printf("Index %zu: Value %d\n", _index, *num);
    return 1; /* Return 1 to continue iteration */
}

/* Helper function to free the dynamically allocated integer memory */
void DestroyInt(void* _item) {
    free(_item);
}

int main() {
    Vector* vec;
    int* pVal;
    int i;
    VectorResult res;

    /* 1. Create the vector with an initial capacity of 5 and a block size of 2 */
    vec = VectorCreate(5, 2);
    if (!vec) {
        printf("Failed to create vector\n");
        return 1;
    }

    /* 2. Add elements to the vector
       Note: We must allocate memory on the heap because the vector 
       stores pointers (void*), not the actual values. */
    for (i = 0; i < 7; ++i) {
        int* n = malloc(sizeof(int));
        *n = (i + 1) * 10;
        res = VectorAppend(vec, n);
        if (res != VECTOR_SUCCESS) {
            printf("Failed to append at index %d\n", i);
        }
    }

    /* 3. Print the vector content using VectorForEach */
    printf("--- Vector content ---\n");
    VectorForEach(vec, PrintInt, NULL);

    /* 4. Access a specific element using VectorGet */
    res = VectorGet(vec, 2, (void**)&pVal);
    if (res == VECTOR_SUCCESS) {
        printf("Value at index 2 is: %d\n", *pVal);
    }

    /* 5. Remove an element using VectorRemove
       The vector stops "tracking" the pointer, but we must free the memory 
       ourselves if it was dynamically allocated. */
    res = VectorRemove(vec, (void**)&pVal);
    if (res == VECTOR_SUCCESS) {
        printf("Removed value: %d\n", *pVal);
        free(pVal); /* Responsibility to free the memory */
    }

    /* 6. Destroy the vector
       We pass the DestroyInt function so the vector can clean up 
       all remaining items inside it. */
    VectorDestroy(&vec, DestroyInt);
    printf("Vector destroyed successfully.\n");

    return 0;
}