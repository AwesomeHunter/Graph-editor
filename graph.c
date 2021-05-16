#include "graph.h"

struct Graph createGraph(){
    struct Graph graph;
    graph.vertices = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, vertexDestroy);
    graph.edges = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, edgeDestroy);
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
    return g_hash_table_lookup(g->vertices, id);
}

Edge *getEdgeById(Graph *g, int id){
    return g_hash_table_lookup(g->edges, id);
}

void addVertex(Graph *g, Point position, int weight){
    Vertex *vertex = vertexCreate(g->vertex_id, weight, position);
    g_hash_table_insert(g->vertices, g->vertex_id, vertex);
    g->vertex_id++;
}

GHRFunc is_edge_adjacent(gpointer key, gpointer value, gpointer user_data){
    Edge *e = value;
    int vertex_id = GPOINTER_TO_INT(user_data);
    return (edgeGetBegin(e) == vertex_id || edgeGetEnd(e) == vertex_id);
}

void deleteVertex(Graph *g, int v_id){
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->vertices);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GArray *new_adj = g_array_new(FALSE, FALSE, sizeof(int));
        GArray *old_adj = vertexGetAdjList(value);
        for(int j = 0; j < old_adj->len; j++){
            int edge_index = g_array_index(old_adj, int, j);
            Edge *edge = getEdgeById(g, edge_index);
            int begin = edgeGetBegin(edge);
            int end = edgeGetEnd(edge);
            if(begin != v_id && end != v_id)
                g_array_append_val(new_adj, edge_index);
        }
        vertexSetAdjList(value, new_adj);
    }
    g_hash_table_foreach_remove(g->edges, is_edge_adjacent, GINT_TO_POINTER(v_id));
    g_hash_table_remove(g->vertices, v_id);
}

void addEdge(Graph *g, int id_begin, int id_end, int weight){
    Vertex *begin = g_hash_table_lookup(g->vertices, id_begin);
    Vertex *end = g_hash_table_lookup(g->vertices, id_end);
    if(begin && end){
        Edge *edge = edgeCreate(g->edge_id, weight, begin, end);
        g_hash_table_insert(g->edges, g->edge_id, edge);
        vertexAddEdge(begin, g->edge_id);
        g->edge_id++;
    }
}

void deleteEdge(Graph *g, int e_id){
    Edge *e = getEdgeById(g, e_id);
    Vertex *v = getVertexById(g, edgeGetBegin(e));
    vertexRemoveEdge(v, e);
    g_hash_table_remove(getEdges(g), edgeGetId(e));
}

void destroyGraph(Graph *g){
    g_hash_table_remove_all(g->vertices);
    g_hash_table_remove_all(g->edges);
    g_hash_table_destroy(g->vertices);
    g_hash_table_destroy(g->edges);
}