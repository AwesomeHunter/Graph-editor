#ifndef H_DRAWING
#define H_DRAWING

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#include "vertex.h"
#include "edge.h"
#include "point.h"
#include "graph.h"
#include "gui.h"

double dist(Point a, Point b);
double triangleArea(Point a, Point b, Point c);
bool rectIntersect(Point begin, Point end, Point mouse, double width);
bool edgeLoopIntersect(Point position, Point mouse, double width);
void drawVertex(cairo_t *cr, Vertex *vertex);
void drawLoopEdge(cairo_t *cr, Edge *edge);
void drawEdge(cairo_t *cr, Edge *edge);
void drawAll(GtkWidget *widget, cairo_t *cr, gpointer data_pointer);

#endif