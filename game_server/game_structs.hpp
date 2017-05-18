/*
 *
 * game_structs.hpp - codename-ted (2017)
 *
 * Definitions for necessary structs
 * including player, lobby,PiGameState, etc
 *
 * Worked on by Jigar, Charlie, Chris, David, and Victor
 *
 * 5/14 - significant code structure overhaul - Victor
 *
 */

// TODO rename variable names to be more clear

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <vector>

#ifndef _GAME_STRUCTS_HPP_
#define _GAME_STRUCTS_HPP_

using namespace std;

// CLASSES
class vec2 {
public:
    // Attributes
    float x, y;
    // Constructor
    vec2(float a, float b) : x(a), y(b) {};
    // Methods
    void print_vec2();

    float Length();
 
    // Normalizes the vector
    vec2 Normalize();
};

class vec3 {
public:
    //Attributes

    float x, y, z;
    //Constructor
    vec3(float a, float b, float c) : x(a), y(b), z(c) {};
    // Methods
    void print_vec3();
};

class PiPlayer {
public:
    int uID;
    float x;
    float y;
    bool registered;
    string name;
};

class PiShip {
public:
    //Attributes
    vec2 position; // map tile position
    vec2 coord_pos; // coordinate position
    vec2 velocity;
    vec2 acceleration;
    float orientation;
    float rudderRot;
    int goldAmount;
    bool AI;
    // Constructor
    PiShip(vec2 a, vec2 b, vec2 c, float d,
            float e, int f, bool g, vec2 h) :
            position(a), velocity(b), acceleration(c),
            orientation(d), rudderRot(e), goldAmount(f),
            AI(g), coord_pos(h) {};
    // Methods
};

class PiPirate : public PiShip {
private:

public:
    // Attributes
    string pirate_name;
    // Constructor
    // TODO construct map tile position from coordinate position
    PiPirate(string a, vec2 b, vec2 c, vec2 d,
             float e, float f, int g, bool h, vec2 i)
        : pirate_name(a)
        , PiShip(b, c, d, e, f, g, h, i) {
    };
    PiPirate(vec2 loc) : PiPirate("Vader", loc, vec2(0,0), vec2(0,0), 0, 0, 0, true, vec2(0,0)) {}
    PiPirate() : PiPirate(vec2(0,0)) {}
};

class PiMerchant : public PiShip {
public:
    // Attributes
    string merchant_name;
    // Constructor
    PiMerchant(string a, vec2 b, vec2 c, vec2 d,
               float e, float f, int g, bool h, vec2 i)
        : merchant_name(a)
        , PiShip(b, c, d, e, f, g, h, i) {};
    PiMerchant(vec2 loc) 
        : PiMerchant("Clone", loc, vec2(0,0), vec2(0,0), 0, 0, 0, true, vec2(0,0)) {}
    PiMerchant() : PiMerchant(vec2(0,0)) {} 
};

class PiLobby {
public:
    int uID;
    unordered_map<int, PiPlayer> players;
};

class PiMapTile {
public:
    // ATTRIBUTES
    vec2 currentDirection;
    float currentStrength;
    vec2 windDirection;
    float windStrength;
    // Convert these to enums later
    int is_ship; // 2 if PiPirate, 1 if PiMerchant, 0 if nothing
    int start_finish; // 1 if start, 0 if finish
    int land_water; // 1 if land, 0 if water

    // CONSTRUCTOR
    PiMapTile(vec2 a, float b, vec2 c, float d, int e)
        : currentDirection(a)
        , currentStrength(b)
        , windDirection(a)
        , windStrength(b)
        , is_ship(e) {};

    PiMapTile() : PiMapTile(vec2(0,0), 0, vec2(0,0), 0, 0) {};

    static PiMapTile createRandomTile(int PiPirate);
    // METHODS
    void print_map_tile();
};

class PiGameMap {
public:
    // ATTRIBUTES
    vector<vector<PiMapTile> > mapTiles;
    int x_size; // for tiles
    int y_size; // for tiles
    vector<PiPirate> pirates;
    vector<PiMerchant> merchants;
    vec3 size; // Charlie, pls confirm this is for coord

    // CONSTRUCTOR
    PiGameMap(vector< vector<PiMapTile> > a, int b, int c,
              vector<PiPirate> d, vector <PiMerchant> e, vec3 f) 
        : mapTiles(a)
        , x_size(b)
        , y_size(c)
        , pirates(d)
        , merchants(e)
        , size(f) {};

    // TODO size in num tiles I suppose??
    // Jigar: I didn't create it, but I was assuming its coord size.
    PiGameMap(int sz)
        : x_size(sz)
        , y_size(sz)
        , size(sz, sz, sz) {
        vec3 size(sz, sz, sz);
        for(int i = 0; i < sz; i++) {
            vector<PiMapTile> curr_row; 
            for (int j = 0; j < sz; j++) {
                curr_row.push_back(PiMapTile());
            }
            mapTiles.push_back(curr_row);
        }
    }
    PiGameMap() : PiGameMap(25) {};

    static PiGameMap createRandomMap();

    // METHODS
    void print_game_map();
};

class PiGameState {
public:
    long id;
    PiGameMap map;
    PiPirate Pirate;
    unordered_map<int, PiPlayer> players;
    unordered_map<int, PiLobby> lobbies;
    float **heightMap;

    PiGameState(PiGameMap map, PiPirate Pirate,
                unordered_map<int, PiPlayer> players,
                unordered_map<int, PiLobby> lobbies);

    PiGameState()
        : id(0)
        , map()
        , Pirate()
        , players()
        , lobbies()
        , heightMap(NULL) {}
};

bool compare_vec(vec2 a, vec2 b);
void move_pirate(PiGameMap& m, vec2 pos1, vec2 pos2);
void move_merchant(PiGameMap& map, vec2 pos1, vec2 pos2);
void shift_pirate(PiGameMap& p, vec2 coord1, vec2 coord2);
void shift_merchant(PiGameMap& m, vec2 coord1, vec2 coord2);
vec2 convert_coord_tile(PiGameMap& m, vec2 coord); 

#endif
