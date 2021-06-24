#ifndef H_UTILS
#define H_UTILS

#include <stdbool.h>
#include "../graph.h"

bool isVisited(Vertex *ptr);
bool inQueue(Vertex *ptr);
bool isActive(Vertex *ptr);
void makeVisited(Vertex *v);
void makeActive(Vertex *v);
void makeInQueue(Vertex *v);

#endif