#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

/* --- Error Codes --- */
#define OK 0
#define NULL_POINTER -1
#define Overflow -2
#define Resize -3
#define Underflow -4

/* --- Structs --- */

typedef struct dynamic_array_struct {
    int *dArray;
    unsigned dArraySize;
    int dArrayBlockSize;
    int NumOfElements;
} darr;

/* Forward declaration for the 'ap' struct used in sorting.
 * Make sure the full struct is defined in your code. */
typedef struct ap ap;

/* --- Function Prototypes --- */

/* DESCRIPTION:
 * Validates the parameters before creating a dynamic array.
 *
 * PARAMS:
 * initialSize - initial size of the array (unsigned)
 * blockSize - block size for resizing (int)
 *
 * RETURN:
 * OK if valid, NULL_POINTER if both are 0 or blockSize is negative.
 */
int check_parameters(unsigned initialSize, int blockSize);

/* DESCRIPTION:
 * Allocates and initializes a new dynamic array.
 *
 * PARAMS:
 * initialSize - initial maximum capacity of the array (unsigned)
 * blockSize - number of elements to add when resizing (int)
 *
 * RETURN:
 * Pointer to the new dynamic array (darr*), or NULL if allocation fails.
 */
darr* DynamicArrayCreate(unsigned initialSize, int blockSize);

/* DESCRIPTION:
 * Frees all memory associated with the dynamic array.
 *
 * PARAMS:
 * arr - pointer to the dynamic array to destroy (darr*)
 *
 * RETURN:
 * void
 */
void DynamicArrayDestroy(darr* arr);

/* DESCRIPTION:
 * Inserts a new integer into the dynamic array, resizing it if necessary.
 *
 * PARAMS:
 * DynamicArrayPtr - pointer to the dynamic array (darr*)
 * data - the integer value to insert (int)
 *
 * RETURN:
 * OK if successful, NULL_POINTER if pointer is NULL,
 * Overflow if block size is 0 and array is full, Resize if reallocation fails.
 */
int DynamicArrayInsert(darr *DynamicArrayPtr, int data);

/* DESCRIPTION:
 * Retrieves and logically deletes the last inserted element from the array (LIFO).
 *
 * PARAMS:
 * DynamicArrayPtr - pointer to the dynamic array (darr*)
 * data - pointer to store the deleted value (int*)
 *
 * RETURN:
 * OK if successful, NULL_POINTER if any pointer is NULL, Underflow if array is empty.
 */
int DynamicArrayDelete(darr *DynamicArrayPtr, int* data);

/* DESCRIPTION:
 * Sorts an array of people by their names in ascending order using Bubble Sort.
 *
 * PARAMS:
 * people_array - pointer to the people array structure (ap*)
 *
 * RETURN:
 * OK if successful, NULL_POINTER if the array pointer is NULL.
 */
int sort_people_by_name(ap* people_array);

#endif /* __DYNAMIC_ARRAY_H__ */