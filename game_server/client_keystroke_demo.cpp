#include "client.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

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
        // Create window
        window = SDL_CreateWindow("client keystroke demo",
           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
           1024 * 1, 768 * 1, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
            exit(1);
        }

        // Create renderer for window
        renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
            exit(1);
        }

        // Initialize renderer color to white
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Update screen
        SDL_RenderPresent(renderer);

        // Update the surface
        SDL_UpdateWindowSurface(window);

        SDL_Event e;

        unordered_set<int> keysDown;
        unordered_set<char> buttonsDown;

        bool quit = false;

        boost::asio::io_service io_service;
        TCPClient registration_client(io_service, argv[1], argv[2]);
        UDPClient loop_client(io_service, argv[1], argv[2], 0);

        if (argc == 3) {
            std::cout << "register called!" << std::endl;
            int player_id = registration_client.register_player();
            std::cout << "loop called!" << std::endl;

            while(!quit) {
                vector<int> keystrokes;

                while (SDL_PollEvent(&e) != 0) {
                    // user requests QUIT
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

                // update() block for keypresses
                if (keysDown.count(SDLK_q)) {
                    quit = true;
                }

                // TODO make sure the angles don't just keep increasing infinitely
                if (keysDown.count(SDLK_LEFT)) {
                    keystrokes.push_back(KC_J);
                }
                if (keysDown.count(SDLK_RIGHT)) {
                    keystrokes.push_back(KC_L);
                }

                if (keysDown.count(SDLK_UP)) {
                    keystrokes.push_back(KC_I);
                }

                if (keysDown.count(SDLK_DOWN)) {
                    keystrokes.push_back(KC_K);
                }

                loop_client.send_keystrokes(keystrokes);
                //loop_client.get_gamestate();
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
