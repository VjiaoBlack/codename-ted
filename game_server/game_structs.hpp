/* 
 *
 * game_structs.hpp - codename-ted (2017)
 *
 * Definitions for necessary structs
 * including player, lobby, gamestate, etc
 *
 * Worked on by Chris, David, Victor, Jigar, and Charlie
 *
 */

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

// Adding this in so it doesn't break code for right now. 
class vec2 {
    public: 
        float x, y;
};

class player_t {
    public:
        int uID;
	    float x;
	    float y;
	    bool registered;
	    string name;
};

class ship {
    public:
        vec2 position;
        vec2 velocity;
        vec2 acceleration;
        float orientation;
        float rudderRot;
        //Angular velocity?
        int goldAmount;
        bool AI;
};

class pirate {
    public:
        vec2 position;
        vec2 velocity;
        vec2 acceleration;
        float orientation;
        float rudderRot;
        //Angular velocity?
        int goldAmount;
        bool AI;
};

class lobby_t {
    public:
        int uID;
        unordered_map<int, player_t> players;
};

class mapTile {
    public:   
        vec2 currentDirection;
        float currentStrength;
        vec2 windDirection;
        float windStrength;
        int is_ship; // 1 if ship, 0 if not
        //ENUM start_finish;
        //ENUM land_water;
};

class gameMap {
    public:
        vector< vector<mapTile> > mapTiles;
        int x_size; 
        int y_size;
};

class gamestate {
    public:
        gameMap map;
        pirate Pirate;
        unordered_map<int, player_t> players;
        unordered_map<int, lobby_t> lobbies;
        float **heightMap; 
};

