# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS=-std=c++11
INCLUDE=-I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system -I/usr/local/include -L/usr/local/lib -lSDL2 -lpthread

all: server client_demo client_keystroke_demo

build/physics_tryout.o: ../physics_ck/physics_tryout.cpp ../physics_ck/physics_tryout.hpp
	$(CC) $(CFLAGS) -c ../physics_ck/physics_tryout.cpp $(INCLUDE) -o build/physics_tryout.o

build/server.o: server.cpp server.hpp
	$(CC) $(CFLAGS) -c server.cpp $(INCLUDE) -o build/server.o

build/client.o: client.cpp client.hpp
	$(CC) $(CFLAGS) -c client.cpp $(INCLUDE) -o build/client.o

build/client_demo.o: client_demo.cpp
	$(CC) $(CFLAGS) -c client_demo.cpp  $(INCLUDE) -o build/client_demo.o

build/client_keystroke_demo.o: client_keystroke_demo.cpp
	$(CC) $(CFLAGS) -c client_keystroke_demo.cpp  $(INCLUDE) -o build/client_keystroke_demo.o

build/game_structs.o: game_structs.cpp game_structs.hpp
	$(CC) $(CFLAGS) -c game_structs.cpp  $(INCLUDE) -o build/game_structs.o

build/json_serializer.o: json_serializer.cpp
	$(CC) $(CFLAGS) -c json_serializer.cpp $(INCLUDE) -o build/json_serializer.o

build/astar.o: ../ai/astar.cpp ../ai/astar.hpp
	$(CC) $(CFLAGS) -c ../ai/astar.cpp $(INCLUDE) -o build/astar.o

server: build/server.o build/game_structs.o build/json_serializer.o build/physics_tryout.o build/astar.o
	$(CC) $(CFLAGS) build/physics_tryout.o build/game_structs.o build/server.o build/json_serializer.o build/astar.o $(INCLUDE) -o server

client_demo: build/client_demo.o build/client.o build/game_structs.o build/json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/client.o build/json_serializer.o build/client_demo.o $(INCLUDE) -o client_demo

client_keystroke_demo: build/client_keystroke_demo.o build/client.o build/game_structs.o build/json_serializer.o
	$(CC) $(CFLAGS) build/game_structs.o build/client.o build/json_serializer.o build/client_keystroke_demo.o $(INCLUDE) -o client_keystroke_demo

clean:
	rm -rf *.o
	rm -rf build/*
