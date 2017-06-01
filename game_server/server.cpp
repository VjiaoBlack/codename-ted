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

            // If it is first registration, edit ship that was created by createRandomMap

            currentGameState_->add_player(current_registered_, 0, 0, true, player_names[current_registered_]);

            std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
            //cout << serialized_gamestate << endl;
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
        {DROP_GOLD, "DROP_GOLD"}
    });
    return keystroke_map[ks];
}

void GameLoopServer::handle_receive(const boost::system::error_code& error,
                    std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size) {
        std::string incoming_message(recv_buffer_.data(), bytes_transferred);
        std::string gamestate_signal("gamestate");

        std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
        //cout << serialized_gamestate << endl;

        if (!incoming_message.compare(gamestate_signal)) {
            std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
            //cout << serialized_gamestate << endl;
            boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

            socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                  boost::bind(&GameLoopServer::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));

        } else {
            // We've got some keystrokes!
            std::cout << incoming_message << std::endl;
            keystrokes_obj ks = deserialize_keystrokes(incoming_message);
            //keystrokes added to queue, updated on timer!
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
        std::cout << "name: " << all_objects.front()["player_name"][0] << "\n";
        for (int i = 0; i < all_objects.front()["keystrokes"].size(); ++i) {
            std::cout << "key: " << all_objects.front()["keystrokes"][i] << "\n";
        }
        incoming_objects.pop();
    }

    return all_objects;
}

void GameLoopServer::advance_timer() {
    queue<unordered_map<string, vector<string> > > objects_to_process = this->remove_all_keys();
    ++count_;

    //run 'run_astar' every 5 ticks (interval TBD)
    if(count_ % 100 == 0) {
        std::cout << "running astar" << std::endl;
        run_astar(currentGameState_->map);
        std::cout << "ran astar" << std::endl;
    }

    vec2 initial_pirate_pos = currentGameState_->map.pirates[0].coord_pos;
    vec2 destination(initial_pirate_pos.x + currentGameState_->map.pirates[0].velocity.x,
                    initial_pirate_pos.y + currentGameState_->map.pirates[0].velocity.y);
//    currentGameState_->map.pirates[0].coord_pos.x += (currentGameState_->map.pirates[0].velocity.x);
//    currentGameState_->map.pirates[0].coord_pos.y += (currentGameState_->map.pirates[0].velocity.y);


    //std::cout << std::endl;
    //initial_pirate_pos.print_vec2();

    shift_pirate(currentGameState_->map, initial_pirate_pos, destination);
    //Process gamestate/ai
    while (!objects_to_process.empty())
    {
        currentGameState_->map = compute_gamestate(objects_to_process.front(), currentGameState_->map);
        std::cout << serialize_gamestate(*currentGameState_) << std::endl;
        objects_to_process.pop();
    }

    timer_.expires_at(timer_.expires_at() + boost::posix_time::microseconds(100000));
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
        // Creates a pirate and a merchant
        coreGameState->map = PiGameMap::createStartMap(256, 256, 65536);
        // coreGameState->add_pirate(10, 10);
        RegistrationServer registration_server(io_service, port, coreGameState);
        GameLoopServer loop_server(io_service, port, coreGameState);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
