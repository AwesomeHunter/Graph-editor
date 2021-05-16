#ifndef H_GRAPH
#define H_GRAPH

#include <gmodule.h>

#include "edge.h"
#include "vertex.h"
#include "point.h"

typedef struct Graph{
    GHashTable *vertices;
    GHashTable *edges;
    int vertex_id, edge_id;
} Graph;

Graph createGraph();
GHashTable *getVertices(Graph* g);
GHashTable *getEdges(Graph* g);
Vertex *getVertexById(Graph *g, int id);
Edge *getEdgeById(Graph *g, int id);
void addVertex(Graph *g, Point position, int weight);
void deleteVertex(Graph *g, int v_id);
void addEdge(Graph *g, int begin_id, int end_id, int weight);
void deleteEdge(Graph *g, int e_id);
void destroyGraph(Graph *g);

#endif