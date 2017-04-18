/* 
 *
 * definitions.hpp - codename-ted (2017)
 *
 * Definitions for necessary structs
 * including player, lobby, gamestate, etc
 *
 * Worked on by Chris, David, Victor, and Jigar
 *
 */

#include <cstdlib>
#include <unordered_map>

class player_t {
    public:
        int uID;
	    float x;
	    float y;
	    bool registered;
	    std::string name;
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
} ;

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
}

class lobby_t {
    public:
        int uID;
        std::unordered_map<int, player_t> players;
};

class mapTile {
    public:   
        vec2 currentDirection;
        float currentStrength;
        vec2 windDirection;
        float windStrength;
        ENUM start_finish;
        ENUM land_water;
        int **heightMap;  //idk how this will be represented
};

class gameMap {
    public:
        vector<mapTile> mapTiles;
} ;

class gamestate {
    public:
        map gameMap;
        pirate Pirate;
        std::unordered_map<int, player_t> players;
        std::unordered_map<int, lobby_t> lobbies;
};
