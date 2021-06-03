#ifndef H_GUI
#define H_GUI

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdbool.h>

// #include "algorithms.h"
#include "vector2.h"
#include "graph.h"
#include "vertex.h"
#include "edge.h"

typedef enum Mode{
    SELECT_MODE,
    ADD_MODE,
    DELETE_MODE
} Mode;

typedef struct GUIData{
    Graph *graph;
    Vertex *active_vertex;
    Edge *active_edge;
    GtkBuilder *builder;
    Vector2 mouse_position;
    int vertex_radius;
    Mode active_mode;
    cairo_t *cr;
    GtkTextMark *text_mark;
    // Algorithm *algorithm;
    bool mouse_pressed;
    Vertex *holded_vertex;
    Vector2 previous_drawing_area_size;
    Vector2 vertex_shift;
} GUIData;

void GUI(int argc, char *argv[], Graph *g);

#endif