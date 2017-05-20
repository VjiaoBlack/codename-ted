/**
 * json_serializer.hpp
 *
 * David Woldenberg
 *
 * functions to serialize/deserialize game state via JSON/maps
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include "game_structs.hpp"
#include "json.hpp"

#ifndef _JSON_SERIALIZER_HPP_
#define _JSON_SERIALIZER_HPP_

using std::string;
using std::vector;
using json = nlohmann::json;

class keystrokes_obj {
	public:

		//attributes
		int unique_id;
		vector<int> keystrokes;

		//Constructor
		keystrokes_obj(int uid, vector<int> ks){
			this->unique_id = uid;
			this->keystrokes = ks;
		}
};

keystrokes_obj deserialize_keystrokes(string JSON);

string serialize_keystrokes(keystrokes_obj ks);

PiGameState deserialize_gamestate(string JSON, bool withMapTiles=false);

string serialize_gamestate(PiGameState g, bool withMapTiles=false);

#endif