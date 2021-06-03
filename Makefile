# OBJS	= main.o gui.o vertex.o edge.o colors.o graph.o utils.o vector2.o callbacks.o
# SOURCE	= main.c gui.c vertex.c edge.c colors.c graph.c utils.c vector2.c callbacks.c
# HEADER	= gui.h vertex.h edge.h colors.h graph.h utils.h vector2.h callbacks.h
# OUT	= main
# CC	 = gcc
# FLAGS	 = -g -c -Wall `pkg-config --cflags gtk+-3.0 --libs`
# LFLAGS	 = -lm

# all: $(OBJS)
# 	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

# main.o: main.c
# 	$(CC) $(FLAGS) main.c 

# gui.o: gui.c
# 	$(CC) $(FLAGS) gui.c 

# vertex.o: vertex.c
# 	$(CC) $(FLAGS) vertex.c 

# edge.o: edge.c
# 	$(CC) $(FLAGS) edge.c 

# colors.o: colors.c
# 	$(CC) $(FLAGS) colors.c 

# graph.o: graph.c
# 	$(CC) $(FLAGS) graph.c 

# utils.o: utils.c
# 	$(CC) $(FLAGS) utils.c 

# vector2.o: vector2.c
# 	$(CC) $(FLAGS) vector2.c 

# callbacks.o: callbacks.c
# 	$(CC) $(FLAGS) callbacks.c 


# clean:
# 	rm -f $(OBJS) $(OUT)

all: main.c edge.c gui.c callbacks.c colors.c vector2.c vertex.c graph.c
	gcc @flags main.c -o main edge.c gui.c callbacks.c colors.c vector2.c vertex.c graph.c -lm `pkg-config --cflags gtk+-3.0 --libs`