/**
 * test_driving.cpp
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include "TdBike.h"

int main(int argv, char* argc[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window* window;
    SDL_Renderer* renderer;


    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    /** Create window */
    window = SDL_CreateWindow("test_driving",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               1024 * 2, 768 * 2, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    /** Create renderer for window */
    renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    /** Initialize renderer color to white */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    struct timeval start, current;

    gettimeofday(&start, NULL);
    gettimeofday(&current, NULL);

	SDL_Event e;


	unordered_set<int> keysDown;
    unordered_set<char> buttonsDown;

    bool quit = false;

    TdBike bike = TdBike(200.0, 200.0, 500.0);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            /* user requests QUIT */
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                keysDown.insert(e.key.keysym.sym);
            } else if (e.type == SDL_KEYUP) {
                keysDown.erase(e.key.keysym.sym);
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                buttonsDown.insert(e.button.button);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                buttonsDown.erase(e.button.button);
            }
        }

        /** update() block for keypresses */
        if (keysDown.count(SDLK_q)) {
            quit = true;
        }


        // TODO make sure the angles don't just keep increasing infinitely
        if (keysDown.count(SDLK_LEFT)) {
        	if (bike.wheel < 1.6f) {
        		bike.wheel += 0.02;
        	}
        }
        if (keysDown.count(SDLK_RIGHT)) {
        	if (bike.wheel > -1.6f) {
        		bike.wheel -= 0.02;
        	}
        }

        if (keysDown.count(SDLK_UP)) {
        	if (bike.vel < 2.0) {
        		bike.vel += 0.01;
        	}
        } else {
        	if (bike.vel > 0.0005) {
        		bike.vel -= 0.0005;
        	} else {
                bike.vel = 0;
            }
        }

        if (keysDown.count(SDLK_DOWN)) {
        	if (bike.vel > 0.0) {
        		bike.vel -= 0.0095;
        	}
        }

        /** Clear screen */
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	    SDL_RenderClear(renderer);

	    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	    SDL_Rect wholeRect = {25 * 2, 25 * 2, (1024 - 50) * 2, (768 - 50) * 2};
    	SDL_RenderDrawRect(renderer, &wholeRect);

    	bike.update();
    	bike.draw(renderer);

	    /** Update screen */
	    SDL_RenderPresent(renderer);

	    /** Update the surface */
	    SDL_UpdateWindowSurface(window);

        // usleep(3000);
    }

    return 0;
}