#include "dfs.h"
#include "utils.h"

void dfsInitData(void **algorithm_data, int start_vertex, Graph *g){
    DFSData *data = malloc(sizeof(DFSData));
    data->graph = g;
    data->stack = g_queue_new();
    g_queue_push_head(data->stack, GINT_TO_POINTER(start_vertex));
    makeActive(getVertexById(g, start_vertex));
    *algorithm_data = data;
}

bool dfsNextStep(gpointer algorithm_data){
    DFSData *data = algorithm_data;
    if(g_queue_is_empty(data->stack)){
        return false;
    }
    int v_id = GPOINTER_TO_INT(g_queue_pop_head(data->stack));
    Vertex *current_vertex = getVertexById(data->graph, v_id);
    if(!isActive(current_vertex)){
        makeActive(current_vertex);
        g_queue_push_head(data->stack, GINT_TO_POINTER(v_id));
        return true;
    }
    GArray *adj_list = vertexGetAdjList(current_vertex);
    for(int i=0;i<(int)adj_list->len;i++){
        Edge *e = getEdgeById(data->graph, g_array_index(adj_list, int, i));
        Vertex *v = getVertexById(data->graph, edgeGetEnd(e));
        if(!isVisited(v) && !inQueue(v) && !isActive(v)){
            makeInQueue(v);
            g_queue_push_head(data->stack, GINT_TO_POINTER(v_id));
            g_queue_push_head(data->stack, GINT_TO_POINTER(vertexGetId(v)));
            return true;
        }
    }
    makeVisited(current_vertex);
    return true;
}

void dfsDestroyAlgorithm(gpointer algorithm_data){
    DFSData *data = algorithm_data;
    g_queue_free(data->stack);
    free(data);
}