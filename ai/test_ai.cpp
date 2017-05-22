/* 
 * test_ai.cpp
 *
 * Tests AI with SDL graphics 
 *
 * Worked on by Jigar
 *
 */

#include "astar.hpp"

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

    return 0;
}
