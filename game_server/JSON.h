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
#include <map>
#include <boost/foreach.hpp>
#include "game_structs.hpp"
#include "json.hpp"

using std::string;
using json = nlohmann::json;

class keystrokes_to_send {
	public:

		//attributes
		string unique_id;
		vector<string> keystrokes;

		//Constructor
		response(int uid, vector<string> ks){
			this->unique_id = uid;
			this->keystrokes = ks;
		}
};

keystrokes_to_send deserilize_keystrokes(string JSON);

string serialize_keystrokes(keystrokes_to_send ks);

gamestate deserilize_gamestate(string JSON);

string serialize_gamestate(gamestate g);
