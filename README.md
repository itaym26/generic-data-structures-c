# 🧱 Generic Data Structures in C

> A collection of fully generic, reusable data structure implementations in C — built with `void*` pointers and function pointer callbacks to support any data type.

---

## 📁 Project Structure
gen_DS/
├── gen_alloc/          # Dynamic array (int-based, early stage)
├── gen_vector/         # Generic dynamic array (resizable)
├── gen_stack/          # Generic stack (LIFO)
├── gen_queue/          # Generic queue (FIFO, circular buffer)
├── gen_linked_list/    # Generic doubly linked list with iterators
├── gen_BST/            # Generic binary search tree with sentinel
├── gen_heap/           # Generic max/min heap (built on vector)
└── gen_hash/           # Generic hash map (separate chaining)

---

## 📦 Data Structures

### 🔢 Dynamic Array — `gen_alloc`
- `DynamicArrayCreate(initialSize, blockSize)`
- `DynamicArrayInsert(arr, data)`
- `DynamicArrayDelete(arr, &data)`
- `DynamicArrayDestroy(arr)`

### 📐 Generic Vector — `gen_vector`
- `VectorCreate(initialCapacity, blockSize)` — O(1)
- `VectorAppend(vector, item)` — O(1) amortized
- `VectorRemove(vector, &pValue)` — O(1)
- `VectorGet(vector, index, &pValue)` — O(1)
- `VectorSet(vector, index, value)` — O(1)
- `VectorForEach(vector, action, context)`

### 📚 Generic Stack — `gen_stack`
- `StackCreate(initialCapacity, blockSize)`
- `StackPush(stack, item)` — O(1)
- `StackPop(stack, &pValue)` — O(1)
- `StackTop(stack, &pValue)` — O(1)
- `StackIsEmpty(stack)`

### 🔄 Generic Queue — `gen_queue`
- `QueueCreate(size)`
- `QueueInsert(queue, item)` — O(1)
- `QueueRemove(queue, &item)` — O(1)
- `QueueIsEmpty(queue)`
- `QueueForEach(queue, action, context)`

### 🔗 Generic Doubly Linked List — `gen_linked_list`
- `ListCreate()` / `ListDestroy()`
- `ListPushHead(list, item)` / `ListPushTail(list, item)` — O(1)
- `ListPopHead(list)` / `ListPopTail(list)` — O(1)
- `ListItrBegin(list)` / `ListItrEnd(list)`
- `ListItrNext(itr)` / `ListItrPrev(itr)`
- `ListItrForEach(begin, end, action, context)`

### 🌳 Generic BST — `gen_BST`
- `BSTreeCreate(comparator)`
- `BSTreeInsert(tree, item)` — O(log n) avg
- `BSTreeItrBegin(tree)` / `BSTreeItrEnd(tree)`
- `BSTreeItrNext(itr)` / `BSTreeItrPrev(itr)` — O(1)
- `BSTreeItrRemove(itr)` — O(1)
- `BSTreeForEach(tree, PREORDER | INORDER | POSTORDER, action, context)`

### ⛰️ Generic Heap — `gen_heap`
- `HeapBuild(vector, comparator)` — O(n)
- `HeapInsert(heap, element)` — O(log n)
- `HeapPeek(heap)` — O(1)
- `HeapExtract(heap)` — O(log n)
- `HeapForEach(heap, action, context)`

### 🗂️ Generic Hash Map — `gen_hash`
- `HashMap_Create(capacity, hashFunc, keysEqualFunc)`
- `HashMap_Insert(map, key, value)` — O(1) avg
- `HashMap_Find(map, key, &pValue)` — O(1) avg
- `HashMap_Remove(map, searchKey, &pKey, &pValue)` — O(1) avg
- `HashMap_Rehash(map, newCapacity)` — O(n)
- `HashMap_ForEach(map, action, context)`

---

## ⏱️ Time Complexity Summary

| Data Structure     | Insert         | Delete       | Search       | Access       | Space |
|--------------------|:--------------:|:------------:|:------------:|:------------:|:-----:|
| Dynamic Array      | O(1) amort.    | O(1)         | O(n)         | O(1)         | O(n)  |
| Vector             | O(1) amort.    | O(1)         | O(n)         | O(1)         | O(n)  |
| Stack              | O(1)           | O(1)         | —            | O(1) top     | O(n)  |
| Queue              | O(1)           | O(1)         | —            | O(1) front   | O(n)  |
| Doubly Linked List | O(1) head/tail | O(1) at itr  | O(n)         | O(n)         | O(n)  |
| BST                | O(log n) avg   | O(1) at itr  | O(log n) avg | O(log n) avg | O(n)  |
| Heap               | O(log n)       | O(log n)     | O(n)         | O(1) top     | O(n)  |
| Hash Map           | O(1) avg       | O(1) avg     | O(1) avg     | O(1) avg     | O(n)  |

---

## 🧬 Architecture & Design

- **Generic via `void*`** — any data type can be stored
- **Function pointer callbacks** — for comparison, equality, hashing, destruction, and iteration
- **Iterator pattern** — BST and Linked List expose opaque iterator types
- **Lazy allocation** — Hash map chains allocated only when needed
- **Sentinel nodes** — BST uses a sentinel to simplify edge-case handling

---

## 🚀 Usage Example

```c
#include "gen_vector/gen_vector.h"
#include <stdio.h>

int printInt(void* elem, size_t idx, void* ctx) {
    printf("[%zu] = %d\n", idx, *(int*)elem);
    return 1;
}

int main(void) {
    Vector* v = VectorCreate(4, 4);
    int a = 10, b = 20, c = 30;
    VectorAppend(v, &a);
    VectorAppend(v, &b);
    VectorAppend(v, &c);
    VectorForEach(v, printInt, NULL);
    VectorDestroy(&v, NULL);
    return 0;
}
```

---

## 🛠️ Compilation

```bash
gcc -Wall -Wextra -o vector_test gen_vector/gen_vector.c gen_vector/main.c
```

---

## 👤 Author

Built as part of the **Embedded & Real-Time** course —
[Reichman University & Google School of Hi-Tech](https://www.runi.ac.il/en/schools/hi-tech/)
