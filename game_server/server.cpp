/**
 * server.cpp
 *
 *  by Chris, Jigar
 *
 */

#include "server.hpp"

// TCP Connection Handler and Server Functions

void TCPConnection::start() {
    message_ = '0' + registration_number_;
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&TCPConnection::handle_write, shared_from_this()));
}

void RegistrationServer::handle_accept(TCPConnection::pointer new_connection,
    const boost::system::error_code &error) {
        int i = 0;
        if (!error) {
            // Add players to registration queue
            registered_players_.push_back(current_registered_);

            // Start all players at same x but different y
            currentGameState_->add_player(current_registered_, 8000, 8000 + (50 + current_registered_) * 3, true,
                                          player_names[current_registered_]);

            // std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
            // cout << serialized_gamestate << endl;
            current_registered_++;
            new_connection->start();
        }

    RegistrationServer::start_accept();
}

// UDP Server Functions

string GameLoopServer::translate_keystroke(int ks) {
    unordered_map<int, std::string> keystroke_map({
        {UP, "UP"},
        {DOWN, "DOWN"},
        {LEFT, "LEFT"},
        {RIGHT, "RIGHT"},
        {DROP_GOLD, "GOLD"}
    });

    return keystroke_map[ks];
}

void GameLoopServer::handle_receive(const boost::system::error_code& error,
                    std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size) {
        std::string incoming_message(recv_buffer_.data(), bytes_transferred);
        std::string gamestate_signal("gamestate");

        std::string serialized_gamestate = serialize_gamestate(*currentGameState_);

        if (!incoming_message.compare(gamestate_signal)) {
            std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
            boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

            socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                  boost::bind(&GameLoopServer::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));

        } else {
            // We've got some keystrokes!
            std::cout << incoming_message << std::endl;
            keystrokes_obj ks = deserialize_keystrokes(incoming_message);
            // keystrokes added to queue, updated on timer!
            int queue_size = add_keys_to_queue(ks.keystrokes, player_names_[ks.unique_id]);
        }

        start_receive();
    }
}

// Timer Functions for GameLoop

int GameLoopServer::add_keys_to_queue(vector<int> ks, string name) {
        unordered_map<string, vector<string> > input_object;
        input_object["player_name"].push_back(name);

        for (int i = 0; i < ks.size(); ++i) {
            std::string key_str = translate_keystroke(ks[i]);
            input_object["keystrokes"].push_back(key_str);
        }
        incoming_objects.push(input_object);

        return incoming_objects.size();
    }

queue<unordered_map<string, vector<string> > > GameLoopServer::remove_all_keys(){
    queue<unordered_map<string, vector<string> > > all_objects;

    while (!incoming_objects.empty())
    {
        all_objects.push(incoming_objects.front());
        /* std::cout << "name: " << all_objects.front()["player_name"][0] << "\n";
        for (int i = 0; i < all_objects.front()["keystrokes"].size(); ++i) {
            std::cout << "key: " << all_objects.front()["keystrokes"][i] << "\n";
        } */
        incoming_objects.pop();
    }

    return all_objects;
}

void GameLoopServer::check_game_ended() {
    if (currentGameState_->map.merchants.size()) {
        for (int i = 0; i < currentGameState_->map.merchants.size(); ++i) {
            if (currentGameState_->map.merchants[i].AI)
                return;
        }
        game_has_ended_ = true;
    }
}

void GameLoopServer::end_game() {
    int high_gold = -1;
    int winner_id = -1;

    for (int j = 0; j < currentGameState_->map.merchants.size(); ++j) {
        if (high_gold < currentGameState_->map.merchants[j].goldAmount) {
            high_gold = currentGameState_->map.merchants[j].goldAmount;
            winner_id = j;
        }
    }

    if (winner_id >= 0) {
        currentGameState_->Pirate.pirate_name = player_names_[winner_id];
        currentGameState_->Pirate.AI = false;
    }
}

void GameLoopServer::advance_timer() {
    queue<unordered_map<string, vector<string> > > objects_to_process = this->remove_all_keys();
    ++count_;

    GameLoopServer::check_game_ended();

    //std::cout << game_has_ended_ << std::endl;

    if (!game_has_ended_) {
        //run 'run_astar' every 5 ticks (interval TBD)
        if (count_ % 100 == 0) {
            std::cout << "running astar" << std::endl;
            run_astar(currentGameState_->map);
            std::cout << "ran astar" << std::endl;
        }

        //Process gamestate/ai
        while (!objects_to_process.empty())
        {
            currentGameState_->map = compute_gamestate(objects_to_process.front(), currentGameState_->map);
            std::cout << serialize_gamestate(*currentGameState_) << std::endl;
            objects_to_process.pop();
        }

        if (objects_to_process.empty()) {
            unordered_map<string, vector<string> > input_object;
            vector<string> name;
            name.push_back("Reppy");
            vector<string> content;
            input_object["player_name"] = name;
            input_object["keystrokes"] = content;
            currentGameState_->map = compute_gamestate(input_object, currentGameState_->map);
        }
    }

    timer_.expires_at(timer_.expires_at() + boost::posix_time::microseconds(10000));
    timer_.async_wait(boost::bind(&GameLoopServer::advance_timer, this));
}

//Main
int main(int argc, char *argv[1]) {
    if (argc != 2) {
        std::cout << "Usage ./server <port>" << std::endl;
        return 1;
    }

    try {
        int port = atoi(argv[1]);
        boost::asio::io_service io_service;
        PiGameState *coreGameState = new PiGameState();
        // coreGameState->map = PiGameMap::createStartMap(64, 64, 65536);
        coreGameState->map = read_png_heightmap("../ai/height.csv", 48, 48, 32000);

        // Creates a pirate and a merchant
        RegistrationServer registration_server(io_service, port, coreGameState);
        GameLoopServer loop_server(io_service, port, coreGameState);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
