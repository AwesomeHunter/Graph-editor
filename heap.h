#ifndef H_HEAP
#define H_HEAP

#include <stdbool.h>

#include "vector.h"

typedef struct Temp Temp;
typedef struct Heap{
    Vector array;
    bool (*comp)(void *, void*);
} Heap;

Heap heapCreate(bool (*comp)(void *, void*));
void *heapLeft(Heap h, int index);
void *heapRight(Heap h, int index);
void *heapParent(Heap h, int index);
void *heapTop(Heap h);
bool heapEmpty(Heap h);
void heapPush(Heap *h, void *obj);
void heapPop(Heap *h);
void heapDestroy(Heap *h);

#endif