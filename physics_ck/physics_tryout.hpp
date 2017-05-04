#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

gamestate main_loop(unordered_map<string, vector<string> > input_object, gamestate gameState);
void print_boat(gameMap gmap);