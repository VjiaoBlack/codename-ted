/**
 * json_serializer.cpp
 *
 * David Woldenberg
 *
 * implementation of serializing/deserializing functions
 */

#include "json_serializer.hpp"

string serialize_gamestate(PiGameState gstate, bool withMapTiles) {
    // example serialize from given structs in Slack
    // assuming some game state, "gstate"
    json main_obj;

    // map start
    main_obj["id"] = gstate.id;

    json maptiles;

    if (withMapTiles) {
        for(auto tile_row_unit : gstate.map.mapTiles) {
            json tiles;

            for(auto tile_column_mem : tile_row_unit) {

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
    }

    json pirates;

    for(auto PiPirate : gstate.map.pirates) {
        json p;

        p["pirate_name"] = PiPirate.pirate_name;
        p["position"] = {PiPirate.position.x, PiPirate.position.y};
        p["coord_pos"] = {PiPirate.coord_pos.x, PiPirate.coord_pos.y};
        p["velocity"] = {PiPirate.velocity.x, PiPirate.velocity.y};
        p["acceleration"] = {PiPirate.acceleration.x, PiPirate.acceleration.y};
        p["orientation"] = PiPirate.orientation;
        p["rudderRot"] = PiPirate.rudderRot;
        p["goldAmount"] = PiPirate.goldAmount;
        p["AI"] = PiPirate.AI;

        pirates.emplace_back(p);
    }

    main_obj["map"]["pirates"] = pirates;

    json merchants;

    for(auto PiMerchant : gstate.map.merchants) {
        json m;

        m["merchant_name"] = PiMerchant.merchant_name;
        m["position"] = {PiMerchant.position.x, PiMerchant.position.y};
        m["coord_pos"] = {PiMerchant.coord_pos.x, PiMerchant.coord_pos.y};
        m["velocity"] = {PiMerchant.velocity.x, PiMerchant.velocity.y};
        m["acceleration"] = {PiMerchant.acceleration.x, PiMerchant.acceleration.y};
        m["orientation"] = PiMerchant.orientation;
        m["rudderRot"] = PiMerchant.rudderRot;
        m["goldAmount"] = PiMerchant.goldAmount;
        m["AI"] = PiMerchant.AI;

        merchants.emplace_back(m);
    }

    main_obj["map"]["merchants"] = merchants;
    main_obj["map"]["size"] = {gstate.map.size.x, gstate.map.size.y, gstate.map.size.z};

    // PiPirate start

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

    // PiPirate done

    // players start

    json players;

    printf("ASDSDF\n");
    for (auto player : gstate.players) {
        json p;

        printf("1\n");
        p["uID"] = player.second.uID;
        printf("2\n");
        p[0] = player.second.x;
        printf("3\n");
        p[1] = player.second.y;
        printf("4\n");
        p["registered"] = player.second.registered;
        printf("5\n");
        p["name"] = player.second.name;
        printf("6\n");

        players.emplace_back(player.first,p);
    }
    printf("ASDSD222222F\n");


    main_obj["players"] = players;

    // players done
    // not doing lobbies
    // not doing heightmap

    // std::cout << "JSON from serialize: " << main_obj.dump() << "\n";

    return main_obj.dump();
}

PiGameState deserialize_gamestate(string JSON, bool withMapTiles) {
    std::cout << JSON << std::endl;
    auto j = json::parse(JSON);

    //TODO: fix for no constructor, NEEDS TO BE ADDRESSED
    PiGameState gstate;

    gstate.id = j["id"];

    //build map
    vector< vector<PiMapTile> > mapTiles;

    if (withMapTiles) {

        for(auto tiles : j["map"]["mapTiles"]) {
            vector<PiMapTile> mapTile_col;

            for(auto tile : tiles) {
                PiMapTile temp = PiMapTile(vec2(tile["currentDirection"][0], tile["currentDirection"][1]), tile["currentStrength"],
                    vec2(tile["windDirection"][0], tile["windDirection"][1]), tile["currentStrength"], tile["is_ship"]);
                temp.start_finish = tile["start_finish"];
                temp.land_water = tile["land_water"];

                mapTile_col.push_back(temp);
            }

            mapTiles.push_back(mapTile_col);
        }
    }

    vector<PiPirate> pirates;
    for(auto pirate_temp : j["map"]["pirates"]) {
        PiPirate p = PiPirate(pirate_temp["pirate_name"],
            vec2(pirate_temp["position"][0], pirate_temp["position"][1]),
            vec2(pirate_temp["velocity"][0], pirate_temp["velocity"][1]),
            vec2(pirate_temp["acceleration"][0], pirate_temp["acceleration"][1]),
            pirate_temp["orientation"], pirate_temp["rudderRot"],
            pirate_temp["goldAmount"], pirate_temp["AI"],
            vec2(pirate_temp["coord_pos"][0], pirate_temp["coord_pos"][1]));

        pirates.push_back(p);
    }

    vector<PiMerchant> merchants;

    for(auto merchant_temp : j["map"]["merchants"]) {
        PiMerchant m = PiMerchant(merchant_temp["merchant_name"],
            vec2(merchant_temp["position"][0], merchant_temp["position"][1]),
            vec2(merchant_temp["velocity"][0], merchant_temp["velocity"][1]),
            vec2(merchant_temp["acceleration"][0], merchant_temp["acceleration"][1]),
            merchant_temp["orientation"], merchant_temp["rudderRot"],
            merchant_temp["goldAmount"], merchant_temp["AI"],
            vec2(merchant_temp["coord_pos"][0], merchant_temp["coord_pos"][1]));

        merchants.push_back(m);
    }

    // build PiPirate
    gstate.Pirate = PiPirate(j["Pirate"]["pirate_name"],
                             vec2(j["Pirate"]["position"][0], j["Pirate"]["position"][1]),
                             vec2(j["Pirate"]["velocity"][0], j["Pirate"]["velocity"][1]),
                             vec2(j["Pirate"]["acceleration"][0], j["Pirate"]["acceleration"][1]),
                             j["Pirate"]["orientation"], j["Pirate"]["rudderRot"],
                             j["Pirate"]["goldAmount"], j["Pirate"]["AI"],
                             vec2(j["Pirate"]["coord_pos"][0], j["Pirate"]["coord_pos"][1]));

    // PiPirate done

    // build players
    unordered_map<int, PiPlayer> players;

    for(auto player_temp : j["players"]) {
        PiPlayer p;
        p.uID = player_temp["uID"];
        p.x = player_temp[0];
        p.y = player_temp[1];
        p.registered = player_temp["registered"];
        p.name = player_temp["name"];

        players[p.uID] = p;
    }

    return gstate;
}

string serialize_keystrokes(keystrokes_obj ks) {
    json main_obj;

    main_obj["unique_id"] = ks.unique_id;

    json keys;
    for(auto key : ks.keystrokes) {
        keys.emplace_back(key);
    }

    main_obj["keystrokes"] = keys;

    return main_obj.dump();
}

keystrokes_obj deserialize_keystrokes(string JSON) {
    auto j = json::parse(JSON);

    int uid = j["unique_id"];

    vector<int> keys;
    for( auto key : j["keystrokes"]) {
        keys.push_back(key);
    }

    keystrokes_obj ret = keystrokes_obj(uid, keys);

    return ret;
}
