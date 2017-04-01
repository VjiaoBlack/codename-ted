/**
 * test_driving.h
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#ifndef _TEST_DRIVING_H_
#define _TEST_DRIVING_H_

#include <SDL2/SDL.h>

#include <unordered_set>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

#define PI 3.1415926535897


class Bike {
public:
	float x, y;
	float wheel; // of front wheel
	float dir;
	float r;

	float mass;
	float vel;

	Bike(float x, float y, float mass) {
		this->x = x;
		this->y = y;
		this->mass = mass;

		wheel = 0.0;
		dir = 0.0;
		r = 50.0;
		vel = 0.0;
	}

	void update() {
		printf("vel: %f\n", vel);

		float change = 0.02 * wheel * vel;
		vel -= 0.5f * fabs(change);
		dir += change;
		wheel -= change;

		printf("dir, wheel: %f, %f\n", dir, wheel);
		this->x += vel * (sin(dir + wheel));
		this->y += vel * (cos(dir + wheel));
	}

	void draw(SDL_Renderer* renderer) {
	    SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);

	    int x1 = x;
	    int y1 = y;

	    int x2 = x - sin(dir) * r;
	    int y2 = y - cos(dir) * r;

    	SDL_RenderDrawLine(renderer, x1 * 2, y1 * 2, x2 * 2, y2 * 2);


	    SDL_SetRenderDrawColor(renderer, 0x00, 0xCC, 0x00, 0xFF);

    	int wx1 = x1 + sin(dir + wheel) * 5.0;
    	int wy1 = y1 + cos(dir + wheel) * 5.0;

    	int wx2 = x1 - sin(dir + wheel) * 5.0;
    	int wy2 = y1 - cos(dir + wheel) * 5.0;

    	SDL_RenderDrawLine(renderer, wx1 * 2, wy1 * 2, wx2 * 2, wy2 * 2);

	}
};

#endif