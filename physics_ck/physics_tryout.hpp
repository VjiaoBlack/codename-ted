#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <math.h>

#include <SDL2/SDL.h>
#include <unordered_set>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

using namespace std;

PiGameState compute_gamestate(unordered_map<string, vector<string> > input_object, PiGameState gameState);
void print_boat(PiGameMap gmap);
