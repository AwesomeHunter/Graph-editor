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

int vectorCapacity(Vector *v){
    return v->capacity;
}

void *vectorValue(struct Vector *v, int index){
    return v->vector[index];
}

void *vectorBack(struct Vector *v){
    return vectorValue(v, vectorSize(v)-1);
}

bool vectorEmpty(struct Vector *v){
    return (vectorSize(v) == 0);
}

void vectorPushBack(struct Vector *v, void *obj){
    if(vectorSize(v) == vectorCapacity(v))
        vectorResize(v);
    vectorSet(v, vectorSize(v), obj);
    v->size += 1;
}

void vectorPopBack(struct Vector *v){
    v->size -= 1;
}

void vectorRemove(Vector *v, void *obj){
    int new_good_index = 0;
    for(int i=0; i<vectorSize(v); i++){
        if(vectorValue(v, i) == obj){
            new_good_index = i;
            while(new_good_index < vectorSize(v) && vectorValue(v, new_good_index) == obj)
                new_good_index++;
            if(new_good_index == vectorSize(v))
                break;
            vectorSet(v, i, vectorValue(v, new_good_index));
            vectorSet(v, new_good_index, obj);
        }
    }
    while(!vectorEmpty(v) && vectorBack(v) == obj)
        vectorPopBack(v);
}

void vectorSet(Vector *v, int index, void *obj){
    v->vector[index] = obj;
}

void vectorDestroy(struct Vector *v){
    free(v->vector);
}