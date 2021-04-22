#include "graph.h"
#include "gui.h"

int main(int argc, char *argv[]){
    Graph g = createGraph();
    GUI(argc, argv, &g);
    return 0;
}