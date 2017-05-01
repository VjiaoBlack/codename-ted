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

vector<vec2> retrieve_next_positions(vec2 curr, int max_x, int max_y) {
    vector<vec2> next; 
    int x; 
    int y; 
    for (x = -1; x < 2; x++) {
        for(y = -1; y < 2; y++) {
            int new_x = curr.x + x; 
            int new_y = curr.y + y; 
            if (new_x > 0 && new_y > 0 && 
                new_x < max_x && new_y < max_y && 
                (new_x != curr.x || new_y != curr.y)) {
                next.push_back(vec2(new_x, new_y));        
            }
        }
    }
    return next; 
}


int main() {
    cout << "Staring A Star Algorithm\n";
    astar_main();
    cout << "A Star Algorithm Complete\n";
}
