#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

// DATA STRUCTURES 

// FUNCTION HEADERS
void astar_main(); 
vector<vec2> retrieve_ship_positions(gameMap gm); 
vector<vec2> retrieve_next_positions(vec2 curr, int max_x, int max_y);
float distance(vec2 pos1, vec2 pos2);

