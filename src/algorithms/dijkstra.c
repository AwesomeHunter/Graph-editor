#include "dijkstra.h"
#include "utils.h"

void setAllCostsToInfinity(GHashTable *costs, Graph *g){
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, getVertices(g));
    while(g_hash_table_iter_next(&iter, &key, &value))
        g_hash_table_insert(costs, key, GINT_TO_POINTER(INT32_MAX));
}

void dijkstraInitData(void **algorithm_data, int start_vertex, Graph *g){
    DIJKSTRAData *data = malloc(sizeof(DIJKSTRAData));
    data->graph = g;
    data->costs = g_hash_table_new(g_direct_hash, g_direct_equal);
    data->queue = g_array_new(FALSE, FALSE, sizeof(int));
    setAllCostsToInfinity(data->costs, g);
    g_hash_table_replace(data->costs, GINT_TO_POINTER(start_vertex), GINT_TO_POINTER(0));
    g_array_append_val(data->queue, start_vertex);
    makeActive(getVertexById(g, start_vertex));
    *algorithm_data = data;
}

int getVertexWithMinCost(gpointer algorithm_data){
    DIJKSTRAData *data = algorithm_data;
    int vertex_id = -1;
    int min_cost = INT32_MAX;
    for(int i=0;i<(int)data->queue->len;i++){
        int id = g_array_index(data->queue, int, i);
        int cost = GPOINTER_TO_INT(g_hash_table_lookup(data->costs, GINT_TO_POINTER(id)));
        if(cost < min_cost){
            vertex_id = id;
            min_cost = cost;
        }
    }
    return vertex_id;
}

void deleteIfInQueue(gpointer algorithm_data, int id){
    DIJKSTRAData *data = algorithm_data;
    for(int i=0;i<(int)data->queue->len;i++){
        if(g_array_index(data->queue, int, i) == id){
            g_array_remove_index(data->queue, (unsigned int)i);
            return;
        }
    }
}

bool dijkstraNextStep(gpointer algorithm_data){
    DIJKSTRAData *data = algorithm_data;
    if(data->queue->len == 0)
        return false;
    int current_id = getVertexWithMinCost(algorithm_data);
    Vertex *current_vertex = getVertexById(data->graph, current_id);
    if(!isActive(current_vertex)){
        makeActive(current_vertex);
        return true;
    }
    int current_cost = GPOINTER_TO_INT(g_hash_table_lookup(data->costs, GINT_TO_POINTER(current_id)));
    GArray *adj_list = vertexGetAdjList(current_vertex);
    for(int i=0;i<(int)adj_list->len;i++){
        Edge *e = getEdgeById(data->graph, g_array_index(adj_list, int, i));
        int v_id = edgeGetEnd(e);
        Vertex *v = getVertexById(data->graph, v_id);
        int new_cost = current_cost + edgeGetWeight(e);
        if(new_cost < GPOINTER_TO_INT(g_hash_table_lookup(data->costs, GINT_TO_POINTER(v_id)))){
            makeInQueue(v);
            deleteIfInQueue(algorithm_data, v_id);
            g_array_append_val(data->queue, v_id);
            g_hash_table_insert(data->costs, GINT_TO_POINTER(v_id), GINT_TO_POINTER(new_cost));
            return true;
        }
    }
    deleteIfInQueue(algorithm_data, current_id);
    makeVisited(current_vertex);
    return true;
}

void dijkstraDestroyAlgorithm(gpointer algorithm_data){
    DIJKSTRAData *data = algorithm_data;
    g_hash_table_destroy(data->costs);
    g_array_free(data->queue, TRUE);
    free(data);
}