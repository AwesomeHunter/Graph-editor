#ifndef H_HEAP
#define H_HEAP

#include <stdbool.h>

#include "vector.h"

typedef struct Heap{
    Vector array;
    bool (*comp)(void *, void*);
} Heap;

Heap heapCreate(bool (*comp)(void *, void*));

bool heapIsIndexCorrect(Heap *h, int index);

void *heapLeft(Heap *h, int index);
void *heapRight(Heap *h, int index);
void *heapParent(Heap *h, int index);
void *heapValue(Heap *h, int index);
void *heapTop(Heap *h);

void heapSetLeft(Heap *h, int index, void *obj);
void heapSetRight(Heap *h, int index, void *obj);
void heapSetParent(Heap *h, int index, void *obj);
void heapSetValue(Heap *h, int index, void *obj);

void heapSwapLeft(Heap* h, int index);
void heapSwapRight(Heap* h, int index);
void heapSwapParent(Heap* h, int index);

bool heapIsSmallerThan(Heap *h, void *obj_a, void *obj_b);

int heapSize(Heap* h);

bool heapEmpty(Heap *h);
void heapPush(Heap *h, void *obj);
void heapPop(Heap *h);

void heapDestroy(Heap *h);

#endif