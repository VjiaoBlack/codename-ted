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

struct lobby_t {
	int uID;
	std::unordered_map<int, player_t> players;
};

struct gamestate {
	std::unordered_map<int, player_t> players;
	std::unordered_map<int, lobby_t> lobbies;

};