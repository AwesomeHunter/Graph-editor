#ifndef H_ALGO
#define H_ALGO

#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "vector.h"
#include "deque.h"
#include "vertex.h"
#include "gui.h"
#include "heap.h"

typedef enum algorithmName {
    DFS, BFS, DIJKSTRA
} algorithmName;

typedef struct Algorithm {
    void *container;
    algorithmName algorithm;
    Vertex *current_vertex;
    void *algo_data;
    bool running;
} Algorithm;

typedef struct vertexCost {
    Vertex *vertex;
    int cost;
} vertexCost;
typedef struct GUIData GUIData;
bool comp(void *a, void *b);
bool comp_bs(void *a, void *b);
bool eq(void *a, void *b);
Algorithm createAlgorithm(Vertex *start, algorithmName algo, GUIData *data);
bool equal(Color a, Color b);
bool isVisited(void *ptr);
bool inQueue(void *ptr);
bool isActive(void *ptr);
void resetVertices(GUIData *data);
void endAlgorithm(GUIData *data);
gboolean nextStep(gpointer data_pointer);


#endif