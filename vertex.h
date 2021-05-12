#ifndef H_VERTEX
#define H_VERTEX

#include "edge.h"
#include "vector.h"
#include "colors.h"
#include "point.h"

typedef struct Vertex{
    int id, weight;
    Point position;
    Vector adj_list;
    Color color;
} Vertex;

typedef struct Edge Edge;

Vertex *vertexCreate(int id, int weight, Point position);

int vertexGetId(Vertex *v);
int vertexGetWeight(Vertex *v);
Point vertexGetPosition(Vertex *v);
Color vertexGetColor(Vertex *v);
Vector vertexGetAdjList(Vertex *v);

void vertexSetId(Vertex *v, int id);
void vertexSetWeight(Vertex *v, int weight);
void vertexSetPosition(Vertex *v, Point position);
void vertexSetColor(Vertex *v, Color c);
void vertexSetAdjList(Vertex *v, Vector new_adj);
void vertexAddEdge(Vertex *v, Edge *e);
void vertexRemoveEdge(Vertex *v, Edge *e);

void vertexDestroy(Vertex *v);

#endif