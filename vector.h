#ifndef H_VECTOR
#define H_VECTOR

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

typedef struct Vector{
    void **vector;
    size_t size;
    size_t capacity;
} Vector;

Vector vectorCreate();
void vectorResize(Vector *v);

int vectorSize(Vector v);
void *vectorValue(Vector v, int index);
void *vectorBack(Vector v);

bool vectorEmpty(Vector v);
void vectorPushBack(Vector *v, void *obj);
void vectorPopBack(Vector *v);
void vectorErase(Vector *v, void *obj);
void vectorSet(Vector *v, int index, void *val);
int vectorBinsearch(Vector v, void *val, bool (*comp)(void *, void*), bool (*eq)(void *, void*));

void vectorDestroy(Vector *v);

#endif