#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
#include <math.h>
using namespace std;

PiGameState compute_gamestate(unordered_map<string, vector<string> > input_object, PiGameState gameState);
void print_boat(PiGameMap gmap);
