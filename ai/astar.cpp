/* 
 * astar.cpp
 *
 * Manages the A* implementation
 *
 * Worked on by Jigar
 *
 */

#include "astar.hpp"
#include <math.h>

using namespace std;

void astar_main() {
    gameMap map = create_random_map();
    priority_queue<q_elem, vector<q_elem>, PriorityComp> main_q; 

    vector<vec2> positions = retrieve_ship_positions(map);
    vec2 pirate_pos = positions[0]; // Hacky way to get pirate position
    vector<vec2> next_positions = retrieve_next_positions(pirate_pos, 25, 25);
    for (vec2 pos : next_positions) {
        q_elem new_elem(pos, 0);
        main_q.push(new_elem);
    }
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

// Basic distance, add better physics later
float distance(vec2 pos1, vec2 pos2) {
    float delta_x = pos1.x - pos2.x; 
    float delta_y = pos2.y - pos2.y;
    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

int main() {
    cout << "Staring A Star Algorithm\n";
    astar_main();
    cout << "A Star Algorithm Complete\n";
}
