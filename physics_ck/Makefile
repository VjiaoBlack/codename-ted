CC=g++
CFLAGS=-std=c++11
INCLUDE=


all: physics_tryout

physics_tryout: build/physics_tryout.o build/game_structs.o
	$(CC) $(CFLAGS) build/physics_tryout.o build/game_structs.o $(INCLUDE) -lSDL2 -o physics_tryout

build/physics_tryout.o: physics_tryout.cpp physics_tryout.hpp
	$(CC) $(CFLAGS) -c physics_tryout.cpp $(INCLUDE) -lSDL2 -o build/physics_tryout.o

build/game_structs.o: ../game_server/game_structs.cpp ../game_server/game_structs.hpp
	$(CC) $(CFLAGS) -c ../game_server/game_structs.cpp $(INCLUDE) -lSDL2 -o build/game_structs.o





clean:
	rm -rf *.o
	rm -rf build/*
	rm -f physics_tryout

