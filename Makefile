all: game

game: src/game.cpp src/game.h
	g++ src/game.cpp -o game