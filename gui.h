#ifndef H_GUI
#define H_GUI

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "algorithms.h"
#include "drawing.h"
#include "colors.h"
#include "deque.h"
#include "graph.h"
#include "point.h"
#include "vector.h"
#include "vertex.h"

typedef struct Algorithm Algorithm;

typedef enum Mode{
    SELECT_MOVE_MODE, ADD_VERTEX_MODE, ADD_EDGE_MODE, DELETE_MODE
} Mode;

typedef struct GUIData{
    Graph *graph;
    Vector active_vertices;
    Vector active_edges;
    Vector widgets;
    Algorithm *algorithm;
    bool mouse_pressed;
    Point delta_mouse_pos;
    Point prev_drawing_area_size;
    Mode cur_mode;
    GtkWidget *text_view;
    GtkTextMark *text_mark_end;
    GtkWidget *drawing_area;
    GtkWidget *vertex_id_entry;
    GtkWidget *edge_id_entry;
    GtkWidget *weight_entry;
    GtkWidget *load_entry;
    GtkWidget *save_entry;
    GtkWidget *algorithms_combo;
    GtkWidget *mode_combo;
} GUIData;

void closeWindow(GtkWidget *widget, gpointer data_pointer);
bool isNumber(const gchar *number);
void sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer data_pointer);
GtkWidget *createWindow(GUIData* data);
void insertTextToTextView(const gchar *text, GUIData *data);
GtkWidget *createTextView();
GtkTextMark *createTextMark(GtkWidget *text_view);
GtkWidget *createScrolledTextView(GtkWidget *text_view);
GtkWidget *createTextEntry(int max_len);
void clearTextViewSignal(GtkWidget *widget, gpointer data_pointer);
void displayEdgeInfo(Edge *edge, const char *action, GUIData *data);
void displayVertexInfo(Vertex *vertex, const char *action, GUIData *data);
void deselectEverything(GUIData *data);
Vertex *tryToSelectVertex(Point mouse, GUIData *data);
Edge *tryToSelectEdge(Point mouse, GUIData *data);
void selectEdge(Edge *edge, GUIData *data);
void selectVertex(Vertex *vertex, GUIData *data);
void clickedDrawingAreaSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer);
void buttonReleasedSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer);
void mouseMovedSignal(GtkWidget *widget, GdkEventButton *event, gpointer data_pointer);
void selectVertexSignal(GtkWidget *widget, gpointer data_pointer);
void selectEdgeSignal(GtkWidget *widget, gpointer data_pointer);
void deleteSelectedSignal(GtkWidget *widget, gpointer data_pointer);
void setWeightSignal(GtkWidget *widget, gpointer data_pointer);
void loadGraph(GtkWidget *widget, gpointer data_pointer);
void saveGraph(GtkWidget *widget, gpointer data_pointer);
void runAlgorithm(GtkWidget *widget, gpointer data_pointer);
GtkWidget *createRightBox(GUIData *data);
GtkWidget *createLeftBox(GUIData *data);
GtkWidget *createMainBox(GUIData *data);
void GUI(int argc, char *argv[], Graph *g);

#endif