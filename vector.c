#include "vector.h"

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

int vectorSize(struct Vector v){
    return v.size;
}

void *vectorValue(struct Vector v, int index){
    return v.vector[index];
}

void *vectorBack(struct Vector v){
    return v.vector[v.size-1];
}

bool vectorEmpty(struct Vector v){
    return (v.size == 0);
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

void vectorErase(Vector *v, void *obj){
    for(int i=0; i<vectorSize(*v)-1; i++){
        if(vectorValue(*v, i) == obj){
            v->vector[i] = v->vector[i+1];
            v->vector[i+1] = obj;
        }
    }
    if(!vectorEmpty(*v) && vectorBack(*v) == obj)
        vectorPopBack(v);
}

void vectorSet(Vector *v, int index, void *val){
    v->vector[index] = val;
}

int vectorBinsearch(Vector v, void *val, bool (*comp)(void *, void*), bool (*eq)(void *, void*)){
    int l = 0;
    int r = vectorSize(v) - 1;
    while(l<r){
        int mid = (l+r+1)/2;
        if(comp(vectorValue(v, mid), val)){
            l = mid;
        }else{
            r = mid - 1;
        }
    }
    if(eq(vectorValue(v, l), val))
        return l;
    return -1;
}

void vectorDestroy(struct Vector *v){
    free(v->vector);
}