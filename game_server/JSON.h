/**
 * JSON.h
 *
 * David Woldenberg
 *
 * functions to serialize/deserilize game state via JSON/maps
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <map>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define MAX_CHAR 16384

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

class gamestate {
	public:
		// Game state var
		// Pirate var
		// Merchant var
		int time;
		game_map gmap; //will have 2D array: std::array< std::array <map_tiles> > map_tiles
		vector<players> players;
		
		gamestate(){
			// Game state declaration, need to consult charlie
		}
};

class response {
	public:

		int unique_id;
		vector<string> keystrokes;

		response(int uid, vector<string> ks){
			this->unique_id = uid;
			this->keystrokes = ks;
		}
};

response deserilize_response(char JSON[MAX_CHAR]);

string serialize_response(response r);

gamestate deserilize_gamestate(char JSON[MAX_CHAR]);

string serialize_gamestate(gamestate g);
