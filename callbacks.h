#ifndef H_CALLBACKS
#define H_CALLBACK

#include <gtk/gtk.h>
#include <cairo.h>
#include "edge.h"
#include "vertex.h"

// gboolean isVertexClicked(gpointer key, gpointer value, gpointer user_data);
// gboolean isEdgeClicked(gpointer key, gpointer value, gpointer user_data);
void mouseMovedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void buttonReleasedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void deselectActive(gpointer user_data);
void selectEdge(Edge *edge, gpointer user_data);
void selectVertex(Vertex *vertex, gpointer user_data);
void buttonClickedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void clearConsoleSignal(GtkWidget *widget, gpointer user_data);
void selectEdgeSignal(GtkWidget *widget, gpointer user_data);
void selectVertexSignal(GtkWidget *widget, gpointer user_data);
void setWeightSignal(GtkWidget *widget, gpointer user_data);
void deleteSelectedSignal(GtkWidget *widget, gpointer user_data);
void changeMode(GtkWidget *widget, gpointer user_data);
void updateVertexPosition(gpointer key, gpointer value, gpointer user_data);
void sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer user_data);
void drawLoopEdge(Edge *edge, gpointer user_data);
void drawStraightEdge(Edge *edge, gpointer user_data);
void drawVertex(gpointer key, gpointer value, gpointer user_data);
void drawEdge(gpointer key, gpointer value, gpointer user_data);
void drawAll(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void closeWindow(GtkWidget *widget, gpointer user_data);

#endif