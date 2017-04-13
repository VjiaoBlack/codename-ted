# Set BOOST_ROOT globally somwhere, like in .profile or .bash_profile. You could also just set it temproarly on the command line

CC=g++
CFLAGS= -std=c++11 -I/$(BOOST_ROOT)  -L/$(BOOST_ROOT)/stage/lib -lboost_system

all: server client

all_echo: echo_server udp_echo_client

server:
	$(CC) $(CFLAGS) server.cpp -o server

client:
	$(CC) $(CFLAGS) client.cpp -o client

echo_server:
	$(CC) $(CFLAGS) udp_echo_server.cpp -o echo_server

echo_client:
	$(CC) $(CFLAGS) udp_echo_client.cpp -o echo_client

clean:
	rm -rf *.o
	rm -f server echo_server
	rm -f clieant echo_client
