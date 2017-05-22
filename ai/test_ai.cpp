/* 
 * test_ai.cpp
 *
 * Tests AI with SDL graphics 
 *
 * Worked on by Jigar
 *
 */

#include <SDL2/SDL.h>
#include "astar.hpp"
#include <unordered_set>
#include <sys/time.h>

using namespace std;

int main() {
    cout << "test_ai called\n";
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

    while(!quit) {
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

        /** Clear screen */
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	    SDL_RenderClear(renderer);

	    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	    SDL_Rect wholeRect = {25 * 2, 25 * 2, (1024 - 50) * 2, (768 - 50) * 2};
    	SDL_RenderDrawRect(renderer, &wholeRect);

	    /** Update screen */
	    SDL_RenderPresent(renderer);

	    /** Update the surface */
	    SDL_UpdateWindowSurface(window);

        // usleep(3000);
    }
    return 0;
}
