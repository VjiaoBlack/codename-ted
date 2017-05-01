/* 
 * astar.cpp
 *
 * Manages the A* implementation
 *
 * Worked on by Jigar
 *
 */

#include "astar.hpp"



using namespace std;

void astar_main() {
    gameMap map = create_random_map(); 
    vector<vec2> positions = retrieve_ship_positions(map);
    map.print_game_map(); 
}

vector<vec2> retrieve_ship_positions(gameMap gm) {
    vector<vec2> positions; 
    for(pirate p: gm.pirates) {
        positions.push_back(p.position);
    }
    for(merchant m: gm.merchants) {
        positions.push_back(m.position);
    }
    return positions; 
}


int main() {
    cout << "Staring A Star Algorithm\n";
    astar_main();
    cout << "A Star Algorithm Complete\n";
}
