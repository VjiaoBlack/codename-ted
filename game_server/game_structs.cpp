#include "game_structs.hpp"
#include <vector>
#include <iostream>

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


using namespace std;

// CLASS METHODS 

void vec2::print_vec2() {
    cout << "{" << x << ", " << y << "}"; 
} 


// HELPER FUNCTIONS
mapTile create_blank_tile() {
    vec2 wind_dir = vec2(0, 0);
    vec2 curr_dir = vec2(0, 0);
    float wind_str = 0; 
    float curr_str = 0; 
    int is_ship = 0;
    mapTile blank_tile(curr_dir, curr_str, wind_dir, wind_str, is_ship);
    return blank_tile;
} 

gameMap create_blank_map() {
    int x_len = 500; 
    int y_len = 500;
    vector< vector<mapTile> > tiles;
    for(int i = 0; i < x_len; i++) {
        vector<mapTile> curr_row; 
        for (int j = 0; j < y_len; j++) {
            curr_row.push_back(create_blank_tile());
        }
        tiles.push_back(curr_row);
    }
    gameMap blank_map(tiles, x_len, y_len);
    return blank_map;;
}  


