#include "gui.h"

#define VERTEX_RADIUS 10

void closeWindow(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    destroyGraph(data->graph);
    gtk_text_buffer_delete_mark(gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view)), data->text_mark_end);
    for(int i=0;i<vectorSize(data->widgets);i++){
        gtk_widget_destroy(vectorValue(data->widgets, i));
    }
    vectorDestroy(&data->widgets);
    vectorDestroy(&data->active_vertices);
    vectorDestroy(&data->active_edges);
    pango_cairo_font_map_set_default(NULL);
    cairo_debug_reset_static_data();
    gtk_main_quit();
    (void)widget;
}

bool isNumber(const gchar *number){
    if(strcmp(number, "")==0)
        return false;
    for(int i=0;i<(int)strlen(number);i++){
        if(number[i]<'0' || number[i]>'9')
            return false;
    }
    return true;
}

void sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer data_pointer){
    GUIData *data = data_pointer;
    Point new_drawing_size = {allocation->width, allocation->height};
    Vector vertices = getVertices(data->graph);
    for(int i=0; i<vectorSize(vertices); i++){
        Vertex *v = vectorValue(vertices, i);
        Point new_position = multPoint(divPoint(new_drawing_size, data->prev_drawing_area_size), vertexGetPosition(v));
        vertexSetPosition(v, new_position);
    }
    data->prev_drawing_area_size = new_drawing_size;
    gtk_widget_queue_draw(data->drawing_area);
    (void)widget;
}

GtkWidget *createWindow(GUIData* data){
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
    gtk_window_set_title(GTK_WINDOW(window), "Graphs");
    gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(closeWindow), data);
    return window;
}

void insertTextToTextView(const gchar *text, GUIData *data){
    GtkTextIter iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data->text_view), data->text_mark_end, 0, FALSE, 0, 0);
}

GtkWidget *createTextView(){
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD); 
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    return text_view;
}

GtkTextMark *createTextMark(GtkWidget *text_view){
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter text_iter_end;
    gtk_text_buffer_get_end_iter(text_buffer, &text_iter_end);
    GtkTextMark *text_mark_end = gtk_text_buffer_create_mark (text_buffer, NULL, &text_iter_end, FALSE);
    return text_mark_end;
}

GtkWidget *createScrolledTextView(GtkWidget *text_view){
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    return scrolled_window;
}

GtkWidget *createTextEntry(int max_len){
    GtkWidget *text_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(text_entry), max_len);
    return text_entry;
}

void clearTextViewSignal(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
    gtk_text_buffer_set_text(buffer, "", 0);
    (void)widget;
}

void displayEdgeInfo(Edge *edge, const char *action, GUIData *data){
    Vertex *begin = edgeGetBegin(edge);
    Vertex *end = edgeGetEnd(edge);
    char temp[50];
    sprintf(temp, "> EDGE #%d  from: %d | to: %d | weight: %d\n", edgeGetId(edge), vertexGetId(begin), vertexGetId(end), edgeGetWeight(edge));
    insertTextToTextView(action, data);
    insertTextToTextView(temp, data);
}

void displayVertexInfo(Vertex *vertex, const char *action, GUIData *data){
    char temp[50];
    sprintf(temp, "> VERTEX #%d weight: %d\n", vertexGetId(vertex), vertexGetWeight(vertex));
    insertTextToTextView(action, data);
    insertTextToTextView(temp, data);
    (void)vertex, (void)action, (void)data;
}

void deselectEverything(GUIData *data){
    while(!vectorEmpty(data->active_edges)){
        edgeSetColor(vectorBack(data->active_edges), NORMAL_EDGE);
        vectorPopBack(&data->active_edges);
    }
    while(!vectorEmpty(data->active_vertices)){
        vertexSetColor(vectorBack(data->active_vertices), NORMAL_VERTEX);
        vectorPopBack(&data->active_vertices);
    }
}

Vertex *tryToSelectVertex(Point mouse, GUIData *data){
    Vector vertices = getVertices(data->graph);
    for(int i = vectorSize(vertices) - 1; i >= 0;i--){
        Vertex *v = vectorValue(vertices, i);
        if(dist(vertexGetPosition(v), mouse) <= VERTEX_RADIUS){
            return v;
        }
    }
    return NULL;
}

Edge *tryToSelectEdge(Point mouse, GUIData *data){
    Vector edges = getEdges(data->graph);
    for(int i = vectorSize(edges) - 1; i >= 0; i--){
        Edge *e = vectorValue(edges, i);
        Vertex *begin = edgeGetBegin(e);
        Vertex *end = edgeGetEnd(e);
        if(begin == end){
            if(edgeLoopIntersect(vertexGetPosition(begin), mouse, 5.0)){
                return e;
            }
        }else{
            if(rectIntersect(vertexGetPosition(begin), vertexGetPosition(end), mouse, 5.0)){
                return e;
            }
        }
    }
    return NULL;
}

void selectEdge(Edge *edge, GUIData *data){
    if(data->cur_mode != SELECT_MOVE_MODE){
        deselectEverything(data);
    }
    vectorPushBack(&data->active_edges, edge);
    edgeSetColor(edge, SELECTED_EDGE);
    displayEdgeInfo(edge, "Edge selected\n", data);
}

void selectVertex(Vertex *vertex, GUIData *data){
    if(data->cur_mode != SELECT_MOVE_MODE){
        deselectEverything(data);
    }
    vectorPushBack(&data->active_vertices, vertex);
    vertexSetColor(vertex, SELECTED_VERTEX);
    displayVertexInfo(vertex, "Vertex selected\n", data);
}

void clickedDrawingAreaSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    GdkEventMotion *e = (GdkEventMotion *)event;
    if(event->button != GDK_BUTTON_PRIMARY || event->type == GDK_2BUTTON_PRESS)
        return;
    Point mouse = {e->x, e->y};
    Vertex *vertex = tryToSelectVertex(mouse, data);
    Edge *edge = tryToSelectEdge(mouse, data);
    switch (data->cur_mode){
    case ADD_VERTEX_MODE:
        if(vertex == NULL && edge == NULL){
            addVertex(data->graph, mouse, 1);
            displayVertexInfo(vectorBack(getVertices(data->graph)), "Vertex added\n", data);
        }
        break;
    case ADD_EDGE_MODE:
        if(vertex){
            if(!vectorEmpty(data->active_vertices)){
                addEdge(data->graph, vertexGetId(vectorBack(data->active_vertices)), vertexGetId(vertex), 1);
                displayEdgeInfo(vectorBack(getEdges(data->graph)), "Edge added\n", data);
                deselectEverything(data);
            }else{
                selectVertex(vertex, data);
            }
        }
        break;
    case SELECT_MOVE_MODE:
        if(vertex){
            bool found = false;
            for(int i=0;i<vectorSize(data->active_vertices);i++){
                if(vectorValue(data->active_vertices, i) == vertex){
                    found = true;
                    break;
                }
            }
            if(found){
                vertexSetColor(vertex, NORMAL_VERTEX);
                vectorErase(&data->active_vertices, vertex);
            }else{
                data->mouse_pressed = true;
                data->delta_mouse_pos = subtrPoint(vertexGetPosition(vertex), mouse);
                selectVertex(vertex, data);
            }
        }else if(edge){
                bool found = false;
                for(int i=0;i<vectorSize(data->active_edges);i++){
                    if(vectorValue(data->active_edges, i) == edge){
                        found = true;
                        break;
                    }
                }
                if(found){
                    edgeSetColor(edge, NORMAL_EDGE);
                    vectorErase(&data->active_edges, edge);
                }else{
                    selectEdge(edge, data);
                }
        }
        break;
    case DELETE_MODE:
        if(vertex){
            displayVertexInfo(vertex, "Vertex deleted\n", data);
            deleteVertex(data->graph, vertex);
        }else if(edge){
            displayEdgeInfo(edge, "Edge deleted\n", data);
            deleteEdge(data->graph, edge);
        }
        break;
    default:
        break;
    }
    gtk_widget_queue_draw(data->drawing_area);
    (void)widget;
}

void buttonReleasedSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer){
    GUIData *data = data_pointer;
    data->mouse_pressed = false;
    (void)widget, (void)event;
}

void mouseMovedSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer){
    GUIData *data = data_pointer;
    double width = gtk_widget_get_allocated_width(data->drawing_area);
    double height = gtk_widget_get_allocated_height(data->drawing_area);
    if(data->mouse_pressed && !vectorEmpty(data->active_vertices)){
        GdkEventMotion *e = (GdkEventMotion *)event;
        Point mouse = {e->x, e->y};
        Point new_vertex_pos = addPoint(mouse, data->delta_mouse_pos);
        new_vertex_pos.x = fmax(fmin(new_vertex_pos.x, width), 0.0);
        new_vertex_pos.y = fmax(fmin(new_vertex_pos.y, height), 0.0);
        vertexSetPosition(vectorBack(data->active_vertices), new_vertex_pos);
        gtk_widget_queue_draw(data->drawing_area);
    }
    (void)widget;
}

void selectVertexSignal(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(data->vertex_id_entry));
    if(isNumber(text)){
        int id = atoi(text);
        Vector vertices = getVertices(data->graph);
        Vertex *vertex = vertexFindById(vertices, id);
        if(data->cur_mode != SELECT_MOVE_MODE){
            deselectEverything(data);
            if(vertex){
                selectVertex(vertex, data);
            }
        }else{
            bool found = false;
            for(int i=0;i<vectorSize(data->active_vertices);i++){
                if(vectorValue(data->active_vertices, i) == vertex){
                    found = true;
                    break;
                }
            }
            if(found){
                vertexSetColor(vertex, NORMAL_VERTEX);
                vectorErase(&data->active_vertices, vertex);
            }else{
                if(vertex){
                    selectVertex(vertex, data);
                }
            }
        }
        gtk_widget_queue_draw(data->drawing_area);
    }
    (void)widget;
}

void selectEdgeSignal(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(data->edge_id_entry));
    if(isNumber(text)){
        int id = atoi(text);
        Vector edges = getEdges(data->graph);
        Edge *edge = edgeFindById(edges, id);
        if(data->cur_mode != SELECT_MOVE_MODE){
            deselectEverything(data);
            if(edge){
                selectEdge(edge, data);
            }
        }else{
            bool found = false;
            for(int i=0;i<vectorSize(data->active_edges);i++){
                if(vectorValue(data->active_edges, i) == edge){
                    found = true;
                    break;
                }
            }
            if(found){
                edgeSetColor(edge, NORMAL_EDGE);
                vectorErase(&data->active_edges, edge);
            }else{
                if(edge){
                    selectEdge(edge, data);
                }
            }
        }
        gtk_widget_queue_draw(data->drawing_area);
    }
    (void)widget;
}

void deleteSelectedSignal(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    while(!vectorEmpty(data->active_edges)){
        Edge *edge = vectorBack(data->active_edges);
        displayEdgeInfo(edge, "Edge deleted\n", data);
        vectorPopBack(&data->active_edges);
        deleteEdge(data->graph, edge);
    }
    while(!vectorEmpty(data->active_vertices)){
        Vertex *vertex = vectorBack(data->active_vertices);
        displayVertexInfo(vertex, "Vertex deleted\n", data);
        vectorPopBack(&data->active_vertices);
        deleteVertex(data->graph, vertex);
    }
    gtk_widget_queue_draw(data->drawing_area);
    (void)widget;
}

void setWeightSignal(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(data->weight_entry));
    if(isNumber(text)){
        int weight = atoi(text);
        if(weight == 0)
            return;
        for(int i=0;i<vectorSize(data->active_edges);i++){
            edgeSetWeight(vectorValue(data->active_edges, i), weight);
            displayEdgeInfo(vectorValue(data->active_edges, i), "Edge weight changed\n", data);
        }
        for(int i=0;i<vectorSize(data->active_vertices);i++){
            vertexSetWeight(vectorValue(data->active_vertices, i), weight);
            displayVertexInfo(vectorValue(data->active_vertices, i), "Vertex weight changed\n", data);
        }
        gtk_widget_queue_draw(data->drawing_area);
    }
    (void)widget;
}

void changeMode(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    deselectEverything(data);
    gchar *mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->mode_combo));
    if(strcmp(mode, "select mode") == 0){
        data->cur_mode = SELECT_MOVE_MODE;
        insertTextToTextView("Current mode: select mode\n", data);
    }else if(strcmp(mode, "add vertex mode") == 0){
        data->cur_mode = ADD_VERTEX_MODE;
        insertTextToTextView("Current mode: add vertex mode\n", data);
    }else if(strcmp(mode, "add edge mode") == 0){
        data->cur_mode = ADD_EDGE_MODE;
        insertTextToTextView("Current mode: add edge mode\n", data);
    }else if(strcmp(mode, "delete mode") == 0){
        data->cur_mode = DELETE_MODE;
        insertTextToTextView("Current mode: delete mode\n", data);
    }
    free(mode);
    gtk_widget_queue_draw(data->drawing_area);
    (void)widget;
}

void loadGraph(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(data->load_entry)), "")==0)
        return;
    char path[50] = "graphs/";
    strcat(path, gtk_entry_get_text(GTK_ENTRY(data->load_entry)));
    FILE *file = fopen(path, "r");
    if(file){
        deselectEverything(data);
        char buffer[255];
        Graph new_graph = createGraph();
        while(fgets(buffer, 255, file)) {
            char *ptr = strtok(buffer, " ");
            if(strcmp(ptr, "e") == 0){
                ptr = strtok(NULL, " ");
                int begin = atoi(ptr) - 1;
                ptr = strtok(NULL, " ");
                int end = atoi(ptr) - 1;
                addEdge(&new_graph, begin, end, 1);
            }else if(strcmp(ptr, "p") == 0){
                ptr = strtok(NULL, " ");
                ptr = strtok(NULL, " ");
                int number_of_vertices = atoi(ptr);
                double radius = fmin(data->prev_drawing_area_size.x, data->prev_drawing_area_size.y) / 2.0 - 2.0 * VERTEX_RADIUS;
                Point circle_mid = divScalar(data->prev_drawing_area_size, 2.0);
                double angle_change = 2.0 * G_PI / (double)number_of_vertices;
                for(int i=0; i<number_of_vertices; i++){
                    Point delta_pos = {radius*cos(i*angle_change), radius*sin(i*angle_change)};
                    addVertex(&new_graph, addPoint(circle_mid, delta_pos), 1);
                }
            }
        }
        destroyGraph(data->graph);
        *data->graph = new_graph;
        fclose(file);
        char path_coords[50] = "graphs/coords_";
        strcat(path_coords, gtk_entry_get_text(GTK_ENTRY(data->load_entry)));
        FILE *file_coords = fopen(path_coords, "r");
        if(file_coords){
            char buffer2[255];
            Vector vertices = getVertices(data->graph);
            while(fgets(buffer2, 255, file_coords)) {
                Point pos;
                char *ptr = strtok(buffer2, " ");
                int id = atoi(ptr) - 1;
                ptr = strtok(NULL, " ");
                pos.x = atof(ptr) * data->prev_drawing_area_size.x;
                ptr = strtok(NULL, " ");
                pos.y = atof(ptr) * data->prev_drawing_area_size.y;
                Vertex *vertex = vertexFindById(vertices, id);
                vertexSetPosition(vertex, pos);
            }
            fclose(file_coords);
        }
        insertTextToTextView("Graph \"", data);
        insertTextToTextView(gtk_entry_get_text(GTK_ENTRY(data->load_entry)), data);
        insertTextToTextView("\" loaded\n", data);
        gtk_widget_queue_draw(data->drawing_area);
    }
    (void)widget;
}

void saveGraph(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    if(strcmp(gtk_entry_get_text(GTK_ENTRY(data->save_entry)), "")==0)
        return;
    deselectEverything(data);
    char path[50] = "graphs/";
    strcat(path, gtk_entry_get_text(GTK_ENTRY(data->save_entry)));
    FILE *file = fopen(path, "w");
    Vector vertices = getVertices(data->graph);
    if(file){
        for(int i=0;i<vectorSize(vertices);i++){
            vertexSetId(vectorValue(vertices, i), i);
        }
        Vector edges = getEdges(data->graph);
        fprintf(file, "p edge %d %d\n", vectorSize(vertices), vectorSize(edges));
        for(int i=0;i<vectorSize(edges);i++){
            Edge *edge = vectorValue(edges, i);
            int begin_id = vertexGetId(edgeGetBegin(edge)) + 1;
            int end_id = vertexGetId(edgeGetEnd(edge)) + 1;
            fprintf(file, "e %d %d\n", begin_id, end_id);
        }
        fclose(file);
        char path_coords[50] = "graphs/coords_";
        strcat(path_coords, gtk_entry_get_text(GTK_ENTRY(data->save_entry)));
        FILE *file_coords = fopen(path_coords, "w");
        if(file_coords){
            for(int i=0;i<vectorSize(vertices);i++){
                Vertex *vertex = vectorValue(vertices, i);
                int id = vertexGetId(vertex) + 1;
                Point pos = divPoint(vertexGetPosition(vertex), data->prev_drawing_area_size);
                fprintf(file_coords, "%d %f %f\n", id, pos.x, pos.y);
            }
            fclose(file_coords);
        }
        insertTextToTextView("Graph \"", data);
        insertTextToTextView(gtk_entry_get_text(GTK_ENTRY(data->save_entry)), data);
        insertTextToTextView("\" saved\n", data);
    }
    (void)widget;
}

void runAlgorithm(GtkWidget *widget, gpointer data_pointer){
    GUIData *data = data_pointer;
    if(data->algorithm->running)
        return;
    gchar *algoName = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->algorithms_combo));
    if(!vectorEmpty(data->active_edges) || vectorSize(data->active_vertices) > 1){
        insertTextToTextView("Only one vertex can be selected to run algorithm\n", data);
        return;
    }
    if(vectorSize(data->active_vertices) < 1){
        insertTextToTextView("Select vertex\n", data);
        return;
    }
    Vertex *start = vectorBack(data->active_vertices);
    deselectEverything(data);
    if(strcmp(algoName, "DFS") == 0){
        insertTextToTextView("Algorithm is running: DFS\n", data);
        Algorithm algo = createAlgorithm(start, DFS, data);
        *data->algorithm = algo;
        g_timeout_add(500, nextStep, data);
    }else if(strcmp(algoName, "BFS") == 0){
        insertTextToTextView("Algorithm is running: BFS\n", data);
        Algorithm algo = createAlgorithm(start, BFS, data);
        *data->algorithm = algo;
        g_timeout_add(500, nextStep, data);
    }else if(strcmp(algoName, "DIJKSTRA") == 0){
        insertTextToTextView("Algorithm is running: DIJKSTRA\n", data);
        Algorithm algo = createAlgorithm(start, DIJKSTRA, data);
        *data->algorithm = algo;
        g_timeout_add(500, nextStep, data);
    }
    free(algoName);
    (void)widget;
}

GtkWidget *createRightBox(GUIData *data){
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *select_vertex_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *select_edge_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *set_weight_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *load_graph_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *save_graph_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *algorithm_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *mode_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *button_select_vertex = gtk_button_new_with_label("select vertex");
    GtkWidget *button_select_edge = gtk_button_new_with_label("select edge");
    GtkWidget *button_delete_selected = gtk_button_new_with_label("delete selected");
    GtkWidget *button_set_weight = gtk_button_new_with_label("set weight");
    GtkWidget *button_clear_text_view = gtk_button_new_with_label("clear console");
    GtkWidget *button_load_graph = gtk_button_new_with_label("load graph");
    GtkWidget *button_save_graph = gtk_button_new_with_label("save graph");
    GtkWidget *button_algorithms = gtk_button_new_with_label("run algorithm");

    GtkWidget *vertex_id_entry = createTextEntry(4);
    GtkWidget *edge_id_entry = createTextEntry(4);
    GtkWidget *weight_entry = createTextEntry(7);
    GtkWidget *load_graph_entry = createTextEntry(10);
    GtkWidget *save_graph_entry = createTextEntry(10);

    GtkWidget *text_view = createTextView();
    GtkTextMark *text_mark = createTextMark(text_view);
    GtkWidget *scrolled_text_view = createScrolledTextView(text_view);

    GtkWidget *algorithms_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithms_combo), "DFS");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithms_combo), "BFS");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithms_combo), "DIJKSTRA");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithms_combo), 0);
    
    GtkWidget *mode_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mode_combo), "select mode");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mode_combo), "add vertex mode");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mode_combo), "add edge mode");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(mode_combo), "delete mode");
    gtk_combo_box_set_active(GTK_COMBO_BOX(mode_combo), 0);

    data->text_mark_end = text_mark;
    data->text_view = text_view;
    data->vertex_id_entry = vertex_id_entry;
    data->edge_id_entry = edge_id_entry;
    data->weight_entry = weight_entry;
    data->load_entry = load_graph_entry;
    data->save_entry = save_graph_entry;
    data->algorithms_combo = algorithms_combo;
    data->mode_combo = mode_combo;

    g_signal_connect(G_OBJECT(button_select_vertex), "clicked", G_CALLBACK(selectVertexSignal), data);
    g_signal_connect(G_OBJECT(button_select_edge), "clicked", G_CALLBACK(selectEdgeSignal), data);
    g_signal_connect(G_OBJECT(button_delete_selected), "clicked", G_CALLBACK(deleteSelectedSignal), data);
    g_signal_connect(G_OBJECT(button_set_weight), "clicked", G_CALLBACK(setWeightSignal), data);
    g_signal_connect(G_OBJECT(button_clear_text_view), "clicked", G_CALLBACK(clearTextViewSignal), data);
    g_signal_connect(G_OBJECT(button_load_graph), "clicked", G_CALLBACK(loadGraph), data);
    g_signal_connect(G_OBJECT(button_save_graph), "clicked", G_CALLBACK(saveGraph), data);
    g_signal_connect(G_OBJECT(button_algorithms), "clicked", G_CALLBACK(runAlgorithm), data);

    g_signal_connect(G_OBJECT(mode_combo), "changed", G_CALLBACK(changeMode), data);

    gtk_box_pack_start(GTK_BOX(select_vertex_box), vertex_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(select_vertex_box), button_select_vertex, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(select_edge_box), edge_id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(select_edge_box), button_select_edge, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(set_weight_box), weight_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(set_weight_box), button_set_weight, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(load_graph_box), load_graph_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(load_graph_box), button_load_graph, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(save_graph_box), save_graph_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(save_graph_box), button_save_graph, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(algorithm_box), algorithms_combo, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(algorithm_box), button_algorithms, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(mode_box), mode_combo, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(right_box), select_vertex_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), select_edge_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), button_delete_selected, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), set_weight_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), load_graph_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), save_graph_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), algorithm_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), button_clear_text_view, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), mode_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), scrolled_text_view, TRUE, TRUE, 0);

    vectorPushBack(&data->widgets, button_select_vertex);
    vectorPushBack(&data->widgets, button_select_edge);
    vectorPushBack(&data->widgets, button_delete_selected);
    vectorPushBack(&data->widgets, button_set_weight);
    vectorPushBack(&data->widgets, button_clear_text_view);
    vectorPushBack(&data->widgets, button_load_graph);
    vectorPushBack(&data->widgets, button_save_graph);
    vectorPushBack(&data->widgets, button_algorithms);
    vectorPushBack(&data->widgets, vertex_id_entry);
    vectorPushBack(&data->widgets, edge_id_entry);
    vectorPushBack(&data->widgets, weight_entry);
    vectorPushBack(&data->widgets, load_graph_entry);
    vectorPushBack(&data->widgets, save_graph_entry);
    vectorPushBack(&data->widgets, text_view);
    vectorPushBack(&data->widgets, scrolled_text_view);
    vectorPushBack(&data->widgets, mode_combo);
    vectorPushBack(&data->widgets, algorithms_combo);
    vectorPushBack(&data->widgets, algorithm_box);
    vectorPushBack(&data->widgets, mode_box);
    vectorPushBack(&data->widgets, select_vertex_box);
    vectorPushBack(&data->widgets, select_edge_box);
    vectorPushBack(&data->widgets, set_weight_box);
    vectorPushBack(&data->widgets, load_graph_box);
    vectorPushBack(&data->widgets, save_graph_box);
    vectorPushBack(&data->widgets, right_box);

    return right_box;
}

GtkWidget *createLeftBox(GUIData *data){
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *drawing_area = gtk_drawing_area_new();

    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(drawing_area, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawing_area, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(drawAll), data);
    g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(clickedDrawingAreaSignal), data);
    g_signal_connect(G_OBJECT(drawing_area), "button-release-event", G_CALLBACK(buttonReleasedSignal), data);
    g_signal_connect(G_OBJECT(drawing_area), "motion-notify-event", G_CALLBACK(mouseMovedSignal), data);
    g_signal_connect(G_OBJECT(drawing_area), "size-allocate", G_CALLBACK(sizeChanged), data);
    data->drawing_area = drawing_area;
    data->prev_drawing_area_size.x = gtk_widget_get_allocated_width(drawing_area);
    data->prev_drawing_area_size.y = gtk_widget_get_allocated_height(drawing_area);
    
    gtk_box_pack_start(GTK_BOX(left_box), drawing_area, TRUE, TRUE, 0);
    vectorPushBack(&data->widgets, drawing_area);
    vectorPushBack(&data->widgets, left_box);
    
    return left_box;
}

GtkWidget *createMainBox(GUIData *data){
    GtkWidget *left_box = createLeftBox(data);
    GtkWidget *right_box = createRightBox(data);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), left_box, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), right_box, FALSE, FALSE, 0);
    vectorPushBack(&data->widgets, main_box);
    return main_box;
}

void GUI(int argc, char *argv[], Graph *g){
    gtk_init(&argc, &argv);
    Vector widgets = vectorCreate();
    Vector active_vertices = vectorCreate();
    Vector active_edges = vectorCreate();
    Algorithm algo = {NULL, 0, NULL, NULL, false};
    GUIData data = {g, active_vertices, active_edges, widgets, &algo, false, {0.0, 0.0}, {0.0, 0.0}, SELECT_MOVE_MODE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    GtkWidget *window = createWindow(&data);
    GtkWidget *main_box = createMainBox(&data);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    vectorPushBack(&data.widgets, window);
    gtk_widget_show_all(window);
    gtk_main();
}