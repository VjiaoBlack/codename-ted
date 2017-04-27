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
    map.print_game_map(); 
}


int main() {
    cout << "Staring A Star Algorithm\n";
    astar_main();
    cout << "A Star Algorithm Complete\n";
}
