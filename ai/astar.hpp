#include <vector>
#include "../game_server/game_structs.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

// DATA STRUCTURES 
typedef struct loc {
    int x; 
    int y; 
} loc; 

// FUNCTION HEADERS
void astar_main(); 
vector<loc> retrieve_ship_positions(); 
     


