/**
 * json_serializer.hpp
 *
 * David Woldenberg
 *
 * functions to serialize/deserilize game state via JSON/maps
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include "game_structs.hpp"
#include "json.hpp"

using std::string;
using std::vector;
using json = nlohmann::json;

class keystrokes_obj {
	public:

		//attributes
		string unique_id;
		vector<int> keystrokes;

		//Constructor
		keystrokes_obj(int uid, vector<int> ks){
			this->unique_id = uid;
			this->keystrokes = ks;
		}
};

keystrokes_obj deserilize_keystrokes(string JSON);

string serialize_keystrokes(keystrokes_obj ks);

gamestate deserilize_gamestate(string JSON, bool withMapTiles=false);

string serialize_gamestate(gamestate g, bool withMapTiles=false);
