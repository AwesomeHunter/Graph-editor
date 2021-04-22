#ifndef H_GRAPH
#define H_GRAPH

#include <stdio.h>

#include "edge.h"
#include "vertex.h"
#include "vector.h"
#include "point.h"

typedef struct Graph{
    Vector vertices;
    Vector edges;
    int vertex_id, edge_id;
} Graph;

Graph createGraph();
Vector getVertices(Graph* g);
Vector getEdges(Graph* g);
Vertex *vertexFindById(Vector v, int id);
Edge *edgeFindById(Vector v, int id);
void addVertex(Graph *g, Point position, int weight);
void deleteVertex(Graph *g, Vertex *v);
void addEdge(Graph *g, int id_begin, int id_end, int weight);
void deleteEdge(Graph *g, Edge *e);
void destroyGraph(Graph *g);

#endif