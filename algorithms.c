#include "algorithms.h"


bool comp(void *a, void *b){
    vertexCost *x = a;
    vertexCost *y = b;
    if(x->cost == y->cost){
        if(vertexGetId(x->vertex) < vertexGetId(y->vertex))
            return true;
        return false;
    }
    return (x->cost < y->cost);
}

bool comp_bs(void *a, void *b){
    vertexCost *x = a;
    Vertex *y = b;
    return (vertexGetId(x->vertex) <= vertexGetId(y));
}

bool eq(void *a, void *b){
    vertexCost *x = a;
    Vertex *y = b;
    return (vertexGetId(x->vertex) == vertexGetId(y));
}

Algorithm createAlgorithm(Vertex *start, algorithmName algo, GUIData *data){
    Algorithm algorithm;
    if(algo == DFS || algo == BFS){
        algorithm.algo_data = NULL;
        algorithm.container = malloc(sizeof(Deque));
        *((Deque *)algorithm.container) = dequeCreate();
        dequePushBack(algorithm.container, start);
    }else if(algo == DIJKSTRA){
        algorithm.algo_data = malloc(sizeof(Vector));
        *((Vector *)algorithm.algo_data) = vectorCreate();
        algorithm.container = malloc(sizeof(Heap));
        *((Heap *)algorithm.container) = heapCreate(&comp);
        for(int i=0;i<vectorSize(getVertices(data->graph));i++){
            vertexCost *vert = malloc(sizeof(vertexCost));
            vert->vertex = vectorValue(getVertices(data->graph), i);
            if(vert->vertex == start){
                vert->cost = 0;
                heapPush(algorithm.container, vert);
            }else{
                vert->cost = INT_MAX;
            }
            vectorPushBack(algorithm.algo_data, vert);
        }
    }
    algorithm.running = true;
    algorithm.algorithm = algo;
    algorithm.current_vertex = start;
    return algorithm;
}

bool equal(Color a, Color b){
    double delta = 1e-4;
    return (fabs(a.r - b.r) < delta && fabs(a.g - b.g) < delta && fabs(a.b - b.b) < delta);
}

bool isVisited(void *ptr){
    return equal(vertexGetColor(ptr), VISITED_VERTEX);
}

bool inQueue(void *ptr){
    return equal(vertexGetColor(ptr), IN_QUEUE_VERTEX);
}

bool isActive(void *ptr){
    return equal(vertexGetColor(ptr), ACTIVE_VERTEX);
}

void resetVertices(GUIData *data){
    Vector vertices = getVertices(data->graph);
    for(int i=0;i<vectorSize(vertices);i++){
        Vertex *v = vectorValue(vertices, i);
        vertexSetColor(v, NORMAL_VERTEX);
    }
}

void endAlgorithm(GUIData *data){
    resetVertices(data);
    gtk_widget_queue_draw(data->drawing_area);
    if(data->algorithm->algorithm == DFS || data->algorithm->algorithm == BFS)
        dequeDestroy(data->algorithm->container);
    else if(data->algorithm->algorithm == DIJKSTRA){
        heapDestroy(data->algorithm->container);
        Vector *vec = data->algorithm->algo_data;
        for(int i=0;i<vectorSize(*vec);i++){
            free(vectorValue(*vec, i));
        }
        vectorDestroy(data->algorithm->algo_data);
    }
    data->algorithm->running = false;
    insertTextToTextView("Algorithm finished\n", data);
}

gboolean nextStep(gpointer data_pointer){
    GUIData *data = data_pointer;
    Algorithm *algo = data->algorithm;
    if(algo->algorithm == DFS){
        Deque *container = algo->container;
        if(dequeEmpty(*container)){
            endAlgorithm(data);
            return FALSE;
        }
        if(!isActive(algo->current_vertex)){
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
            gtk_widget_queue_draw(data->drawing_area);
            return TRUE;
        }
        Vector adj_list = vertexGetAdjList(algo->current_vertex);
        for(int i=0;i<vectorSize(adj_list);i++){
            Vertex *v = edgeGetEnd(vectorValue(adj_list, i));
            if(!isVisited(v) && !inQueue(v) && !isActive(v)){
                vertexSetColor(v, IN_QUEUE_VERTEX);
                dequePushFront(container, v);
                gtk_widget_queue_draw(data->drawing_area);
                return TRUE;
            }
        }
        vertexSetColor(algo->current_vertex, VISITED_VERTEX);
        while(!dequeEmpty(*container) && isVisited(dequeFront(*container))){
            dequePopFront(container);
        }
        if(!dequeEmpty(*container) && !isVisited(dequeFront(*container))){
            algo->current_vertex = dequeFront(*container);
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
            dequePopFront(container);
        }
    }else if(algo->algorithm == BFS){
        Deque *container = algo->container;
        if(dequeEmpty(*container)){
            endAlgorithm(data);
            return FALSE;
        }
        if(!isActive(algo->current_vertex)){
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
            gtk_widget_queue_draw(data->drawing_area);
            return TRUE;
        }
        Vector adj_list = vertexGetAdjList(algo->current_vertex);
        for(int i=0;i<vectorSize(adj_list);i++){
            Vertex *v = edgeGetEnd(vectorValue(adj_list, i));
            if(!isVisited(v) && !inQueue(v) && !isActive(v)){
                vertexSetColor(v, IN_QUEUE_VERTEX);
                dequePushFront(container, v);
                gtk_widget_queue_draw(data->drawing_area);
                return TRUE;
            }
        }
        vertexSetColor(algo->current_vertex, VISITED_VERTEX);
        dequePopBack(container);
        if(!dequeEmpty(*container)){
            algo->current_vertex = dequeBack(*container);
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
        }
    }else if(algo->algorithm == DIJKSTRA){
        Heap* heap = algo->container;
        Vector *vec = algo->algo_data;
        if(!isActive(algo->current_vertex)){
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
            gtk_widget_queue_draw(data->drawing_area);
            return TRUE;
        }
        for(int i=0;i<vectorSize(vertexGetAdjList(algo->current_vertex));i++){
            int index = vectorBinsearch(*vec, edgeGetEnd(vectorValue(vertexGetAdjList(algo->current_vertex), i)), &comp_bs, &eq);
            if(index == -1){
                endAlgorithm(data);
                return FALSE;
            }
            vertexCost *v = vectorValue(*vec, index);
            int edge_weigth = edgeGetWeight(vectorValue(vertexGetAdjList(algo->current_vertex), i));
            int cur_index = vectorBinsearch(*vec, algo->current_vertex, &comp_bs, &eq);
            if(cur_index == -1){
                endAlgorithm(data);
                return FALSE;
            }
            vertexCost *cur = vectorValue(*vec, cur_index);
            if(v->cost > edge_weigth + cur->cost){
                v->cost = edge_weigth + cur->cost;
                heapPush(heap, v);
                vertexSetColor(v->vertex, IN_QUEUE_VERTEX);
                gtk_widget_queue_draw(data->drawing_area);
                return TRUE;
            }
        }
        vertexSetColor(algo->current_vertex, VISITED_VERTEX);
        if(heapEmpty(*heap)){
            endAlgorithm(data);
            return FALSE;
        }
        while(!heapEmpty(*heap) && ((vertexCost *)heapTop(*heap))->vertex == algo->current_vertex){
            heapPop(heap);
        }
        if(!heapEmpty(*heap)){
            algo->current_vertex = ((vertexCost *)heapTop(*heap))->vertex;
            vertexSetColor(algo->current_vertex, ACTIVE_VERTEX);
            heapPop(heap);
        }
    }
    gtk_widget_queue_draw(data->drawing_area);
    return TRUE;
}
