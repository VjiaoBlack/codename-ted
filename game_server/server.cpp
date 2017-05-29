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
            std::cout << "We got here!" << std::endl;
            // Add players to registration queue
            registered_players_.push_back(current_registered_);
            currentGameState_->add_player(current_registered_, 0, 0, true, player_names[current_registered_]);
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
        {RIGHT, "RIGHT"}
    });
    return keystroke_map[ks];
}

void GameLoopServer::handle_receive(const boost::system::error_code& error,
                    std::size_t bytes_transferred) {
    if (!error || error == boost::asio::error::message_size) {
        std::string incoming_message(recv_buffer_.data(), bytes_transferred);
        std::string gamestate_signal("gamestate");

        std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
        cout << serialized_gamestate << endl;

        if (!incoming_message.compare(gamestate_signal)) {
            std::string serialized_gamestate = serialize_gamestate(*currentGameState_);
            cout << serialized_gamestate << endl;
            boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

            socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                  boost::bind(&GameLoopServer::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));

        } else {
            // We've got some keystrokes!
            std::cout << incoming_message << std::endl;
            keystrokes_obj ks = deserialize_keystrokes(incoming_message);
            unordered_map<string, vector<string> > input_object;

            for (int i = 0; i < ks.keystrokes.size(); ++i) {
                std::string key_str = translate_keystroke(ks.keystrokes[i]);
                input_object["keystrokes"].push_back(key_str);
            }

            input_object["player_name"].push_back(player_names_[ks.unique_id]);

            currentGameState_->map = compute_gamestate(input_object, currentGameState_->map);
            //run_astar(currentGameState_->map);

            std::cout << incoming_message << std::endl;
        }

        start_receive();
    }
}

int main(int argc, char *argv[1]) {
    if (argc != 2) {
        std::cout << "Usage ./server <port>" << std::endl;
        return 1;
    }

    try {
        int port = atoi(argv[1]);
        boost::asio::io_service io_service;
        PiGameState *coreGameState = new PiGameState();
        RegistrationServer registration_server(io_service, port, coreGameState);
        GameLoopServer loop_server(io_service, port, coreGameState);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
