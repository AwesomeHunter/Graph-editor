#include "heap.h"

Heap heapCreate(bool (*comp)(void *, void*)){
    Heap heap;
    heap.array = vectorCreate();
    heap.comp = comp;
    return heap;
}

void *heapLeft(Heap h, int index){
    if(vectorSize(h.array) >= 2*index)
        return vectorValue(h.array, 2*index - 1);
    return NULL; 
}

void *heapRight(Heap h, int index){
    if(vectorSize(h.array) >= 2*index + 1)
        return vectorValue(h.array, 2*index + 1 - 1);
    return NULL; 
}

void *heapParent(Heap h, int index){
    if(index > 1)
        return vectorValue(h.array, index / 2 - 1);
    return NULL;
}

void *heapTop(Heap h){
    return vectorValue(h.array, 0);
}

bool heapEmpty(Heap h){
    return (vectorSize(h.array) == 0);
}

void heapPush(Heap *h, void *obj){
    vectorPushBack(&h->array, obj);
    int index = vectorSize(h->array);////////////////////////////// L  <  R
    while(heapParent(*h, index) && h->comp(vectorValue(h->array, index - 1), vectorValue(h->array, index / 2 - 1))){
        void *temp = vectorValue(h->array, index - 1);
        vectorSet(&h->array, index - 1, heapParent(*h, index));
        vectorSet(&h->array, index / 2 - 1, temp);
        index /= 2;
    }
}

void heapPop(Heap *h){
    vectorSet(&h->array, 0, vectorBack(h->array));
    vectorPopBack(&h->array);
    int index = 1;
    while(heapLeft(*h, index) || heapRight(*h, index)){
        if(heapLeft(*h, index) && heapRight(*h, index)){
            if(h->comp(heapLeft(*h, index), heapRight(*h, index))){
                if(h->comp(heapLeft(*h, index), vectorValue(h->array, index - 1))){
                    void *temp = heapLeft(*h, index);
                    vectorSet(&h->array, 2*index - 1, vectorValue(h->array, index - 1));
                    vectorSet(&h->array, index - 1, temp);
                    index = index * 2;
                }else{
                    break;
                }
            }else{
                if(h->comp(heapRight(*h, index), vectorValue(h->array, index - 1))){
                    void *temp = heapRight(*h, index);
                    vectorSet(&h->array, 2*index + 1 - 1, vectorValue(h->array, index - 1));
                    vectorSet(&h->array, index - 1, temp);
                    index = index * 2 + 1;
                }else{
                    break;
                }
            }
        }else if(heapLeft(*h, index) && h->comp(heapLeft(*h, index), vectorValue(h->array, index - 1))){
            void *temp = heapLeft(*h, index);
            vectorSet(&h->array, 2*index - 1, vectorValue(h->array, index - 1));
            vectorSet(&h->array, index - 1, temp);
            index = index * 2;
        }else if(heapRight(*h, index) && h->comp(heapRight(*h, index), vectorValue(h->array, index - 1))){
            void *temp = heapRight(*h, index);
            vectorSet(&h->array, 2*index + 1 - 1, vectorValue(h->array, index - 1));
            vectorSet(&h->array, index - 1, temp);
            index = index * 2 + 1;
        }else{
            break;
        }
    }
}

void heapDestroy(Heap *h){
    vectorDestroy(&h->array);
}