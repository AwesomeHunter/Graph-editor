all: app.c vector.c heap.c point.c drawing.c gui.c algorithms.c graph.c deque.c edge.c vertex.c colors.c
	gcc -g -xc -std=c11 -Wall -Wextra -Werror app.c -o app vector.c heap.c point.c drawing.c gui.c algorithms.c graph.c deque.c edge.c vertex.c colors.c -lm `pkg-config --cflags gtk+-3.0 --libs`