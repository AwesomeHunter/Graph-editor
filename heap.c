#include "heap.h"

#include <stdlib.h>

Heap heapCreate(bool (*comp)(void *, void*)){
    Heap heap;
    heap.array = vectorCreate();
    heap.comp = comp; // comp: L  <  R
    return heap;
}

bool heapIsIndexCorrect(Heap *h, int index){
    return (0 <= index && index < vectorSize(&h->array));
}

void *heapLeft(Heap *h, int index){
    if(heapIsIndexCorrect(h, 2*index-1))
        return vectorValue(&h->array, 2*index-1);
    return NULL; 
}

void *heapRight(Heap *h, int index){
    if(heapIsIndexCorrect(h, 2*index))
        return vectorValue(&h->array, 2*index);
    return NULL; 
}

void *heapParent(Heap *h, int index){
    if(heapIsIndexCorrect(h, index/2-1))
        return vectorValue(&h->array, index/2-1);
    return NULL;
}

void *heapValue(Heap *h, int index){
    if(heapIsIndexCorrect(h, index-1))
        return vectorValue(&h->array, index-1);
    return NULL;
}

void *heapTop(Heap *h){
    if(heapIsIndexCorrect(h, 0))
        return vectorValue(&h->array, 0);
    return NULL;
}

int heapSize(Heap* h){
    return vectorSize(&h->array);
}

void heapSetLeft(Heap *h, int index, void *obj){
    if(heapIsIndexCorrect(h, 2*index-1))
        vectorSet(&h->array, 2*index-1, obj);
}

void heapSetRight(Heap *h, int index, void *obj){
    if(heapIsIndexCorrect(h, 2*index))
        vectorSet(&h->array, 2*index, obj);
}

void heapSetParent(Heap *h, int index, void *obj){
    if(heapIsIndexCorrect(h, index/2-1))
        vectorSet(&h->array, index/2-1, obj);
}

void heapSetValue(Heap *h, int index, void *obj){
    if(heapIsIndexCorrect(h, index-1))
        vectorSet(&h->array, index-1, obj);
}

void heapSwapLeft(Heap* h, int index){
    void *temp_value = heapValue(h, index);
    heapSetValue(h, index, heapLeft(h, index));
    heapSetLeft(h, index, temp_value);
}

void heapSwapRight(Heap* h, int index){
    void *temp_value = heapValue(h, index);
    heapSetValue(h, index, heapRight(h, index));
    heapSetRight(h, index, temp_value);
}

void heapSwapParent(Heap* h, int index){
    void *temp_value = heapValue(h, index);
    heapSetValue(h, index, heapParent(h, index));
    heapSetParent(h, index, temp_value);
}

bool heapIsSmallerThan(Heap *h, void *obj_a, void *obj_b){
    return h->comp(obj_a, obj_b);
}

bool heapEmpty(Heap *h){
    return (vectorSize(&h->array) == 0);
}

void heapPush(Heap *h, void *obj){
    vectorPushBack(&h->array, obj);
    int index = heapSize(h);
    while(heapParent(h, index) && heapIsSmallerThan(h, heapValue(h, index), heapParent(h, index))){
        heapSwapParent(h, index);
        index /= 2;
    }
}

void heapPop(Heap *h){
    vectorSet(&h->array, 0, vectorBack(&h->array));
    vectorPopBack(&h->array);
    int index = 1;
    while(true){
        void *left_val = heapLeft(h, index);
        void *right_val = heapRight(h, index);
        void *current_val = heapValue(h, index);
        bool swap_left = (left_val && heapIsSmallerThan(h, left_val, current_val));
        bool swap_right = (right_val && heapIsSmallerThan(h, right_val, current_val));
        if(swap_left && swap_right){
            bool is_left_smaller = heapIsSmallerThan(h, left_val, right_val);
            swap_left = is_left_smaller;
            swap_right = !is_left_smaller;
        }
        if(swap_left){
            heapSwapLeft(h, index);
            index = index*2;
        }else if(swap_right){
            heapSwapRight(h, index);
            index = index*2+1;
        }else{
            break;
        }
    }
}

void heapDestroy(Heap *h){
    vectorDestroy(&h->array);
}