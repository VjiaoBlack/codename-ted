/**
 * JSON.cpp
 *
 * David Woldenberg
 *
 * implementation of serializing/deserializing functions
 */

#include "JSON.h"

string serialize_gamestate(gamestate gstate){
    //example serialize from given structs in Slack
	//assuming some game state, "gstate"

	json main_obj;

	//map start

    json maptiles;

	for(auto tile_row_unit : gstate.map.mapTiles)
	{
        json tiles;

        for(auto tile_column_mem : tile_row_unit)
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

    for(auto pirate : gstate.map.pirates)
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

    for(auto merchant : gstate.map.merchants)
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
    main_obj["map"]["size"] = {gstate.map.size.x, gstate.map.size.y, gstate.map.size.z};

    //map done

    //pirate start

    json the_pirate;

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

    for(auto player : gstate.players)
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

  	return main_obj.dump();
}

gamestate deserialize_gamestate(string JSON){
    auto j = json::parse(JSON);

    std::cout << "JSON from deserialize: " << JSON << "\n";

    //TODO: fix for no constructor, NEEDS TO BE ADDRESSED

    gameMap initGameMap = create_blank_map();
    pirate initPirate = create_basic_pirate(vec2(0.0, 0.0));
    unordered_map<int, player_t> players_x;
    unordered_map<int, lobby_t> lobbies;

    gamestate gstate(initGameMap, initPirate, players_x, lobbies);

    //build map

    vector< vector<mapTile> > mapTiles;

    for(auto tiles : j["map"]["mapTiles"])
    {
        vector<mapTile> mapTile_col;

        for(auto tile : tiles)
        {
            mapTile temp = mapTile(vec2(tile["currentDirection"]["x"], tile["currentDirection"]["y"]), tile["currentStrength"],
                vec2(tile["windDirection"]["x"], tile["windDirection"]["y"]), tile["currentStrength"], tile["is_ship"]);
            temp.start_finish = tile["start_finish"];
            temp.land_water = tile["land_water"];

            mapTile_col.push_back(temp);
        }

        mapTiles.push_back(mapTile_col);
    }

    vector<pirate> pirates; 

    for(auto pirate_temp : j["map"]["pirates"])
    {
        pirate p = pirate(pirate_temp["pirate_name"], 
            vec2(pirate_temp["position"]["x"], pirate_temp["position"]["y"]), 
            vec2(pirate_temp["velocity"]["x"], pirate_temp["velocity"]["y"]),
            vec2(pirate_temp["acceleration"]["x"], pirate_temp["acceleration"]["y"]), 
            pirate_temp["orientation"], pirate_temp["rudderRot"], 
            pirate_temp["goldAmount"], pirate_temp["AI"], 
            vec2(pirate_temp["coord_pos"]["x"], pirate_temp["coord_pos"]["y"]));

        pirates.push_back(p);
    }

    vector<merchant> merchants;

    for(auto merchant_temp : j["map"]["merchants"])
    {
        merchant m = merchant(merchant_temp["merchant_name"], 
            vec2(merchant_temp["position"]["x"], merchant_temp["position"]["y"]), 
            vec2(merchant_temp["velocity"]["x"], merchant_temp["velocity"]["y"]),
            vec2(merchant_temp["acceleration"]["x"], merchant_temp["acceleration"]["y"]), 
            merchant_temp["orientation"], merchant_temp["rudderRot"], 
            merchant_temp["goldAmount"], merchant_temp["AI"], 
            vec2(merchant_temp["coord_pos"]["x"], merchant_temp["coord_pos"]["y"])); 

        merchants.push_back(m);
    }

    vec3 size(j["map"]["size"][0], j["map"]["size"][1], j["map"]["size"][2]);

    gstate.map = gameMap(mapTiles, j["map"]["x_size"], j["map"]["y_size"], pirates, merchants, size);

    //map done

    //build pirate

    gstate.Pirate = pirate(j["Pirate"]["pirate_name"], 
        vec2(j["Pirate"]["position"]["x"], j["Pirate"]["position"]["y"]), 
        vec2(j["Pirate"]["velocity"]["x"], j["Pirate"]["velocity"]["y"]),
        vec2(j["Pirate"]["acceleration"]["x"], j["Pirate"]["acceleration"]["y"]), 
        j["Pirate"]["orientation"], j["Pirate"]["rudderRot"], 
        j["Pirate"]["goldAmount"], j["Pirate"]["AI"], 
        vec2(j["Pirate"]["coord_pos"]["x"], j["Pirate"]["coord_pos"]["y"]));

    //pirate done

    //build players

    unordered_map<int, player_t> players;

    for(auto player_temp : j["players"])
    {
        player_t p;
        p.uID = player_temp["uID"];
        p.x = player_temp["x"];
        p.y = player_temp["y"];
        p.registered = player_temp["registered"];
        p.name = player_temp["name"];

        players[p.uID] = p;
    }

    //players done

    return gstate;
} 

string serialize_keystrokes(keystrokes_to_send ks){
    json main_obj;

    main_obj["unique_id"] = ks.unique_id;

    json keys;
    for(auto key : ks.keystrokes) {
        keys.emplace_back(key);
    }

    main_obj["keystrokes"] = keys;

    std::cout << "JSON from serialize: " << main_obj.dump() << "\n";

    return main_obj.dump();
}

keystrokes_to_send deserialize_keystrokes(string JSON){

	auto j = json::parse(JSON);

	std::cout << "JSON from deserialize: " << JSON << "\n";

	int uid = j["unique_id"];

	vector<string> keys;
	for( auto key : j["keystrokes"]) {
		keys.push_back(key);
	}

	keystrokes_to_send ret = keystrokes_to_send(uid, keys);

	return ret;
}