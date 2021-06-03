#include "graph.h"

Graph createGraph(){
    Graph graph;
    graph.vertices = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, vertexDestroy);
    graph.edges = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, edgeDestroy);
    graph.vertex_id = 0;
    graph.edge_id = 0;
    return graph;
}

GHashTable *getVertices(Graph* g){
    return g->vertices;
}

GHashTable *getEdges(Graph* g){
    return g->edges;
}

Vertex *getVertexById(Graph *g, int id){
    return g_hash_table_lookup(g->vertices, GINT_TO_POINTER(id));
}

Edge *getEdgeById(Graph *g, int id){
    return g_hash_table_lookup(g->edges, GINT_TO_POINTER(id));
}

int addVertex(Graph *g, Vector2 position, int weight){
    Vertex *vertex = vertexCreate(g->vertex_id, weight, position);
    g_hash_table_insert(g->vertices, GINT_TO_POINTER(g->vertex_id), vertex);
    g->vertex_id++;
    return g->vertex_id-1;
}

gboolean isEdgeAdjacent(gpointer key, gpointer value, gpointer user_data){
    Edge *e = value;
    int vertex_id = GPOINTER_TO_INT(user_data);
    (void)key;
    return (edgeGetBegin(e) == vertex_id || edgeGetEnd(e) == vertex_id);
}

void deleteVertex(Graph *g, int v_id){
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->vertices);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GArray *new_adj = g_array_new(FALSE, FALSE, sizeof(int));
        GArray *old_adj = vertexGetAdjList(value);
        for(int j = 0; j < (int)old_adj->len; j++){
            int edge_index = g_array_index(old_adj, int, j);
            Edge *edge = getEdgeById(g, edge_index);
            int begin = edgeGetBegin(edge);
            int end = edgeGetEnd(edge);
            if(begin != v_id && end != v_id)
                g_array_append_val(new_adj, edge_index);
        }
        vertexSetAdjList(value, new_adj);
    }
    g_hash_table_foreach_remove(g->edges, isEdgeAdjacent, GINT_TO_POINTER(v_id));
    g_hash_table_remove(g->vertices, GINT_TO_POINTER(v_id));
}

int addEdge(Graph *g, int id_begin, int id_end, int weight){
    Vertex *begin = g_hash_table_lookup(g->vertices, GINT_TO_POINTER(id_begin));
    Vertex *end = g_hash_table_lookup(g->vertices, GINT_TO_POINTER(id_end));
    if(begin && end){
        Edge *edge = edgeCreate(g->edge_id, weight, vertexGetId(begin), vertexGetId(end));
        g_hash_table_insert(g->edges, GINT_TO_POINTER(g->edge_id), edge);
        vertexAddEdge(begin, g->edge_id);
        g->edge_id++;
        return g->edge_id-1;
    }
    return -1;
}

void deleteEdge(Graph *g, int e_id){
    Edge *e = getEdgeById(g, e_id);
    int edge_id = edgeGetId(e);
    Vertex *v = getVertexById(g, edgeGetBegin(e));
    vertexRemoveEdge(v, edge_id);
    g_hash_table_remove(getEdges(g), GINT_TO_POINTER(edge_id));
}

void destroyGraph(Graph *g){
    g_hash_table_destroy(g->vertices);
    g_hash_table_destroy(g->edges);
}