CC=g++
CFLAGS=-std=c++11
INCLUDE=`pkg-config --cflags --libs sdl2`


all: test_ai 

test_ai: build/astar.o build/game_structs.o build/test_ai.o
	$(CC) $(CFLAGS) build/astar.o build/game_structs.o build/test_ai.o $(INCLUDE) -o test_ai 

build/test_ai.o: test_ai.cpp
	$(CC) $(CFLAGS) -c test_ai.cpp $(INCLUDE) -o build/test_ai.o

build/astar.o: astar.cpp astar.hpp
	$(CC) $(CFLAGS) -c astar.cpp $(INCLUDE) -o build/astar.o

build/game_structs.o: ../game_server/game_structs.cpp ../game_server/game_structs.hpp
	$(CC) $(CFLAGS) -c ../game_server/game_structs.cpp $(INCLUDE) -o build/game_structs.o





clean:
	rm -rf *.o
	rm -rf build/*
	rm -f astar

