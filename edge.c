#include "edge.h"

Edge *edgeCreate(int id, int weight, Vertex *begin, Vertex *end){
    Edge *edge = malloc(sizeof(Edge));
    edge->begin = begin;
    edge->end = end;
    edge->id = id;
    edge->weight = weight;
    edge->color = NORMAL_EDGE;
    return edge;
}

int edgeGetId(Edge *e){
    return e->id;
}

int edgeGetWeight(Edge *e){
    return e->weight;
}

Vertex *edgeGetBegin(Edge *e){
    return e->begin;
}

Vertex *edgeGetEnd(Edge *e){
    return e->end;
}

Color edgeGetColor(Edge *e){
    return e->color;
}

void edgeSetColor(Edge *e, Color c){
    e->color = c;
}

void edgeSetId(Edge *e, int id){
    e->id = id;
}

void edgeSetWeight(Edge *e, int weight){
    e->weight = weight;
}

void edgeSetBegin(Edge *e, Vertex *begin){
    e->begin = begin;
}

void edgeSetEnd(Edge *e, Vertex *end){
    e->end = end;
}

void edgeDestroy(Edge *e){
    free(e);
}