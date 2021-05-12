#ifndef H_DEQUE
#define H_DEQUE

#include <stdbool.h>

typedef struct Node{
    struct Node *prev, *next;
    void *val;
} Node;

typedef struct Deque{
    Node *front, *back;
} Deque;

Deque dequeCreate();
void dequeDestroy(Deque *q);
void dequeDelete(Deque *q, void *v);
bool dequeEmpty(Deque q);
void dequePushBack(Deque *q, void *v);
void dequePushFront(Deque *q, void *v);
void dequePopBack(Deque *q);
void dequePopFront(Deque *q);
void *dequeBack(Deque q);
void *dequeFront(Deque q);

#endif