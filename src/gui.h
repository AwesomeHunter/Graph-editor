#ifndef H_GUI
#define H_GUI

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdbool.h>
#include "vector2.h"
#include "graph.h"
#include "algorithm.h"

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
    bool mouse_pressed;
    Vector2 previous_drawing_area_size;
    Vector2 vertex_shift;
    Algorithm algorithm;
} GUIData;

void GUI(int argc, char *argv[], Graph *g);

#endif