# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS= -std=c++11 -I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system

all: server client

server:
	$(CC) $(CFLAGS) udp_echo_server.cpp -o echo_server

client:
	$(CC) $(CFLAGS) udp_echo_client.cpp -o echo_client
