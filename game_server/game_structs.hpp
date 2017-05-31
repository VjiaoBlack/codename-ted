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
    vec2(float x_pos, float y_pos) : 
        x(x_pos), y(y_pos) {};
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
    vec3(float x_pos, float y_pos, float z_pos) : 
        x(x_pos), y(y_pos), z(z_pos) {};
    // Methods
    void print_vec3();
};

class PiPlayer {
public:
    int uID; // index of the player in the list of merchants
    float x; // don't use
    float y; // don't use
    // bool registered;
    bool is_pirate;
    string name;
    PiPlayer(int id, float x_pos, float y_pos,
            bool pir, string nm) :
            uID(id), x(x_pos), y(y_pos),
            is_pirate(pir), name(nm) {};
    PiPlayer()
        : PiPlayer(0, 0.0, 0.0, false, "Jigar"){};
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
    // Constructor (Note that coord_pos is last)
    PiShip(vec2 pos, vec2 vel, vec2 acc, float orien,
            float rud, int gold, bool ai, vec2 cpos) :
            position(pos), velocity(vel), acceleration(acc),
            orientation(orien), rudderRot(rud), goldAmount(gold),
            AI(ai), coord_pos(cpos) {};

};

class PiPirate : public PiShip {
private:

public:
    // Attributes
    string pirate_name;
    // Constructor
    PiPirate(string pname, vec2 pos, vec2 vel, vec2 acc,
             float orien, float rud, int gold, 
             bool ai, vec2 cpos)
        : pirate_name(pname)
        , PiShip(pos, vel, acc, orien, rud, gold, 
                ai, pos) {
    };
    // BAD CONSTRUCTOR - STOP USING EVENTUALLY
    PiPirate(vec2 loc) : PiPirate("Vader",
            loc,
            loc,
            vec2(0,0),
            0, 0, 0, true,
            vec2(0,0)) {}
    // MAKE SURE COORD IS IN LOC!!
    PiPirate(vec2 loc, vec2 coord) : PiPirate("Vader",
            loc,
            vec2(-1,-1),
            vec2(0,0),
            0, 0, 0, true,
            coord) {}
    PiPirate() : PiPirate(vec2(0,0)) {}

};

class PiMerchant : public PiShip {
public:
    // Attributes
    string merchant_name;
    // Constructor
    PiMerchant(string mname, vec2 pos, vec2 vel, vec2 acc,
               float orien, float rud, int gold, 
               bool ai, vec2 cpos)
        : merchant_name(mname)
        , PiShip(pos, vel, acc, orien, rud, gold, ai, cpos) {};

    // BAD CONSTRUCTOR - STOP USING EVENTUALLY
    PiMerchant(vec2 loc)
        : PiMerchant("Clone", loc, vec2(0,0), vec2(0,0), 0, 0, 0, true, vec2(0,0)) {}

    // MAKE SURE COORD IS IN LOC!!
    PiMerchant(vec2 loc, vec2 coord)
        : PiMerchant("Clone", loc, vec2(-1,1), vec2(0,0), 0, 0, 0, true, coord) {}
    PiMerchant() : PiMerchant(vec2(0,0)) {}

    // Takes in a merchant name
    PiMerchant(string name)
    : PiMerchant(name, vec2(0,0), vec2(-1,1), vec2(0,0), 0, 0, 0, true, vec2(0,0)) {}
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
    PiMapTile(vec2 cdir, float cstr, vec2 wdir, float wstr, 
            int ship, int land)
        : currentDirection(cdir)
        , currentStrength(cstr)
        , windDirection(wdir)
        , windStrength(wstr)
        , is_ship(ship)
        , land_water(land) {};

    PiMapTile() : PiMapTile(vec2(0,0), 0, vec2(0,0), 0, 0, 0) {};

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
    vec3 size; // this is for coord

    // CONSTRUCTOR
    PiGameMap(vector< vector<PiMapTile> > tiles, int x, int y,
              vector<PiPirate> pirs, vector <PiMerchant> merchs, vec3 sz)
        : mapTiles(tiles)
        , x_size(x)
        , y_size(y)
        , pirates(pirs)
        , merchants(merchs)
        , size(sz) {};

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
    static PiGameMap createStartMap(int x_len, int y_len, int map_size);

    // METHODS
    void print_game_map();
    void add_merchant(PiMerchant merchant);
};

class PiGameState {
public:
    // Attributes
    long id;
    PiGameMap map;
    PiPirate Pirate;
    unordered_map<int, PiPlayer> players;
    unordered_map<int, PiLobby> lobbies;
    float **heightMap;

    // Constructors
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

    // Methods
    void add_player(int uID, float x, float y, bool registered, string name);
    void add_pirate(float x, float y);

};

bool compare_vec(vec2 a, vec2 b);
void move_pirate(PiGameMap& m, vec2 pos1, vec2 pos2);
void move_merchant(PiGameMap& map, vec2 pos1, vec2 pos2);
void shift_pirate(PiGameMap& p, vec2 coord1, vec2 coord2);
void shift_merchant(PiGameMap& m, vec2 coord1, vec2 coord2);
vec2 convert_coord_tile(PiGameMap& m, vec2 coord);
vec2 convert_tile_coord(PiGameMap& m, vec2 pos);

#endif
