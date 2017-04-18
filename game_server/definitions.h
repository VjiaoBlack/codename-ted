/* 
 *
 * definitions.h - codename-ted (2017)
 *
 * Definitions for necessary structs
 * including player, lobby, gamestate, etc
 *
 * Worked on by Chris, David, and Victor
 *
 */

#include <cstdlib>
#include <unordered_map>

struct player_t {
	int uID;
	float x;
	float y;
	bool registered;
	std::string name;
};

struct vec2 {
	float mag;
	float dir;
};

struct ship {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    float orientation;
    float rudderRot;
    //Angular velocity?
    int goldAmount;
    bool AI;
};

struct pirate {
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    float orientation;
    float rudderRot;
    //Angular velocity?
    int goldAmount;
    bool AI;
};

struct lobby_t {
	int uID;
	std::unordered_map<int, player_t> players;
};

struct mapTile {
  vec2 currentDirection;
  float currentStrength;
  vec2 windDirection;
  float windStrength;
  //ENUM start_finish;
  //ENUM land_water;
  int **heightMap;  //idk how this will be represented
};

struct game_map {
  std::unordered_map<int, mapTile> mapTiles;
};

struct gamestate {
	game_map gameMap;
	pirate Pirate;
	std::unordered_map<int, player_t> players;
	std::unordered_map<int, lobby_t> lobbies;

};