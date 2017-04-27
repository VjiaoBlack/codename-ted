/* 
 *
 * game_structs.hpp - codename-ted (2017)
 *
 * Definitions for necessary structs
 * including player, lobby, gamestate, etc
 *
 * Worked on by Jigar, Charlie, Chris, David, and Victor 
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
        //Attributes
        vec2 position;
        vec2 velocity;
        vec2 acceleration;
        float orientation;
        float rudderRot;
        int goldAmount;
        bool AI;
        // Constructor
        ship(vec2 a, vec2 b, vec2 c, float d, 
                float e, int f, bool g) :
                position(a), velocity(b), acceleration(c),
                orientation(d), rudderRot(e), goldAmount(f), 
                AI(g) {}; 
        // Methods 
};

class pirate : public ship {
    public:
        // Attributes
        string pirate_name;
        // Constructor
        pirate(string a, vec2 b, vec2 c, vec2 d, 
                float e, float f, int g, bool h) : 
                pirate_name(a), ship(b, c, d, e, f, g, h) {};
};

class merchant : public ship {
    public: 
        // Attributes
        string merchant_name;
        // Constructor
        merchant(string a, vec2 b, vec2 c, vec2 d, 
                float e, float f, int g, bool h) : 
                merchant_name(a), ship(b, c, d, e, f, g, h) {};
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
        int is_ship; // 2 if pirate, 1 if merchant, 0 if nothing
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
        vector<pirate> pirates; 
        vector<merchant> merchants;
        //CONSTRUCTOR 
        gameMap(vector< vector<mapTile> > a, int b, int c,
                vector<pirate> d, vector <merchant> e) {
            mapTiles = a;
            x_size = b; 
            y_size = c;
            pirates = d; 
            merchants = e;
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

// HELPER FUNCTIONS
mapTile create_blank_tile();
gameMap create_blank_map(); 
gameMap create_random_map();
mapTile create_random_tile(int pirate);
pirate create_basic_pirate(vec2 loc);
merchant create_basic_merchant(vec2 loc); 

