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

// CLASSES
class vec2 {
    public: 
        // Attributes
        float x, y;
        // Constructor
        vec2(float a, float b) : x(a), y(b) {};
        // Methods
        void print_vec2();
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
        // ATTRIBUTES
        vec2 currentDirection;
        float currentStrength;
        vec2 windDirection;
        float windStrength;
        // Convert these to enums later
        int is_ship; // 1 if ship, 0 if not
        int start_finish; // 1 if start, 0 if finish
        int land_water; // 1 if land, 0 if water
        
        // CONSTRUCTOR 
        mapTile(vec2 a, float b, vec2 c, float d, int e) : 
                currentDirection(a), currentStrength(b), 
                windDirection(a), windStrength(b), is_ship(e) {};
        
        // METHODS
        void print_map_tile();
};

class gameMap {
    public:
        // ATTRIBUTES
        vector< vector<mapTile> > mapTiles;
        int x_size; 
        int y_size;
        //CONSTRUCTOR 
        gameMap(vector< vector<mapTile> > a, int b, int c) {
            mapTiles = a;
            x_size = b; 
            y_size = c;
        }; 
        // METHODS
        void print_game_map();
};

class gamestate {
    public:
        gameMap map;
        pirate Pirate;
        unordered_map<int, player_t> players;
        unordered_map<int, lobby_t> lobbies;
        float **heightMap; 
};

//METHODS
mapTile create_blank_tile();
gameMap create_blank_map(); 
gameMap create_random_map();
gameMap create_random_tile();

