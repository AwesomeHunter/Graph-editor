#include "callbacks.h"

#include <stdbool.h>
#include <gmodule.h>
#include "gui.h"
#include "vector2.h"
#include <math.h>

bool isNumber(const gchar *number){
    for(int i=0;i<(int)strlen(number);i++){
        if(!g_ascii_isdigit(number[i]))
            return false;
    }
    return (g_strcmp0(number, "")!=0);
}

double triangleArea(Vector2 a, Vector2 b, Vector2 c){
    double x = distanceVector2(a, b);
    double y = distanceVector2(b, c);
    double z = distanceVector2(c, a);
    double p = (x + y + z) / 2.0;
    return sqrt(p * (p - x) * (p - y) * (p - z));
}

bool edgeIntersect(Edge *edge, gpointer user_data){
    GUIData *data = user_data;
    Vertex *begin = getVertexById(data->graph, edgeGetBegin(edge));
    Vertex *end = getVertexById(data->graph, edgeGetEnd(edge));
    Vector2 begin_position = vertexGetPosition(begin);
    Vector2 end_position = vertexGetPosition(end);
    double edge_width = 5.0;
    Vector2 direction = normalizeVector2(subtrVector2(end_position, begin_position));
    Vector2 radius_vector = multScalar(direction, data->vertex_radius);
    begin_position = addVector2(begin_position, radius_vector);
    end_position = subtrVector2(end_position, radius_vector);
    Vector2 shift = multScalar(direction, edge_width);
    Vector2 a = {end_position.x - shift.y, end_position.y + shift.x};
    Vector2 b = {end_position.x + shift.y, end_position.y - shift.x};
    Vector2 c = {begin_position.x - shift.y, begin_position.y + shift.x};
    Vector2 d = {begin_position.x + shift.y, begin_position.y - shift.x};
    double area = 2.0 * edge_width * distanceVector2(begin_position, end_position);
    double area_point = triangleArea(a, b, data->mouse_position) + triangleArea(c, d, data->mouse_position) + 
                        triangleArea(b, d, data->mouse_position) + triangleArea(a, c, data->mouse_position);
    double delta = 1e-3;
    return (fabs(area-area_point) < delta);
}

bool edgeLoopIntersect(Edge *edge, gpointer user_data){
    GUIData *data = user_data;
    Vertex *v = getVertexById(data->graph, edgeGetBegin(edge));
    Vector2 vertex_position = vertexGetPosition(v);
    if(distanceVector2(vertex_position, data->mouse_position) <= data->vertex_radius)
        return false;
    vertex_position.y -= 1.9 * data->vertex_radius;
    double edge_width = 5.0;
    double new_dist = distanceVector2(vertex_position, data->mouse_position);
    return (new_dist >= data->vertex_radius * 1.4 - edge_width && new_dist <= data->vertex_radius * 1.4 + edge_width);
}

void insertTextToConsole(const gchar *text, gpointer user_data){
    GUIData *data = user_data;
    GtkTextIter iter;
    GObject *console = gtk_builder_get_object(data->builder, "console");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(console));
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, text, -1);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(console), data->text_mark, 0, FALSE, 0, 0);
}

void displayEdgeInfo(Edge *edge, const gchar *text, gpointer user_data){
    GUIData *data = user_data;
    Vertex *begin = getVertexById(data->graph, edgeGetBegin(edge));
    Vertex *end = getVertexById(data->graph, edgeGetEnd(edge));
    char temp[50];
    g_snprintf(temp, 50, "> EDGE #%d  from: %d | to: %d | weight: %d\n", edgeGetId(edge), vertexGetId(begin), vertexGetId(end), edgeGetWeight(edge));
    insertTextToConsole(text, data);
    insertTextToConsole(temp, data);
}

void displayVertexInfo(Vertex *vertex, const gchar *text, gpointer user_data){
    GUIData *data = user_data;
    char temp[50];
    g_snprintf(temp, 50, "> VERTEX #%d weight: %d\n", vertexGetId(vertex), vertexGetWeight(vertex));
    insertTextToConsole(text, data);
    insertTextToConsole(temp, data);
}

bool isVertexClicked(Vertex *v, gpointer user_data){
    GUIData *data = user_data;
    return (distanceVector2(vertexGetPosition(v), data->mouse_position) <= data->vertex_radius);
}

bool isEdgeClicked(Edge *e, gpointer user_data){
    int begin = edgeGetBegin(e);
    int end = edgeGetEnd(e);
    return ((begin == end && edgeLoopIntersect(e, user_data)) || edgeIntersect(e, user_data));
}

void selectEdge(Edge* edge, gpointer user_data){
    GUIData *data = user_data;
    edgeSetColor(edge, SELECTED_EDGE);
    data->active_edge = edge;
    displayEdgeInfo(edge, "Selected edge:\n", user_data);
}

void selectVertex(Vertex* vertex, gpointer user_data){
    GUIData *data = user_data;
    vertexSetColor(vertex, SELECTED_VERTEX);
    data->active_vertex = vertex;
    displayVertexInfo(vertex, "Selected vertex:\n", user_data);
}

void deselectActive(gpointer user_data){
    GUIData *data = user_data;
    if(data->active_edge){
        edgeSetColor(data->active_edge, NORMAL_EDGE);
        data->active_edge = NULL;
    }
    if(data->active_vertex){
        vertexSetColor(data->active_vertex, NORMAL_VERTEX);
        data->active_vertex = NULL;
    }
}

GArray *getClickedVertices(gpointer user_data){
    GUIData *data = user_data;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, getVertices(data->graph));
    GArray *vertices = g_array_new(false, false, sizeof(Vertex *));
    while (g_hash_table_iter_next (&iter, &key, &value))
        if(isVertexClicked(value, user_data))
            g_array_append_val(vertices, value);
    return vertices;
}

Vertex *getVertexOnTop(GArray *clicked_vertices){
    if(clicked_vertices->len == 0)
        return NULL;
    return g_array_index(clicked_vertices, Vertex *, clicked_vertices->len-1);
}

GArray *getClickedEdges(gpointer user_data){
    GUIData *data = user_data;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, getEdges(data->graph));
    GArray *edges = g_array_new(false, false, sizeof(Edge *));
    while (g_hash_table_iter_next (&iter, &key, &value))
        if(isEdgeClicked(value, user_data))
            g_array_append_val(edges, value);
    return edges;
}

Edge *getEdgeOnTop(GArray *clicked_edges){
    if(clicked_edges->len == 0)
        return NULL;
    return g_array_index(clicked_edges, Edge *, clicked_edges->len-1);
}

void clearConsoleSignal(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(data->builder, "console")));
    gtk_text_buffer_set_text(buffer, "", 0);
    (void)widget;
}

void selectEdgeSignal(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    // if(data->algorithm->running)
    //     return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(data->builder, "edge_id_entry")));
    if(isNumber(text) && data->active_mode == SELECT_MODE){
        int id = atoi(text);
        Edge *edge = getEdgeById(data->graph, id);
        if(edge){
            deselectActive(user_data);
            selectEdge(edge, user_data);
            gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
        }
    }
    (void)widget;
}

void selectVertexSignal(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    // if(data->algorithm->running)
    //     return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(data->builder, "vertex_id_entry")));
    if(isNumber(text) && data->active_mode == SELECT_MODE){
        int id = atoi(text);
        Vertex *vertex = getVertexById(data->graph, id);
        if(vertex){
            deselectActive(user_data);
            selectVertex(vertex, user_data);
            gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
        }
    }
    (void)widget;
}

void setWeightSignal(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    // if(data->algorithm->running)
    //     return;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(data->builder, "weight_entry")));
    if(isNumber(text)){
        int weight = atoi(text);
        if(weight == 0)
            return;
        if(data->active_edge){
            edgeSetWeight(data->active_edge, weight);
            displayEdgeInfo(data->active_edge, "Edge weight changed\n", data);
        }
        if(data->active_vertex){
            vertexSetWeight(data->active_vertex, weight);
            displayVertexInfo(data->active_vertex, "Vertex weight changed\n", data);
        }
    }
    (void)widget;
}

void deleteSelectedSignal(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    // if(data->algorithm->running)
    //     return;
    Edge *edge = data->active_edge;
    Vertex *vertex = data->active_vertex;
    deselectActive(user_data);
    if(edge){
        deleteEdge(data->graph, edgeGetId(edge));
        displayEdgeInfo(edge, "Edge deleted\n", data);
    }
    if(vertex){
        deleteVertex(data->graph, vertexGetId(vertex));
        displayVertexInfo(vertex, "Vertex deleted\n", data);
    }
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
    (void)widget;
}

void changeMode(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    // if(data->algorithm->running)
    //     return;
    deselectActive(data);
    gchar *mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(gtk_builder_get_object(data->builder, "mode_combo")));
    if(g_strcmp0(mode, "select mode") == 0){
        data->active_mode = SELECT_MODE;
    }else if(g_strcmp0(mode, "add mode") == 0){
        data->active_mode = ADD_MODE;
    }else if(g_strcmp0(mode, "delete mode") == 0){
        data->active_mode = DELETE_MODE;
    }
    insertTextToConsole("Current mode: ", data);
    insertTextToConsole(mode, data);
    insertTextToConsole("\n", data);
    g_free(mode);
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
    (void)widget;
}

void addModeClickEvent(Vertex *clicked_vertex, Edge *clicked_edge, gpointer user_data){
    GUIData *data = user_data;
    if(clicked_vertex && data->active_vertex){
        int e_id = addEdge(data->graph, vertexGetId(data->active_vertex), vertexGetId(clicked_vertex), 1);
        data->active_vertex = clicked_vertex;
        displayEdgeInfo(getEdgeById(data->graph, e_id), "Edge added\n", data);
    }else if(clicked_vertex){
        data->active_vertex = clicked_vertex;
    }else if(data->active_vertex){
        deselectActive(user_data);
    }else{
        int v_id = addVertex(data->graph, data->mouse_position, 1);
        displayVertexInfo(getVertexById(data->graph, v_id), "Vertex added\n", data);
    }
    (void)clicked_edge;
}

void selectModeClickEvent(Vertex *clicked_vertex, Edge *clicked_edge, gpointer user_data){
    GUIData *data = user_data;
    Vertex *prev_active_vertex = data->active_vertex;
    Edge *prev_active_edge = data->active_edge;
    deselectActive(user_data);
    if(clicked_vertex && clicked_vertex != prev_active_vertex){
        selectVertex(clicked_vertex, user_data);
        data->vertex_shift = subtrVector2(data->mouse_position, vertexGetPosition(data->active_vertex));
    }else if(clicked_edge && clicked_edge != prev_active_edge){
        selectEdge(clicked_edge, user_data);
    }
}

void deleteModeClickEvent(Vertex *clicked_vertex, Edge *clicked_edge, gpointer user_data){
    GUIData *data = user_data;
    if(clicked_vertex){
        displayVertexInfo(clicked_vertex, "Vertex deleted\n", data);
        deleteVertex(data->graph, vertexGetId(clicked_vertex));
    }else if(clicked_edge){
        displayEdgeInfo(clicked_edge, "Edge deleted\n", data);
        deleteEdge(data->graph, edgeGetId(clicked_edge));
    }
}

void printClickedEdges(GArray *clicked_edges, gpointer user_data){
    insertTextToConsole("Clicked edges:\n", user_data);
    for(int i=0;i<(int)clicked_edges->len;i++){
        displayEdgeInfo(g_array_index(clicked_edges, Edge *, i), "", user_data);
    }
}

void printClickedVertices(GArray *clicked_vertices, gpointer user_data){
    insertTextToConsole("Clicked vertices:\n", user_data);
    for(int i=0;i<(int)clicked_vertices->len;i++){
        displayVertexInfo(g_array_index(clicked_vertices, Vertex *, i), "", user_data);
    }
}

void getClickedObjects(Vertex **clicked_vertex, Edge **clicked_edge, gpointer user_data){
    GArray *clicked_vertices = getClickedVertices(user_data);
    GArray *clicked_edges = getClickedEdges(user_data);
    if(clicked_vertices->len > 1){
        printClickedVertices(clicked_vertices, user_data);
    }else if(clicked_edges->len > 1){
        printClickedEdges(clicked_edges, user_data);
    }
    *clicked_vertex = getVertexOnTop(clicked_vertices);
    *clicked_edge = getEdgeOnTop(clicked_edges);
    g_array_free(clicked_vertices, TRUE);
    g_array_free(clicked_edges, TRUE);
}

void buttonClickedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    GUIData *data = user_data;
    GdkEventMotion *ev = (GdkEventMotion *)event;
    if(event->button != GDK_BUTTON_PRIMARY || event->type == GDK_2BUTTON_PRESS)// || data->algorithm->running)
        return;
    data->mouse_position = (Vector2){ev->x, ev->y};
    data->mouse_pressed = true;
    Edge *clicked_edge;
    Vertex *clicked_vertex;
    getClickedObjects(&clicked_vertex, &clicked_edge, user_data);
    switch (data->active_mode){
    case ADD_MODE:
        addModeClickEvent(clicked_vertex, clicked_edge, user_data);
        break;
    case SELECT_MODE:
        selectModeClickEvent(clicked_vertex, clicked_edge, user_data);
        break;
    case DELETE_MODE:
        deleteModeClickEvent(clicked_vertex, clicked_edge, user_data);
        break;
    default:
        break;
    }
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
    (void)widget;
}

void mouseMovedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    GUIData *data = user_data;
    GtkWidget *drawing_area = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area"));
    GdkEventMotion *ev = (GdkEventMotion *)event;
    data->mouse_position = (Vector2){ev->x, ev->y};
    if(data->mouse_pressed && data->active_vertex && data->active_mode == SELECT_MODE){
        double width = gtk_widget_get_allocated_width(drawing_area);
        double height = gtk_widget_get_allocated_height(drawing_area);
        Vector2 new_position = addVector2(data->mouse_position, data->vertex_shift);
        new_position.x = fmax(fmin(new_position.x, width), 0.0);
        new_position.y = fmax(fmin(new_position.y, height), 0.0);
        vertexSetPosition(data->active_vertex, new_position);
    }
    gtk_widget_queue_draw(drawing_area);
    (void)widget;
}

void buttonReleasedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    GUIData *data = user_data;
    data->mouse_pressed = false;
    (void)widget, (void)event;
}

void updateVertexPosition(gpointer key, gpointer value, gpointer user_data){
    GUIData *data = user_data;
    GtkWidget *drawing_area = GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area"));
    Vector2 new_drawing_area_size = {gtk_widget_get_allocated_width(drawing_area), gtk_widget_get_allocated_height(drawing_area)};
    Vertex *vertex = value;
    Vector2 new_position = multVector2(divVector2(new_drawing_area_size, data->previous_drawing_area_size), vertexGetPosition(vertex));
    vertexSetPosition(vertex, new_position);
    (void)key;
}

void sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer user_data){
    GUIData *data = user_data;
    g_hash_table_foreach(getVertices(data->graph), updateVertexPosition, user_data);
    data->previous_drawing_area_size = (Vector2){allocation->width, allocation->height};
    gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(data->builder, "drawing_area")));
    (void)widget;
}

void drawLoopEdge(Edge *edge, gpointer user_data){
    GUIData *data = user_data;
    Vertex *v = getVertexById(data->graph, edgeGetBegin(edge));
    Vector2 position = vertexGetPosition(v);
    position.y -= 1.9 * data->vertex_radius;
    Color c = edgeGetColor(edge);
    cairo_set_source_rgb(data->cr, c.r, c.g, c.b);
    cairo_arc(data->cr, position.x, position.y, data->vertex_radius * 1.4, 0, 2 * G_PI);
    double x_shift = 70.0/data->vertex_radius;
    double y_shift = 10.0/data->vertex_radius;
    cairo_move_to(data->cr, position.x + x_shift, position.y + y_shift);
    cairo_line_to(data->cr, position.x + x_shift, position.y + y_shift + data->vertex_radius);
    cairo_line_to(data->cr, position.x + x_shift + data->vertex_radius, position.y + y_shift + data->vertex_radius);
}

void drawEdgeBetweenPoints(Vector2 begin, Vector2 end, Color c, gpointer user_data){
    GUIData *data = user_data;
    Vector2 radius_vector = multScalar(normalizeVector2(subtrVector2(end, begin)), data->vertex_radius);
    Vector2 rotated_top_begin = multScalar((Vector2){radius_vector.x - radius_vector.y, radius_vector.x + radius_vector.y}, G_SQRT2/2.0);
    Vector2 rotated_top_end = multScalar((Vector2){radius_vector.x + radius_vector.y, -radius_vector.x + radius_vector.y}, G_SQRT2/2.0);
    Vector2 begin_arrow = subtrVector2(end, rotated_top_begin);
    Vector2 end_arrow = subtrVector2(end, rotated_top_end);
    cairo_set_source_rgb(data->cr, c.r, c.g, c.b);
    cairo_set_line_width(data->cr, 1.5);
    cairo_move_to(data->cr, begin.x, begin.y);
    cairo_line_to(data->cr, end.x, end.y);
    cairo_move_to(data->cr, begin_arrow.x, begin_arrow.y);
    cairo_line_to(data->cr, end.x, end.y);
    cairo_line_to(data->cr, end_arrow.x, end_arrow.y);
}

void drawStraightEdge(Edge *edge, gpointer user_data){
    GUIData *data = user_data;
    Vector2 begin = vertexGetPosition(getVertexById(data->graph, edgeGetBegin(edge)));
    Vector2 end = vertexGetPosition(getVertexById(data->graph, edgeGetEnd(edge)));
    Vector2 radius_vector = multScalar(normalizeVector2(subtrVector2(end, begin)), data->vertex_radius);
    begin = addVector2(begin, radius_vector);
    end = subtrVector2(end, radius_vector);
    drawEdgeBetweenPoints(begin, end, edgeGetColor(edge), user_data);
}

void drawVertex(gpointer key, gpointer value, gpointer user_data){
    GUIData *data = user_data;
    Vertex *v = value;
    Color c = vertexGetColor(v);
    cairo_set_source_rgb(data->cr, c.r, c.g, c.b);
    Vector2 pos = vertexGetPosition(v);
    cairo_arc(data->cr, pos.x, pos.y, data->vertex_radius, 0, 2 * G_PI);
    cairo_fill(data->cr);
    (void)key;
}

void drawEdge(gpointer key, gpointer value, gpointer user_data){
    GUIData *data = user_data;
    Edge *e = value;
    if(edgeGetBegin(e) == edgeGetEnd(e))
        drawLoopEdge(e, user_data);
    else
        drawStraightEdge(e, user_data);
    cairo_stroke(data->cr);
    (void)key;
}

void drawGhostEdge(gpointer user_data){
    GUIData *data = user_data;
    if(data->active_vertex && data->active_mode == ADD_MODE){
        Vector2 vertex_position = vertexGetPosition(data->active_vertex);
        Vector2 radius_vector = multScalar(normalizeVector2(subtrVector2(data->mouse_position, vertex_position)), data->vertex_radius);
        Vector2 begin = addVector2(vertex_position, radius_vector);
        drawEdgeBetweenPoints(begin, data->mouse_position, GHOST_EDGE, user_data);
        cairo_stroke(data->cr);
    }
}

void drawAll(GtkWidget *widget, cairo_t *cr, gpointer user_data){
    GUIData *data = user_data;
    data->cr = cr;
    g_hash_table_foreach(getEdges(data->graph), drawEdge, user_data);
    g_hash_table_foreach(getVertices(data->graph), drawVertex, user_data);
    drawGhostEdge(user_data);
    (void)widget;
}

void closeWindow(GtkWidget *widget, gpointer user_data){
    GUIData *data = user_data;
    destroyGraph(data->graph);
    gtk_text_buffer_delete_mark(gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(data->builder, "console"))), data->text_mark);
    gtk_widget_destroy(GTK_WIDGET(gtk_builder_get_object(data->builder, "window")));
    g_object_unref(G_OBJECT(data->builder));
    gtk_main_quit();
    (void)widget;
}