# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS=-std=c++11
INCLUDE=-I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system -I/usr/local/include -L/usr/local/lib -lSDL2 -lpthread

all: server client_demo client_victor

build/server.o: server.cpp 
	$(CC) $(CFLAGS) -c server.cpp $(INCLUDE) -o build/server.o

build/client.o: client.cpp client.hpp 
	$(CC) $(CFLAGS) -c client.cpp $(INCLUDE) -o build/client.o

build/client_demo.o: client_demo.cpp
	$(CC) $(CFLAGS) -c client_demo.cpp  $(INCLUDE) -o build/client_demo.o

build/client_victor.o: client_victor.cpp
	$(CC) $(CFLAGS) -c client_victor.cpp $(INCLUDE) -o build/client_victor.o

build/game_structs.o: game_structs.cpp game_structs.hpp
	$(CC) $(CFLAGS) -c game_structs.cpp  $(INCLUDE) -o build/game_structs.o

build/json_serializer.o: json_serializer.cpp
	$(CC) $(CFLAGS) -c json_serializer.cpp $(INCLUDE) -o build/json_serializer.o

server: build/server.o build/game_structs.o build/json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/server.o build/json_serializer.o $(INCLUDE) -o server

client_demo: build/client_demo.o build/client.o build/game_structs.o build/json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/client.o build/json_serializer.o build/client_demo.o $(INCLUDE) -o client_demo

client_victor: build/client_victor.o build/client.o build/game_structs.o build/json_serializer.o
	$(CC) $(CFLAGS) build/client_victor.o build/client.o build/game_structs.o build/json_serializer.o $(INCLUDE) -o client_victor

clean:
	rm -rf *.o
	rm -rf build/*
