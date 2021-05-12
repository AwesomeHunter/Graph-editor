#include "vector.h"

#include <stdlib.h>

Vector vectorCreate(){
    Vector vector;
    vector.size = 0;
    vector.capacity = 1;
    vector.vector = malloc(sizeof(void *));
    return vector;
}

void vectorResize(struct Vector *v){
    v->capacity *= 2;
    v->vector = realloc(v->vector, v->capacity * sizeof(void *));
}

int vectorSize(struct Vector *v){
    return v->size;
}

void *vectorValue(struct Vector *v, int index){
    return v->vector[index];
}

void *vectorBack(struct Vector *v){
    return v->vector[v->size-1];
}

bool vectorEmpty(struct Vector *v){
    return (v->size == 0);
}

void vectorPushBack(struct Vector *v, void *obj){
    if(v->size == v->capacity)
        vectorResize(v);
    v->vector[v->size] = obj;
    v->size += 1;
}

void vectorPopBack(struct Vector *v){
    v->size -= 1;
}

void vectorRemove(Vector *v, void *obj){
    for(int i=0; i<vectorSize(v)-1; i++){
        if(vectorValue(v, i) == obj){
            v->vector[i] = v->vector[i+1];
            v->vector[i+1] = obj;
        }
    }
    if(!vectorEmpty(v) && vectorBack(v) == obj)
        vectorPopBack(v);
}

void vectorSet(Vector *v, int index, void *obj){
    v->vector[index] = obj;
}

void vectorDestroy(struct Vector *v){
    free(v->vector);
}