/**
 * TdBike.cpp
 *
 * Victor Jiao
 *
 * Holds information about the simulator bike.
 */

#include "TdBike.h"

void TdBike::update() {
	printf("%f\n", vel);
	float change = 0.02 * (wheel) * fabs(vel);
	vel -= 0.5f * fabs(change);
	dir += change;
	wheel -= change;

	float vx = vel * sin(dir + wheel);
	float vy = vel * cos(dir + wheel);

	/** check if head of bike is in bounds */
	if (x + vx >= 25.0 && x + vx <= 1024.0 - 25.0) {
		x += vx;
	} else {
		vel = fabs(vy);
	}

	if (y + vy >= 25.0 && y + vy <= 768.0 - 25.0) {
		y += vy;
	} else {
		vel = fabs(vx);
	}

	printf("%f, %f\n", x, y);
}

void TdBike::draw(SDL_Renderer* renderer) {
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
