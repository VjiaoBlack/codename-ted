/* 
 *
 * game_structs.cpp - codename-ted (2017)
 *
 * Implementations of Class Methods in game_structs.hpp as 
 * well as other useful helper functions. 
 *
 * Worked on by Jigar
 *
 */

#include "game_structs.hpp"
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

// CLASS METHODS 

void vec2::print_vec2() {
    cout << "{" << x << ", " << y << "}\n"; 
} 

void mapTile::print_map_tile() {
    if (is_ship == 1) {
        cout << "[x]";
    } else if (is_ship == 2) {
        cout << "[o]";
    } 
    else {
        cout << "[ ]";
    } 
}  

void gameMap::print_game_map() {
    for (vector<mapTile> xs: mapTiles) {
        for (mapTile x : xs) { 
            x.print_map_tile();
        }
        cout << "\n";
    } 
}


// HELPER FUNCTIONS
bool compare_vec(vec2 a, vec2 b) {
    return (a.x == b.x && a.y == b.y);
}

pirate create_basic_pirate(vec2 loc) {
    string name = "Vader";
    vec2 position = loc; 
    vec2 velocity(0, 0);
    vec2 acceleration(0, 0);
    float orientation = 0;
    float rudderRot = 0; 
    int goldAmount = 0; 
    bool AI = true; 
    pirate basic(name, position, velocity, acceleration, 
                orientation, rudderRot, goldAmount, AI);
    return basic; 
}

merchant create_basic_merchant(vec2 loc) {
    string name = "Clone";
    vec2 position = loc; 
    vec2 velocity(0, 0);
    vec2 acceleration(0, 0);
    float orientation = 0;
    float rudderRot = 0; 
    int goldAmount = 0; 
    bool AI = true; 
    merchant basic(name, position, velocity, acceleration, 
                orientation, rudderRot, goldAmount, AI);
    return basic; 
}


mapTile create_blank_tile() {
    vec2 wind_dir = vec2(0, 0);
    vec2 curr_dir = vec2(0, 0);
    float wind_str = 0; 
    float curr_str = 0; 
    int is_ship = 0;
    mapTile blank_tile(curr_dir, curr_str, wind_dir, wind_str, is_ship);
    return blank_tile;
} 

mapTile create_random_tile(int pirate) {
    float wdx = rand() % 5; 
    float wdy = rand() % 5; 
    float cdx = rand() % 5; 
    float cdy = rand() % 5; 
    float ws = rand() % 10; 
    float cs = rand() % 10;
    int ship = rand() % 100;
    vec2 wind_dir = vec2(wdx, wdy);
    vec2 curr_dir = vec2(cdx, cdy);
    float wind_str = ws; 
    float curr_str = cs;
    int is_ship;
    if (pirate)
        is_ship = 2;
    else if (ship < 99)
        is_ship = 0;
    else 
        is_ship = 1;
    mapTile random_tile(curr_dir, curr_str, wind_dir, wind_str, is_ship);
    return random_tile;
}

gameMap create_blank_map() {
    int x_len = 25; 
    int y_len = 25;
    vector< vector<mapTile> > tiles;
    vector<pirate> pirates; 
    vector<merchant> merchants;
    for(int i = 0; i < x_len; i++) {
        vector<mapTile> curr_row; 
        for (int j = 0; j < y_len; j++) {
            curr_row.push_back(create_blank_tile());
        }
        tiles.push_back(curr_row);
    }
    gameMap blank_map(tiles, x_len, y_len, pirates, merchants);
    return blank_map;;
}

gameMap create_random_map() {
    int x_len = 25; 
    int y_len = 25;
    int rand_i = rand() % 25;
    int rand_j = rand() % 25;
    vector< vector<mapTile> > tiles;
    vector<pirate> pirates; 
    vector<merchant> merchants;
    for(int i = 0; i < x_len; i++) {
        vector<mapTile> curr_row; 
        for (int j = 0; j < y_len; j++) {
            if (i == rand_i && j == rand_j) {
                mapTile curr_tile = create_random_tile(1);
                curr_row.push_back(curr_tile);
                pirate curr_pirate = create_basic_pirate(vec2(i, j));
                pirates.push_back(curr_pirate);
            } else {
                mapTile curr_tile = create_random_tile(0);
                curr_row.push_back(curr_tile);
                if (curr_tile.is_ship) {
                    merchant curr_merch = create_basic_merchant(vec2(i, j));
                    merchants.push_back(curr_merch);
                }
            }
        }
        tiles.push_back(curr_row);
    }
    gameMap random_map(tiles, x_len, y_len, pirates, merchants);
    return random_map;;   
}

