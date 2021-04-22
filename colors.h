#ifndef H_COLORS
#define H_COLORS

typedef struct Color{
    double r, g, b;
} Color;

extern Color SELECTED_VERTEX; 
extern Color NORMAL_VERTEX;
extern Color SELECTED_EDGE;
extern Color NORMAL_EDGE;
extern Color ACTIVE_VERTEX; 
extern Color VISITED_VERTEX; 
extern Color IN_QUEUE_VERTEX; 

#endif