#ifndef H_GRAPH
#define H_GRAPH

#include <gmodule.h>

#include "edge.h"
#include "vertex.h"
#include "vector2.h"

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
int addVertex(Graph *g, Vector2 position, int weight);
gboolean isEdgeAdjacent(gpointer key, gpointer value, gpointer user_data);
void deleteVertex(Graph *g, int v_id);
int addEdge(Graph *g, int id_begin, int id_end, int weight);
void deleteEdge(Graph *g, int e_id);
void destroyGraph(Graph *g);

#endif