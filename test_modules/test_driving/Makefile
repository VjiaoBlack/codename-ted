all: test_driving

test_driving: src/test_driving.cpp src/test_driving.h
	g++ -std=gnu++11 src/test_driving.cpp `pkg-config --cflags --libs sdl2` -o test_driving