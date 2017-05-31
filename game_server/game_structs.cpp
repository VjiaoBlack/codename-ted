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
#include <math.h>

using namespace std;

// CLASS METHODS
void vec2::print_vec2() {
    cout << "{" << x << ", " << y << "}\n";
}

float vec2::Length(){
    return sqrt(x * x + y * y);
}

// Normalizes the vector
vec2 vec2::Normalize(){
    float norm_x;
    float norm_y;

    float length = this->Length();

    if(length != 0){
        norm_x = x/length;
        norm_y = y/length;
    }

    vec2 vector = vec2(norm_x, norm_y);

    return vector;
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

void PiGameState::add_player(int uID, float x,
        float y, bool registered, string name) {
    PiPlayer player = PiPlayer(uID, x, y, registered, name);
    players.insert(make_pair(uID, player));
    map.add_merchant(PiMerchant(name));

}

void PiGameState::add_pirate(float x, float y) {
    PiPirate pir = PiPirate(vec2(x, y));
    Pirate = pir;
    map.pirates.push_back(pir);
}

void PiGameMap::add_merchant(PiMerchant merchant) {
    merchants.push_back(merchant);
}

// HELPER FUNCTIONS

// Checks if two vectors are the same
bool compare_vec(vec2 a, vec2 b) {
    return (a.x == b.x && a.y == b.y);
}

// Creates a random tile, with placements for pirates, etc.
PiMapTile PiMapTile::createRandomTile(int PiPirate) {
    float wdx = rand() % 5;
    float wdy = rand() % 5;
    float cdx = rand() % 5;
    float cdy = rand() % 5;
    float ws = rand() % 10;
    float cs = rand() % 10;
    int ship = rand() % 100;
    int land = rand() % 100;
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
    int is_land;
    is_land = land < 99 ? 0 : 1;
    PiMapTile random_tile(curr_dir, curr_str, wind_dir,
                            wind_str, is_ship, is_land);
    return random_tile;
}

// Converts a coordinate position to a tile position
vec2 convert_coord_tile(PiGameMap& m, vec2 coord) {
    float x_ratio = m.x_size / m.size.x;
    float y_ratio = m.y_size / m.size.y;
    vec2 tile = vec2(int(x_ratio * coord.x), int(y_ratio * coord.y));
    return tile;
}
// Converts a tile position to a coord position
vec2 convert_tile_coord(PiGameMap& m, vec2 pos) {
    float x_ratio = m.size.x / m.x_size;
    float y_ratio = m.size.y / m.y_size;
    vec2 tile = vec2(x_ratio * pos.x, y_ratio * pos.y);
    return tile;
}

// Creates a randomly generated map, 25 tiles by 25 tiles
PiGameMap PiGameMap::createRandomMap() {
    int x_len = 25;
    int y_len = 25;
    int rand_i = rand() % 25;
    int rand_j = rand() % 25;
    vector< vector<PiMapTile> > tiles;
    vector<PiPirate> pirates;
    vector<PiMerchant> merchants;
    vec3 size(500, 500, 500);
    for(int i = 0; i < x_len; i++) {
        vector<PiMapTile> curr_row;
        for (int j = 0; j < y_len; j++) {
            if (i == rand_i && j == rand_j) {
                PiMapTile curr_tile = PiMapTile::createRandomTile(1);
                curr_row.push_back(curr_tile);
                int c_x = i * 500/25;
                int c_y = j * 500/25;
                PiPirate curr_pirate = PiPirate(vec2(i, j), vec2(c_x, c_y));
                pirates.push_back(curr_pirate);
            } else {
                PiMapTile curr_tile = PiMapTile::createRandomTile(0);
                curr_row.push_back(curr_tile);
                if (curr_tile.is_ship) {
                    int c_x = i * 500/25;
                    int c_y = j * 500/25;
                    PiMerchant curr_merch = PiMerchant(vec2(i, j), vec2(c_x, c_y));
                    merchants.push_back(curr_merch);
                }
            }
        }
        tiles.push_back(curr_row);
    }
    PiGameMap random_map(tiles, x_len, y_len, pirates, merchants, size);
    return random_map;
}

// Creates the starting game map for the game.
PiGameMap PiGameMap::createStartMap(int x_len, 
        int y_len, int map_size) {
    int rand_i = rand() % 25;
    int rand_j = rand() % 25;
    vector< vector<PiMapTile> > tiles;
    vector<PiPirate> pirates;
    vector<PiMerchant> merchants;
    vec3 size(map_size, map_size, map_size);
    for(int i = 0; i < x_len; i++) {
        vector<PiMapTile> curr_row;
        for (int j = 0; j < y_len; j++) {
            if (i == rand_i && j == rand_j) {
                PiMapTile curr_tile = PiMapTile();
                curr_row.push_back(curr_tile);
                int c_x = i * map_size/x_len;
                int c_y = j * map_size/y_len;
                PiPirate curr_pirate = PiPirate(vec2(i, j), vec2(c_x, c_y));
                pirates.push_back(curr_pirate);
            } else {
                PiMapTile curr_tile = PiMapTile();
                curr_row.push_back(curr_tile);
                if (curr_tile.is_ship) {
                    int c_x = i * map_size/x_len;
                    int c_y = j * map_size/y_len;
                    PiMerchant curr_merch = PiMerchant(vec2(i, j), vec2(c_x, c_y));
                    merchants.push_back(curr_merch);
                }
            }
        }
        tiles.push_back(curr_row);
    }
    PiGameMap random_map(tiles, x_len, y_len, pirates, merchants, size);
    return random_map;
}


// Shifts pirate from one coordinate location to another
void shift_pirate(PiGameMap& map, vec2 coord1, vec2 coord2) {
    if (coord2.x < 1 || coord2.x > map.size.x - 1 ||
        coord2.y < 1 || coord2.y > map.size.y - 1)
        return;
    vector<PiPirate> pirates;
    for(PiPirate p : map.pirates) {
        if(compare_vec(p.coord_pos, coord1)) {
            p.coord_pos = coord2;
            p.position = convert_coord_tile(map, coord2);
        }
        pirates.push_back(p);
    }
    map.pirates = pirates;
}

// Shifts merchant from one coordinate location to another
void shift_merchant(PiGameMap& map, vec2 coord1, vec2 coord2) {
    if (coord2.x < 20 || coord2.x > map.size.x - 20 ||
        coord2.y < 20 || coord2.y > map.size.y - 20)
        return;
    vector<PiMerchant> merchants;
    for(PiMerchant m : map.merchants) {
        if (compare_vec(m.coord_pos, coord1)) {
            m.coord_pos = coord2;
            m.position = convert_coord_tile(map, coord2);
        }
        merchants.push_back(m);
    }
    map.merchants = merchants;
}

// Moves pirate from one tile to another in maptile view
// Called within the shift functions
void move_pirate(PiGameMap& m, vec2 pos1, vec2 pos2) {
    vector<PiPirate> pirates;
    for(PiPirate p : m.pirates) {
        if (compare_vec(p.position, pos1)) // hacky
            p.position = pos2;
        pirates.push_back(p);
    }
    m.pirates = pirates;
    m.mapTiles[pos1.x][pos1.y].is_ship = 0;
    m.mapTiles[pos2.x][pos2.y].is_ship = 2;
}

// Moves merchant from one tile to another on maptile view
// Called within the shift functions
void move_merchant(PiGameMap& map, vec2 pos1, vec2 pos2) {
    vector<PiMerchant> merchants;
    for(PiMerchant m : map.merchants) {
        if (compare_vec(m.position, pos1)) // hacky
            m.position = pos2;
        merchants.push_back(m);
    }
    map.merchants = merchants;
    map.mapTiles[pos1.x][pos1.y].is_ship = 0;
    map.mapTiles[pos2.x][pos2.y].is_ship = 1;
}
