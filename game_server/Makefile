# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS=-std=c++11
INCLUDE=-I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system -I/usr/local/include -L/usr/local/lib -lSDL2 -lpthread

all: server client

all_echo: echo_server udp_echo_client

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp $(INCLUDE) -o build/server.o

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp $(INCLUDE) -o build/client.o

JSON.o: JSON.cpp
	$(CC) $(CFLAGS) -c JSON.cpp $(INCLUDE) -o build/JSON.o

TdBike.o: TdBike.cpp TdBike.h
	$(CC) $(CFLAGS) -c TdBike.cpp $(INCLUDE) -o build/TdBike.o

server: server.o TdBike.o JSON.o
	$(CC) $(CFLAGS) build/TdBike.o build/server.o build/JSON.o $(INCLUDE) -o server

client: client.o TdBike.o JSON.o
	$(CC) $(CFLAGS) build/TdBike.o build/client.o build/JSON.o $(INCLUDE) -o client

clean:
	rm -rf *.o
	rm -rf build/*
	rm -f server echo_server
	rm -f client echo_client
