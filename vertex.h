#ifndef H_VERTEX
#define H_VERTEX

#include "colors.h"
#include "vector2.h"

#include <gmodule.h>

typedef struct Vertex{
    int id, weight;
    Vector2 position;
    GArray *adj;
    Color color;
} Vertex;

Vertex *vertexCreate(int id, int weight, Vector2 position);

int vertexGetId(Vertex *v);
int vertexGetWeight(Vertex *v);
Vector2 vertexGetPosition(Vertex *v);
Color vertexGetColor(Vertex *v);
GArray *vertexGetAdjList(Vertex *v);

void vertexSetId(Vertex *v, int id);
void vertexSetWeight(Vertex *v, int weight);
void vertexSetPosition(Vertex *v, Vector2 position);
void vertexSetColor(Vertex *v, Color c);
void vertexSetAdjList(Vertex *v, GArray *new_adj);
void vertexAddEdge(Vertex *v, int edge_id);
void vertexRemoveEdge(Vertex *v, int edge_id);

void vertexDestroy(Vertex *v);

#endif