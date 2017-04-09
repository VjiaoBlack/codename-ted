CC=g++
CFLAGS= -std=c++11 -I/Users/Christopher/boost_1_61_0  -lboost_system

server:
	$(CC) $(CFLAGS) udp_echo_server.cpp -o echo_server

client:
	$(CC) $(CFLAGS) udp_echo_client.cpp -o echo_client
