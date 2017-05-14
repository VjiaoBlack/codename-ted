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

void vec3::print_vec3() {
    cout << "{" << x << ", " << y << ", " << z <<  "}\n"; 
}

void PiMapTile::print_map_tile() {
    if (is_ship == 1) {
        cout << "[x]";
    } else if (is_ship == 2) {
        cout << "[o]";
    }
    else {
        cout << "[ ]";
    }
}

void PiGameMap::print_game_map() {
    for (vector<PiMapTile> xs: mapTiles) {
        for (PiMapTile x : xs) {
            x.print_map_tile();
        }
        cout << "\n";
    }
}

// TODO what do we do with id
PiGameState::PiGameState(PiGameMap map, PiPirate Pirate,
                     unordered_map<int, PiPlayer> players,
                     unordered_map<int, PiLobby> lobbies)
    : id(0)
    , map(map.mapTiles, map.x_size, map.y_size, map.pirates, map.merchants, map.size)
    , Pirate(Pirate.pirate_name, Pirate.position, Pirate.coord_pos,
             Pirate.velocity, Pirate.orientation, Pirate.rudderRot,
             Pirate.goldAmount, Pirate.AI, Pirate.acceleration) {

}

// HELPER FUNCTIONS
bool compare_vec(vec2 a, vec2 b) {
    return (a.x == b.x && a.y == b.y);
}

PiMapTile PiMapTile::createRandomTile(int PiPirate) {
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
    if (PiPirate)
        is_ship = 2;
    else if (ship < 99)
        is_ship = 0;
    else 
        is_ship = 1;
    PiMapTile random_tile(curr_dir, curr_str, wind_dir, wind_str, is_ship);
    return random_tile;
}

PiGameMap PiGameMap::createRandomMap() {
    int x_len = 25; 
    int y_len = 25;
    int rand_i = rand() % 25;
    int rand_j = rand() % 25;
    vector< vector<PiMapTile> > tiles;
    vector<PiPirate> pirates; 
    vector<PiMerchant> merchants;
    vec3 size(250, 250, 250);
    for(int i = 0; i < x_len; i++) {
        vector<PiMapTile> curr_row; 
        for (int j = 0; j < y_len; j++) {
            if (i == rand_i && j == rand_j) {
                PiMapTile curr_tile = PiMapTile::createRandomTile(1);
                curr_row.push_back(curr_tile);
                PiPirate curr_pirate = PiPirate(vec2(i, j));
                pirates.push_back(curr_pirate);
            } else {
                PiMapTile curr_tile = PiMapTile::createRandomTile(0);
                curr_row.push_back(curr_tile);
                if (curr_tile.is_ship) {
                    PiMerchant curr_merch = PiMerchant(vec2(i, j));
                    merchants.push_back(curr_merch);
                }
            }
        }
        tiles.push_back(curr_row);
    }
    PiGameMap random_map(tiles, x_len, y_len, pirates, merchants, size);
    return random_map;
}





// TODO: comment these functions please! I'm not sure what you're trying to do!
// Felt like it'd be better to ask rather than assume.
// - Victor
void move_pirate(PiGameMap& m, vec2 pos1, vec2 pos2) {
    vector<PiPirate> pirates;
    for(PiPirate p : m.pirates) {
        if (compare_vec(p.position, pos1)) 
            p.position = pos2;
        pirates.push_back(p);
    }
    m.pirates = pirates;
    m.mapTiles[pos1.x][pos1.y].is_ship = 0;
    m.mapTiles[pos2.x][pos2.y].is_ship = 2;
}

void move_merchant(PiGameMap& map, vec2 pos1, vec2 pos2) {
    vector<PiMerchant> merchants;
    for(PiMerchant m : map.merchants) {
        if (compare_vec(m.position, pos1)) 
            m.position = pos2;
        merchants.push_back(m);
    }
    map.merchants = merchants;
    map.mapTiles[pos1.x][pos1.y].is_ship = 0;
    map.mapTiles[pos2.x][pos2.y].is_ship = 1;
}
