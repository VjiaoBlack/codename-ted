# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS=-std=c++11
INCLUDE=-I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system -I/usr/local/include -L/usr/local/lib -lSDL2 -lpthread

all: server client_demo

server.o: server.cpp game_structs.cpp
	$(CC) $(CFLAGS) -c server.cpp $(INCLUDE) -o build/server.o

client.o: client.cpp game_structs.cpp client.hpp json_serializer.hpp
	$(CC) $(CFLAGS) -c client.cpp $(INCLUDE) -o build/client.o

client_demo.o: client_demo.cpp client.cpp client.hpp
	$(CC) $(CFLAGS) -c client_demo.cpp  $(INCLUDE) -o build/client_demo.o

game_structs.o: game_structs.cpp game_structs.hpp
	$(CC) $(CFLAGS) -c game_structs.cpp  $(INCLUDE) -o build/game_structs.o

json_serializer.o: json_serializer.cpp game_structs.hpp
	$(CC) $(CFLAGS) -c json_serializer.cpp $(INCLUDE) -o build/json_serializer.o

server: server.o game_structs.o json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/server.o build/json_serializer.o $(INCLUDE) -o server

client_demo: client_demo.o client.o game_structs.o json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/client.o build/json_serializer.o build/client_demo.o $(INCLUDE) -o client_demo

clean:
	rm -rf *.o
	rm -rf build/*
