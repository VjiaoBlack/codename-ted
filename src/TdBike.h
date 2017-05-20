/**
 * TdBike.h
 *
 * hopefully soon defunkt
 *
 * Victor Jiao
 */




#include "PiratePanic.h"

#ifndef _TD_BIKE_H_
#define _TD_BIKE_H_

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
    void update() {
        // printf("%f\n", vel);
        float change = 0.02 * (wheel) * fabs(vel);
        vel -= 0.5f * fabs(change);
        dir += change;
        wheel -= change;

        float vx = vel * sin(dir + wheel);
        float vy = vel * cos(dir + wheel);

        x += vx;
        y += vy;
    }

    /**
     * process input
     */
   	void processInput(OIS::Keyboard* keyboard) {
	    if (keyboard->isKeyDown(OIS::KC_J)) {
	        if (wheel < 1.6f) {
	            wheel += 0.02;
	        }
	    }
	    
	    if (keyboard->isKeyDown(OIS::KC_L)) {
	        if (wheel > -1.6f) {
	            wheel -= 0.02;
	        }
	    }

	    if (keyboard->isKeyDown(OIS::KC_I)) {
	        if (vel < 4.0) {
	            vel += 0.04;
	        }
	    } else {
	        if (vel > 0.005) {
	            vel -= 0.005;
	        } else {
	            vel = 0;
	        }
	    }

	    if (keyboard->isKeyDown(OIS::KC_K)) {
	        if (vel > 0.0) {
	            vel -= 0.0195;
	        }
	    }
   	}
};

#endif