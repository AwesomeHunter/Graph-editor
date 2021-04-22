#include "vertex.h"

Vertex *vertexCreate(int id, int weight, Point position){
    Vertex *vertex = malloc(sizeof(Vertex));
    vertex->id = id;
    vertex->weight = weight;
    vertex->position = position;
    vertex->adj_list = vectorCreate();
    vertex->color = NORMAL_VERTEX;
    return vertex;
}

int vertexGetId(Vertex *v){
    return v->id;
}

int vertexGetWeight(Vertex *v){
    return v->weight;
}

Point vertexGetPosition(Vertex *v){
    return v->position;
}

Color vertexGetColor(Vertex *v){
    return v->color;
}

Vector vertexGetAdjList(Vertex *v){
    return v->adj_list;
}

void vertexSetId(Vertex *v, int id){
    v->id = id;
}

void vertexSetWeight(Vertex *v, int weight){
    v->weight = weight;
}

void vertexSetPosition(Vertex *v, Point position){
    v->position = position;
}

void vertexSetAdjList(Vertex *v, Vector new_adj){
    v->adj_list = new_adj;
}

void vertexSetColor(Vertex *v, Color c){
    v->color = c;
}

void vertexAddEdge(Vertex *v, Edge *e){
    vectorPushBack(&v->adj_list, e);
}

void vertexRemoveEdge(Vertex *v, Edge *e){
    vectorErase(&v->adj_list, e);
}

void vertexDestroy(Vertex *v){
    vectorDestroy(&v->adj_list);
    free(v);
}