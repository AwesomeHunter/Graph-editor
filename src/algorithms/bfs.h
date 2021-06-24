#ifndef H_BFS
#define H_BFS

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../graph.h"

typedef struct BFSData {
    Graph *graph;
    GQueue *queue;
} BFSData;

void bfsInitData(void **algorithm_data, int start_vertex, Graph *g);
bool bfsNextStep(gpointer algorithm_data);
void bfsDestroyAlgorithm(gpointer algorithm_data);

#endif