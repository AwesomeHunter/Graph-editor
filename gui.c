#include "gui.h"

#include "callbacks.h"

void connectSignals(GUIData *data){
    GObject *window = gtk_builder_get_object(data->builder, "window");
    gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
    g_signal_connect(window, "destroy", G_CALLBACK(closeWindow), data);

    GObject *drawing_area = gtk_builder_get_object(data->builder, "drawing_area");
    gtk_widget_add_events(GTK_WIDGET(drawing_area), GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(GTK_WIDGET(drawing_area), GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(GTK_WIDGET(drawing_area), GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(drawing_area, "draw",                 G_CALLBACK(drawAll),                  data);
    g_signal_connect(drawing_area, "button-press-event",   G_CALLBACK(buttonClickedSignal),      data);
    g_signal_connect(drawing_area, "button-release-event", G_CALLBACK(buttonReleasedSignal),     data);
    g_signal_connect(drawing_area, "motion-notify-event",  G_CALLBACK(mouseMovedSignal),         data);
    g_signal_connect(drawing_area, "size-allocate",        G_CALLBACK(sizeChanged),              data);

    g_signal_connect(gtk_builder_get_object(data->builder, "button_select_vertex"),   "clicked", G_CALLBACK(selectVertexSignal),   data);
    g_signal_connect(gtk_builder_get_object(data->builder, "button_select_edge"),     "clicked", G_CALLBACK(selectEdgeSignal),     data);
    g_signal_connect(gtk_builder_get_object(data->builder, "button_delete_selected"), "clicked", G_CALLBACK(deleteSelectedSignal), data);
    g_signal_connect(gtk_builder_get_object(data->builder, "button_set_weight"),      "clicked", G_CALLBACK(setWeightSignal),      data);
    g_signal_connect(gtk_builder_get_object(data->builder, "button_clear_text_view"), "clicked", G_CALLBACK(clearConsoleSignal),   data);
    // g_signal_connect(gtk_builder_get_object(data->builder, "button_load_graph"), "clicked", G_CALLBACK(loadGraph),            data);
    // g_signal_connect(gtk_builder_get_object(data->builder, "button_save_graph"), "clicked", G_CALLBACK(saveGraph),            data);
    // g_signal_connect(gtk_builder_get_object(data->builder, "button_algorithms"), "clicked", G_CALLBACK(runAlgorithm),         data);
    g_signal_connect(gtk_builder_get_object(data->builder, "mode_combo"),             "changed", G_CALLBACK(changeMode),           data);
}

void createTextMark(GUIData *data){
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(data->builder, "console")));
    GtkTextIter text_iter_end;
    gtk_text_buffer_get_end_iter(text_buffer, &text_iter_end);
    GtkTextMark *text_mark_end = gtk_text_buffer_create_mark(text_buffer, NULL, &text_iter_end, FALSE);
    data->text_mark = text_mark_end;
}

void initGUIData(GUIData *data, Graph *g){
    GtkBuilder *builder = gtk_builder_new_from_file("ui.glade");
    GtkWidget *drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    Vector2 drawing_area_size = {gtk_widget_get_allocated_width(drawing_area), gtk_widget_get_allocated_height(drawing_area)};
    data->graph                      = g;
    data->active_edge                = NULL;
    data->active_vertex              = NULL;
    data->builder                    = builder;
    data->vertex_radius              = 10;
    data->mouse_position             = (Vector2){0, 0};
    data->active_mode                = SELECT_MODE;
    data->mouse_pressed              = false;
    data->holded_vertex              = NULL;
    data->cr                         = NULL;
    data->text_mark                  = NULL;
    data->previous_drawing_area_size = drawing_area_size;
    createTextMark(data);
    connectSignals(data);
}

void GUI(int argc, char *argv[], Graph *g){
    gtk_init(&argc, &argv);
    GUIData data;
    initGUIData(&data, g);
    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(data.builder, "window")));
    gtk_main();
}