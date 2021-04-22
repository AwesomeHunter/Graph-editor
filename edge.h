#ifndef H_EDGE
#define H_EDGE

#include <stdlib.h>

#include "vertex.h"
#include "colors.h"

typedef struct Vertex Vertex;
typedef struct Edge{
    int id, weight;
    Vertex *begin, *end;
    Color color;
} Edge;
Edge *edgeCreate(int id, int weight, Vertex *begin, Vertex *end);
int edgeGetId(Edge *e);
int edgeGetWeight(Edge *e);
Vertex *edgeGetBegin(Edge *e);
Vertex *edgeGetEnd(Edge *e);
Color edgeGetColor(Edge *e);

void edgeSetId(Edge *e, int id);
void edgeSetColor(Edge *e, Color c);
void edgeSetWeight(Edge *e, int weight);
void edgeSetBegin(Edge *e, Vertex *begin);
void edgeSetEnd(Edge *e, Vertex *end);

void edgeDestroy(Edge *e);

#endif