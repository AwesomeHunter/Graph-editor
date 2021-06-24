#ifndef H_CALLBACKS
#define H_CALLBACK

#include <gtk/gtk.h>
#include <cairo.h>

void mouseMovedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void buttonReleasedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void buttonClickedSignal(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void clearConsoleSignal(GtkWidget *widget, gpointer user_data);
void selectEdgeSignal(GtkWidget *widget, gpointer user_data);
void selectVertexSignal(GtkWidget *widget, gpointer user_data);
void setWeightSignal(GtkWidget *widget, gpointer user_data);
void deleteSelectedSignal(GtkWidget *widget, gpointer user_data);
void changeMode(GtkWidget *widget, gpointer user_data);
void sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer user_data);
void drawAll(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void closeWindow(GtkWidget *widget, gpointer user_data);
void saveGraphSignal(GtkWidget *widget, gpointer user_data);
void loadGraphSignal(GtkWidget *widget, gpointer user_data);
void runAlgorithmSignal(GtkWidget *widget, gpointer user_data);

#endif