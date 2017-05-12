/**
 * JSON.cpp
 *
 * David Woldenberg
 *
 * implementation of serializing functions
 */

#include "JSON.h"

string serialize_gamestate(gamestate gstate){
	//example serialize from given structs in Slack
	//assuming some game state, "gstate"

	ptree main_tree;

	//Add global/important variables to main tree here (TBD)
	main_tree.add("time", gstate.time);

	// For game_map, assuming struct, need to revise this to account for 2D structure of map_tiles array
	BOOST_FOREACH(const std::string &tile, gstate.gmap.map_tiles)
	{
        ptree tile_tree;
        tile_tree.add("tile.cur_direction", tile.cur_direction);
        tile_tree.add("tile.cur_strength", tile.cur_strength);
        tile_tree.add("tile.wind_direction", tile.wind_direction);
        tile_tree.add("tile.wind_strength", tile.wind_strength);
		tile_tree.add("tile.is_land", tile.is_land);
		tile_tree.add("tile.is_start", tile.is_start);        

        main_tree.add_child("game_map.map_tiles", title_tree);
    }

    //For players, assuming struct
    BOOST_FOREACH(const std::string &player, gstate.players)
	{
        ptree player_tree;
        player_tree.add("player.name", player.name);
        player_tree.add("player.orientation", player.orientation);
        player_tree.add("player.rudder_rotation", player.rudder_rotation);
        player_tree.add("player.gold_amount", player.gold_amount);
        player_tree.add("player.AI", player.AI);

        //add subtree for position, velocity, acceleration

        ptree pos_tree;
        pos_tree.add("position.x", player.position.x);
        pos_tree.add("position.y", player.position.y);
        player_tree.add_child("player.position", pos_tree);

        ptree vel_tree;
        pos_tree.add("velocity.x", player.velocity.x);
        pos_tree.add("velocity.y", player.velocity.y);
        player_tree.add_child("player.velocity", vel_tree);

        ptree accel_tree;
        pos_tree.add("acceleration.x", player.acceleration.x);
        pos_tree.add("acceleration.y", player.acceleration.y);
        player_tree.add_child("player.acceleration", accel_tree);           

        main_tree.add_child("players", player_tree);
    }

    std::ostringstream JSON; 
  	write_json (JSON, main_tree, false); 

  	return JSON.str();
}

gamestate deserialize_gamestate(char JSON[MAX_CHAR]){
	gamestate gstate;
	ptree main_tree;

    std::istringstream is (JSON);
    read_json (is, main_tree);

	gstate.time = main_tree.get<int>("time");

	//iterate through segments of JSON hierarchy and convert them into tree branhces

	vector<map_tile> map_tiles;

	BOOST_FOREACH(pt::ptree::value_type &tile, tree.get_child("game_map.map_tiles")) {

		map_tile the_tile;

        the_tile.cur_direction =  tile.get<int>("tile.cur_direction");
        the_tile.cur_strength =  tile.get<int>("tile.cur_strength");
        the_tile.wind_direction =  tile.get<int>("tile.wind_direction");
        the_tile.wind_strength =  tile.get<int>("tile.wind_strength");
        the_tile.is_land =  tile.get<bool>("tile.is_land");
		the_tile.is_start =  tile.get<bool>("tile.is_start");      

        map_tiles.insert(the_tile);
	}

	vector<player> players;

	//For players, assuming struct
    BOOST_FOREACH(pt::ptree::value_type &player, tree.get_child("players")) {
	{
        
		player the_player;

        the_player.name =  tile.get<string>("player.name");
        the_player.orientation =  tile.get<int>("player.orientation");
        the_player.rudder_rotation =  tile.get<int>("player.rudder_rotation");
        the_player.gold_amount =  tile.get<int>("player.gold_amount");
        the_player.AI =  tile.get<int>("player.AI");

        //add subtstructs for position, velocity, acceleration

        position x;
        x.x = player.get<int>("player.position.x");
        x.y = player.get<int>("player.position.y");
        the_player.position = x;

        velocity v;
        v.x = player.get<int>("player.velocity.x");
        v.y = player.get<int>("player.velocity.y");
        the_player.velocity = v;

        acceleration a;
        a.x = player.get<int>("player.acceleration.x");
        a.y = player.get<int>("player.acceleration.y");
        the_player.acceleration = a;

        players.insert(the_player);
    }
} 

string serialize_response(response r){
    ptree main_tree;

    main_tree.add("unique_id", r.uid);

    BOOST_FOREACH(pt::ptree::value_type &key, response.keystrokes) {
        main_tree.add_child("keys", key);
    }

    std::ostringstream JSON; 
    write_json (JSON, main_tree, false); 

    return JSON.str();
}

response deserialize_response(char JSON[MAX_CHAR]){

	ptree res_tree;
	std::istringstream is (JSON);
	read_json (is, res_tree);

	std::cout << "JSON: " << JSON < "\n";

	int uid = response.get<int> ("unique_id");
	vector<string> keys;

	BOOST_FOREACH(pt::ptree::value_type &key, res_tree.get_child("keystrokes")) {
		keys.insert((string) key);
	}

	response ret(uid, keys);

	return ret;
}