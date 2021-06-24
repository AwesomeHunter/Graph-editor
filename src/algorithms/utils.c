#include "utils.h"
#include "../colors.h"
#include <math.h>

bool equal(Color a, Color b){
    double delta = 1e-4;
    return (fabs(a.r - b.r) < delta && fabs(a.g - b.g) < delta && fabs(a.b - b.b) < delta);
}

bool isVisited(Vertex *ptr){
    return equal(vertexGetColor(ptr), VISITED_VERTEX);
}

bool inQueue(Vertex *ptr){
    return equal(vertexGetColor(ptr), IN_QUEUE_VERTEX);
}

bool isActive(Vertex *ptr){
    return equal(vertexGetColor(ptr), ACTIVE_VERTEX);
}

void makeVisited(Vertex *v){
    vertexSetColor(v, VISITED_VERTEX);
}

void makeActive(Vertex *v){
    vertexSetColor(v, ACTIVE_VERTEX);
}

void makeInQueue(Vertex *v){
    vertexSetColor(v, IN_QUEUE_VERTEX);
}