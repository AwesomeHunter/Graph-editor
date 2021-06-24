#ifndef H_DIJKSTRA
#define H_DIJKSTRA

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../graph.h"

typedef struct DIJKSTRAData {
    Graph *graph;
    GHashTable *costs;
    GArray *queue;
} DIJKSTRAData;

void dijkstraInitData(void **algorithm_data, int start_vertex, Graph *g);
bool dijkstraNextStep(gpointer algorithm_data);
void dijkstraDestroyAlgorithm(gpointer algorithm_data);

#endif