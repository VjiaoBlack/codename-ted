/**
 * TdBike.h
 *
 * Victor Jiao
 *
 * The simulator bike object.
 */

#ifndef _TD_BIKE_H_
#define _TD_BIKE_H_

#include <SDL2/SDL.h>

#include <unordered_set>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

#define PI 3.1415926535897

class TdBike {
public:
	float x, y;
	float wheel; // of front wheel
	float dir;
	float r;

	float mass;
	float vel;

	TdBike(float x, float y, float mass) {
		this->x = x;
		this->y = y;
		this->mass = mass;

		wheel = 0.0;
		dir = 0.0;
		r = 40.0;
		vel = 0.0;
	}

	/** 
	 * update
	 * updates physics of bike
	 */
	void update();

	/**
	 * draw
	 * @param renderer The SDL game renderer
	 */
	void draw(SDL_Renderer* renderer);
};

#endif