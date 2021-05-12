#include "graph.h"

struct Graph createGraph(){
    struct Graph graph;
    graph.vertices = vectorCreate();
    graph.edges = vectorCreate();
    graph.vertex_id = 0;
    graph.edge_id = 0;
    return graph;
}

Vector getVertices(Graph* g){
    return g->vertices;
}

Vector getEdges(Graph* g){
    return g->edges;
}

Vertex *vertexFindById(Vector v, int id){
    for(int i=0;i<vectorSize(v);i++){
        Vertex *vertex = vectorValue(v, i);
        if(vertexGetId(vertex) == id)
            return vertex;
    }
    return NULL;
}

Edge *edgeFindById(Vector v, int id){
    for(int i=0;i<vectorSize(v);i++){
        Edge *edge = vectorValue(v, i);
        if(edgeGetId(edge) == id)
            return edge;
    }
    return NULL;
}

void addVertex(Graph *g, Point position, int weight){
    Vertex *vertex = vertexCreate(g->vertex_id, weight, position);
    g->vertex_id++;
    vectorPushBack(&g->vertices, vertex);
}

void addEdge(Graph *g, int id_begin, int id_end, int weight){
    Vertex *begin = vertexFindById(g->vertices, id_begin);
    Vertex *end = vertexFindById(g->vertices, id_end);
    if(begin && end){
        Edge *edge = edgeCreate(g->edge_id, weight, begin, end);
        vertexAddEdge(begin, edge);
        g->edge_id++;
        vectorPushBack(&g->edges, edge);
    }
}

void deleteEdge(Graph *g, Edge *e){
    Vertex *begin = edgeGetBegin(e);
    vertexRemoveEdge(begin, e);
    vectorRemove(&g->edges, e);
    free(e);
}

void deleteVertex(Graph *g, Vertex *vertex){
    for(int i = 0; i < vectorSize(g->vertices); i++){
        Vertex *v = vectorValue(g->vertices, i);
        Vector new_adj = vectorCreate();
        Vector old_adj = vertexGetAdjList(v);
        for(int j = 0; j < vectorSize(old_adj); j++){
            Edge *edge = vectorValue(old_adj, j);
            Vertex *begin = edgeGetBegin(edge);
            Vertex *end = edgeGetEnd(edge);
            if(begin != vertex && end != vertex)
                vectorPushBack(&new_adj, edge);
        }
        vectorDestroy(&old_adj);
        vertexSetAdjList(v, new_adj);
    }
    Vector new_edges = vectorCreate();
    for(int i = 0; i < vectorSize(g->edges); i++){
        Edge *edge = vectorValue(g->edges, i);
        Vertex *begin = edgeGetBegin(edge);
        Vertex *end = edgeGetEnd(edge);
        if(begin != vertex && end != vertex)
            vectorPushBack(&new_edges, edge);
        else
            edgeDestroy(edge);
    }
    vectorRemove(&g->vertices, vertex);
    vertexDestroy(vertex);
    vectorDestroy(&g->edges);
    g->edges = new_edges;
}

void destroyGraph(Graph *g){
    for(int i = 0; i < vectorSize(g->vertices); i++){
        Vertex *vertex = vectorValue(g->vertices, i);
        vertexDestroy(vertex);
    }
    vectorDestroy(&g->vertices);
    for(int i = 0; i < vectorSize(g->edges); i++){
        Edge *edge = vectorValue(g->edges, i);
        edgeDestroy(edge);
    }
    vectorDestroy(&g->edges);
}