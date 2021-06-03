#include "vertex.h"

#include <stdlib.h>

Vertex *vertexCreate(int id, int weight, Vector2 position){
    Vertex *vertex = malloc(sizeof(Vertex));
    vertex->id = id;
    vertex->weight = weight;
    vertex->position = position;
    vertex->adj = g_array_new(FALSE, FALSE, sizeof(int));
    vertex->color = NORMAL_VERTEX;
    return vertex;
}

int vertexGetId(Vertex *v){
    return v->id;
}

int vertexGetWeight(Vertex *v){
    return v->weight;
}

Vector2 vertexGetPosition(Vertex *v){
    return v->position;
}

Color vertexGetColor(Vertex *v){
    return v->color;
}

GArray *vertexGetAdjList(Vertex *v){
    return v->adj;
}

void vertexSetId(Vertex *v, int id){
    v->id = id;
}

void vertexSetWeight(Vertex *v, int weight){
    v->weight = weight;
}

void vertexSetPosition(Vertex *v, Vector2 position){
    v->position = position;
}

void vertexSetAdjList(Vertex *v, GArray *new_adj){
    g_array_free(v->adj, TRUE);
    v->adj = new_adj;
}

void vertexSetColor(Vertex *v, Color c){
    v->color = c;
}

void vertexAddEdge(Vertex *v, int edge_id){
    g_array_append_val(v->adj, edge_id);
}

void vertexRemoveEdge(Vertex *v, int edge_id){
    for(int i=0;i<(int)v->adj->len;i++)
        if(g_array_index(v->adj, int, i) == edge_id)
            g_array_remove_index(v->adj, (unsigned int)i);
}

void vertexDestroy(gpointer data){
    Vertex *v = data;
    g_array_free(v->adj, TRUE);
    free(v);
}