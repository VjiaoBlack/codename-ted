CC=g++
CFLAGS=-std=c++11
INCLUDE=


all: astar

astar: build/astar.o build/game_structs.o
	$(CC) $(CFLAGS) build/astar.o build/game_structs.o $(INCLUDE) -o astar

build/astar.o: astar.cpp astar.hpp
	$(CC) $(CFLAGS) -c astar.cpp $(INCLUDE) -o build/astar.o

build/game_structs.o: ../game_server/game_structs.cpp ../game_server/game_structs.hpp
	$(CC) $(CFLAGS) -c ../game_server/game_structs.cpp $(INCLUDE) -o build/game_structs.o





clean:
	rm -rf *.o
	rm -rf build/*
	rm -f astar

