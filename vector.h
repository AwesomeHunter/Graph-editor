#ifndef H_VECTOR
#define H_VECTOR

#include <stdbool.h>

typedef struct Vector{
    void **vector;
    size_t size;
    size_t capacity;
} Vector;

Vector vectorCreate();
void vectorResize(Vector *v);

int vectorSize(Vector *v);
int vectorCapacity(Vector *v);
void *vectorValue(Vector *v, int index);
void *vectorBack(Vector *v);

bool vectorEmpty(Vector *v);
void vectorPushBack(Vector *v, void *obj);
void vectorPopBack(Vector *v);
void vectorRemove(Vector *v, void *obj);
void vectorSet(Vector *v, int index, void *obj);

void vectorDestroy(Vector *v);

#endif