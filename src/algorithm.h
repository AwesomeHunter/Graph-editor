#ifndef H_ALGORITHM
#define H_ALGORITHM

#include <gtk/gtk.h>
#include <stdbool.h>
#include "graph.h"

typedef enum AlgorithmName {
    DFS,
    BFS,
    DIJKSTRA
} AlgorithmName;

typedef struct Algorithm {
    void *algorithm_data;
    bool running;
    bool (*nextStep)(gpointer);
    void (*destroyAlgorithm)(gpointer);
} Algorithm;

void initAlgorithm(Algorithm *algorithm, AlgorithmName type, int start_vertex, Graph *g);
gboolean nextStep(gpointer user_data);

#endif