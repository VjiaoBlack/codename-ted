/**
 * JSON.cpp
 *
 * David Woldenberg
 *
 * implementation of serializing/deserializing functions
 */

#include "JSON.h"

string serialize_gamestate(gamestate gstate){
	/*

    //example serialize from given structs in Slack
	//assuming some game state, "gstate"

	json main_obj;

	//map start

    json maptiles;

	BOOST_FOREACH(const std::string &tile_row_unit, gstate.map.mapTiles)
	{
        json tiles;

        BOOST_FOREACH(const std::string &tile_column_mem, tile_row_unit)
        {

            json tile;

            tile["currentDirection"] = {tile_column_mem.currentDirection.x, tile_column_mem.currentDirection.y};
            tile["currentStrength"] = tile_column_mem.currentStrength;
            tile["windDirection"] = {tile_column_mem.windDirection.x, tile_column_mem.windDirection.y};            
            tile["windStrength"] = tile_column_mem.windStrength;
            tile["is_ship"] = tile_column_mem.is_ship;
            tile["start_finish"] = tile_column_mem.start_finish;
            tile["land_water"] = tile_column_mem.land_water;      

            tiles.emplace_back(tile);
        }
        maptiles.emplace_back(tiles);
    }

    main_obj["map"]["mapTiles"] = maptiles;
    main_obj["map"]["x_size"] = gstate.map.x_size;
    main_obj["map"]["y_size"] = gstate.map.y_size;

    json pirates;

    BOOST_FOREACH(const std::string &pirate, gstate.map.pirates)
    {
        json p;

        p["pirate_name"] = pirate.pirate_name;
        p["position"] = {pirate.position.x, pirate.position.y};
        p["coord_pos"] = {pirate.coord_pos.x, pirate.coord_pos.y};
        p["velocity"] = {pirate.velocity.x, pirate.velocity.y};          
        p["acceleration"] = {pirate.acceleration.x, pirate.acceleration.y};
        p["orientation"] = pirate.orientation;
        p["rudderRot"] = pirate.rudderRot;
        p["goldAmount"] = pirate.goldAmount;
        p["AI"] = pirate.AI;     

        pirates.emplace_back(p);
    }

    main_obj["map"]["pirates"] = pirates;

    json merchants;

    BOOST_FOREACH(const std::string &merchant, gstate.map.merchants)
    {
        json m;

        m["merchant_name"] = merchant.merchant_name;
        m["position"] = {merchant.position.x, merchant.position.y};
        m["coord_pos"] = {merchant.coord_pos.x, merchant.coord_pos.y};
        m["velocity"] = {merchant.velocity.x, merchant.velocity.y};          
        m["acceleration"] = {merchant.acceleration.x, merchant.acceleration.y};
        m["orientation"] = merchant.orientation;
        m["rudderRot"] = merchant.rudderRot;
        m["goldAmount"] = merchant.goldAmount;
        m["AI"] = merchant.AI;     

        merchants.emplace_back(m);
    }

    main_obj["map"]["merchants"] = merchants;
    main_obj["map"]["size"] = {gstate.size.x, gstate.size.y, gstate.size.z};

    //map done

    //pirate start

    the_pirate["pirate_name"] = gstate.Pirate.pirate_name;
    the_pirate["position"] = {gstate.Pirate.position.x, gstate.Pirate.position.y};
    the_pirate["coord_pos"] = {gstate.Pirate.coord_pos.x, gstate.Pirate.coord_pos.y};
    the_pirate["velocity"] = {gstate.Pirate.velocity.x, gstate.Pirate.velocity.y};          
    the_pirate["acceleration"] = {gstate.Pirate.acceleration.x, gstate.Pirate.acceleration.y};
    the_pirate["orientation"] = gstate.Pirate.orientation;
    the_pirate["rudderRot"] = gstate.Pirate.rudderRot;
    the_pirate["goldAmount"] = gstate.Pirate.goldAmount;
    the_pirate["AI"] = gstate.Pirate.AI;

    main_obj["Pirate"] = the_pirate;

    //pirate done

    //players start

    json players;

    BOOST_FOREACH(HASH::value_type& player, gstate.map.players)
    {
        json p;

        p["uID"] = player.second.uID;
        p["x"] = player.second.x;
        p["y"] = player.second.y;
        p["registered"] = player.second.registered;
        p["name"] = player.second.name;

        players.emplace_back(player.first,p);
    }

    main_obj["players"] = players;

    //players done
    //not doing lobbies
    //not doing heightmap

  	return main_tree.dump();*/
    return 0;
}

gamestate deserialize_gamestate(string JSON){
	/*auto j = json::parse(JSON);

    std::cout << "JSON from deserialize: " << JSON < "\n";

    gamestate gstate;




    return gstate;*/

    return 0;
} 

string serialize_keystrokes(keystrokes_to_send ks){
    json main_obj;

    main_obj["unique_id"] = ks.uid;

    json keys;
    BOOST_FOREACH(pt::ptree::value_type &key, ks.keystrokes) {
        keys.emplace_back(key);
    }

    main_obj["keystrokes"] = keys

    std::cout << "JSON from serialize: " << main_obj.dump() < "\n";

    return main_obj.dump();
}

keystrokes_to_send deserialize_keystrokes(string JSON){

	auto j = json::parse(JSON);

	std::cout << "JSON from deserialize: " << JSON < "\n";

	int uid = j["unique_id"];

	vector<string> keys;
	BOOST_FOREACH(pt::ptree::value_type &key, j["keystrokes"]) {
		keys.insert((string) key);
	}

	keystrokes_to_send ret(uid, keys);

	return ret;
}