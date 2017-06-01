# Makefile
# 
# Victor Jiao
# 
# the Makefile for the project.

COMPILE=g++ -std=gnu++11 -Winline -Wall -O3

LINK=-I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image

all: main

main: build convert

build:
	mkdir build

clean: 
	rm -rf build
	rm convert


convert: convert.cpp
	$(COMPILE) convert.cpp $(LINK) -o convert

