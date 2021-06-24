#ifndef H_DFS
#define H_DFS

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../graph.h"

typedef struct DFSData {
    Graph *graph;
    GQueue *stack;
} DFSData;

void dfsInitData(void **algorithm_data, int start_vertex, Graph *g);
bool dfsNextStep(gpointer algorithm_data);
void dfsDestroyAlgorithm(gpointer algorithm_data);

#endif