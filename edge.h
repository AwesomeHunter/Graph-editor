#ifndef H_EDGE
#define H_EDGE

#include "colors.h"

typedef struct Edge{
    int id, weight;
    int begin, end;
    Color color;
} Edge;

Edge *edgeCreate(int id, int weight, int begin_id, int end_id);

int edgeGetId(Edge *e);
int edgeGetWeight(Edge *e);
int edgeGetBegin(Edge *e);
int edgeGetEnd(Edge *e);
Color edgeGetColor(Edge *e);

void edgeSetId(Edge *e, int id);
void edgeSetColor(Edge *e, Color c);
void edgeSetWeight(Edge *e, int weight);
void edgeSetBegin(Edge *e, int begin);
void edgeSetEnd(Edge *e, int end);

void edgeDestroy(Edge *e);

#endif