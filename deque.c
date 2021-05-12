#include "deque.h"

#include <stdlib.h>

Deque dequeCreate(){
    Deque q;
    q.front = NULL;
    q.back = NULL;
    return q;
}

bool dequeEmpty(Deque q){
    return (q.front == NULL);
}

void dequeDelete(Deque *q, void *v){
    Node *cur_node = q->back;
    while(cur_node){
        if(cur_node->val == v){
            if(cur_node != q->back)
                cur_node->prev->next = cur_node->next;
            else
                q->back = cur_node->next;
            if(cur_node != q->back)
                cur_node->next->prev = cur_node->prev;
            else
                q->front = cur_node->prev;
            free(cur_node);
            break;
        }
        cur_node = cur_node->next;
    }
}

void dequePushBack(Deque *q, void *v){
    Node *temp = malloc(sizeof(Node));
    temp->prev = NULL;
    temp->next = NULL;
    temp->val = v;
    if(dequeEmpty(*q)){
        q->back = temp;
        q->front = temp;
    }else{
        temp->next = q->back;
        q->back->prev = temp;
        q->back = temp;
    }
}

void dequePushFront(Deque *q, void *v){
    Node *temp = malloc(sizeof(Node));
    temp->prev = NULL;
    temp->next = NULL;
    temp->val = v;
    if(dequeEmpty(*q)){
        q->back = temp;
        q->front = temp;
    }else{
        temp->prev = q->front;
        q->front->next = temp;
        q->front = temp;
    }
}

void dequePopBack(Deque *q){
    Node *temp = q->back;
    if(q->back == q->front){
        q->front = NULL;
        q->back = NULL;
    }else{
        q->back->next->prev = NULL;
        q->back = q->back->next;
    }
    free(temp);
}

void dequePopFront(Deque *q){
    Node *temp = q->front;
    if(q->back == q->front){
        q->front = NULL;
        q->back = NULL;
    }else{
        q->front->prev->next = NULL;
        q->front = q->front->prev;
    }
    free(temp);
}

void *dequeBack(Deque q){
    return q.back->val;
}

void *dequeFront(Deque q){
    return q.front->val;
}

void dequeDestroy(Deque *q){
    free(q);
}