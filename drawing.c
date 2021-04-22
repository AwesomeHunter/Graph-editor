#include "drawing.h"

#define VERTEX_RADIUS 10

double dist(Point a, Point b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double triangleArea(Point a, Point b, Point c){
    double x = dist(a, b);
    double y = dist(b, c);
    double z = dist(c, a);
    double p = (x + y + z) / 2.0;
    return sqrt(p * (p - x) * (p - y) * (p - z));
}

bool rectIntersect(Point begin, Point end, Point mouse, double width){
    Point delta_arrow = multScalar(subtrPoint(end, begin), VERTEX_RADIUS/dist(begin, end));
    begin = addPoint(begin, delta_arrow);
    end = subtrPoint(end, delta_arrow);
    double len = dist(begin, end);
    Point vector = divScalar(subtrPoint(end, begin), len / width);
    Point a = {end.x - vector.y, end.y + vector.x};
    Point b = {end.x + vector.y, end.y - vector.x};
    Point c = {begin.x - vector.y, begin.y + vector.x};
    Point d = {begin.x + vector.y, begin.y - vector.x};
    double area = 2.0 * width * len;
    double area_point = triangleArea(a, b, mouse) + triangleArea(c, d, mouse) + 
                        triangleArea(b, d, mouse) + triangleArea(a, c, mouse);
    double delta = 1e-3;
    return (fabs(area-area_point) < delta);
}

bool edgeLoopIntersect(Point position, Point mouse, double width){
    if(dist(position, mouse) <= VERTEX_RADIUS)
        return false;
    position.y -= 1.9 * VERTEX_RADIUS;
    double new_dist = dist(position, mouse);
    if(new_dist >= VERTEX_RADIUS * 1.4 - width && new_dist <= VERTEX_RADIUS * 1.4 + width)
        return true;
    return false;
}

void drawVertex(cairo_t *cr, Vertex *vertex){
    Color c = vertexGetColor(vertex);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    Point pos = vertexGetPosition(vertex);
    cairo_arc(cr, pos.x, pos.y, VERTEX_RADIUS, 0, 2 * G_PI);
    cairo_fill(cr);
}

void drawLoopEdge(cairo_t *cr, Edge *edge){
    Vertex *v = edgeGetBegin(edge);
    Point position = vertexGetPosition(v);
    position.y -= 1.9 * VERTEX_RADIUS;
    Color c = edgeGetColor(edge);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_arc(cr, position.x, position.y, VERTEX_RADIUS * 1.4, 0, 2 * G_PI);
    cairo_move_to(cr, position.x + 7, position.y + 1);
    cairo_line_to(cr, position.x + 7, position.y + 1 + VERTEX_RADIUS);
    cairo_line_to(cr, position.x + 7 + VERTEX_RADIUS, position.y + 1 + VERTEX_RADIUS);
}

void drawEdge(cairo_t *cr, Edge *edge){
    Point begin = vertexGetPosition(edgeGetBegin(edge));
    Point end = vertexGetPosition(edgeGetEnd(edge));
    Point delta_arrow = multScalar(subtrPoint(end, begin), VERTEX_RADIUS/dist(end, begin));
    Point begin_arrow = addPoint(begin, delta_arrow);
    Point end_arrow = subtrPoint(end, delta_arrow);
    Point delta_rotated_vector_begin = {delta_arrow.x - delta_arrow.y, delta_arrow.x + delta_arrow.y};
    delta_rotated_vector_begin = multScalar(delta_rotated_vector_begin, G_SQRT2/2.0);
    Point delta_rotated_vector_end = {delta_arrow.x + delta_arrow.y, -delta_arrow.x + delta_arrow.y};
    delta_rotated_vector_end = multScalar(delta_rotated_vector_end, G_SQRT2/2.0);
    Point begin_peak = subtrPoint(end_arrow, delta_rotated_vector_begin);
    Point end_peak = subtrPoint(end_arrow, delta_rotated_vector_end);
    Color c = edgeGetColor(edge);
    cairo_set_source_rgb(cr, c.r, c.g, c.b);
    cairo_set_line_width(cr, 1.5);
    cairo_move_to(cr, begin_arrow.x, begin_arrow.y);
    cairo_line_to(cr, end_arrow.x, end_arrow.y);
    cairo_move_to(cr, begin_peak.x, begin_peak.y);
    cairo_line_to(cr, end_arrow.x, end_arrow.y);
    cairo_line_to(cr, end_peak.x, end_peak.y);
}

void drawAll(GtkWidget *widget, cairo_t *cr, gpointer data_pointer){
    GUIData *data = data_pointer;
    Vector edges = getEdges(data->graph);
    for(int i=0;i<vectorSize(edges);i++){
        Edge *edge = vectorValue(edges, i);
        if(edgeGetBegin(edge) == edgeGetEnd(edge))
            drawLoopEdge(cr, edge);
        else
            drawEdge(cr, edge);
        cairo_stroke(cr);
    }
    Vector vertices = getVertices(data->graph);
    for(int i=0;i<vectorSize(vertices);i++){
        Vertex *vertex = vectorValue(vertices, i);
        drawVertex(cr, vertex);
    }
    (void)widget;
}
