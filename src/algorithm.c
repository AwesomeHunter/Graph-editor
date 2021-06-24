#include "algorithm.h"
#include "algorithms/dfs.h"
#include "algorithms/bfs.h"
#include "algorithms/dijkstra.h"
#include "gui.h"
#include <gtk/gtk.h>
#include "colors.h"

void initAlgorithm(Algorithm *algorithm, AlgorithmName type, int start_vertex, Graph *g){
    algorithm->running = true;
    switch (type){
    case DFS:
        dfsInitData(&algorithm->algorithm_data, start_vertex, g);
        algorithm->nextStep = &dfsNextStep;
        algorithm->destroyAlgorithm = &dfsDestroyAlgorithm;
        break;
    case BFS:
        bfsInitData(&algorithm->algorithm_data, start_vertex, g);
        algorithm->nextStep = &bfsNextStep;
        algorithm->destroyAlgorithm = &bfsDestroyAlgorithm;
        break;
    case DIJKSTRA:
        dijkstraInitData(&algorithm->algorithm_data, start_vertex, g);
        algorithm->nextStep = &dijkstraNextStep;
        algorithm->destroyAlgorithm = &dijkstraDestroyAlgorithm;
        break;
    default:
        algorithm->running = false;
        break;
    }
}

void resetVertices(gpointer user_data){
    GUIData *data = user_data;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, getVertices(data->graph));
    while(g_hash_table_iter_next (&iter, &key, &value)){
        vertexSetColor(value, NORMAL_VERTEX);
    }
}

void destroyAlgorithm(gpointer user_data){
    GUIData *data = user_data;
    data->algorithm.destroyAlgorithm(data->algorithm.algorithm_data);
    data->algorithm.running = false;
    data->algorithm.algorithm_data = NULL;
    data->algorithm.nextStep = NULL;
    data->algorithm.destroyAlgorithm = NULL;
    resetVertices(user_data);
}

gboolean nextStep(gpointer user_data){
    GUIData *data = user_data;
    gboolean result = data->algorithm.nextStep(data->algorithm.algorithm_data);
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
    if(!result){
        destroyAlgorithm(user_data);
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}