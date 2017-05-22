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
#include "../game_server/game_structs.hpp"
#include <unistd.h>

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
    window = SDL_CreateWindow("test_ai",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               600, 600, SDL_WINDOW_SHOWN);
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

    // Create Game Map
    PiGameMap map = PiGameMap::createRandomMap();

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

	    SDL_Rect wholeRect = {25 * 2, 25 * 2, 500, 500};
    	SDL_RenderDrawRect(renderer, &wholeRect);

        /** Update Game Map (Basic Physics Engine) */ 
        run_astar(map);
        for(PiPirate p: map.pirates) {
            float new_x = p.coord_pos.x + p.velocity.x;
            float new_y = p.coord_pos.y + p.velocity.y; 
            shift_pirate(map, p.coord_pos, vec2(new_x, new_y));   
        }  

        /** Draw Game Map */ 
        for(PiPirate p : map.pirates) {
    	    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect pirateRect = {50 + (int)p.coord_pos.x, 
                                    50 + (int)p.coord_pos.y, 10, 10}; 
    	    SDL_RenderDrawRect(renderer, &pirateRect);
        }

        for(PiMerchant m: map.merchants) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_Rect merchantRect = {50 + (int)m.coord_pos.x, 
                                    50 + (int)m.coord_pos.y, 10, 10}; 
            SDL_RenderDrawRect(renderer, &merchantRect);
        }

	    /** Update screen */
	    SDL_RenderPresent(renderer);

	    /** Update the surface */
	    SDL_UpdateWindowSurface(window);

        usleep(20000);
    }
    return 0;
}
