/**
 * server.cpp
 *
 *  by Chris, Jigar
 *
 */


#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "json_serializer.hpp"
#include "../physics_ck/physics_tryout.hpp"


using boost::asio::ip::udp;

#define MAX_RECV_LENGTH 1000000

std::string serialize_game_state() {
    std::string sweg("yooooooo");
    return sweg;
}

class GameLoopServer {
public:
    float **heightMap;

    GameLoopServer(boost::asio::io_service& io_service, int port)
        : socket_(io_service, udp::endpoint(udp::v4(), port))
        , currentGameState_() { 

        std::cout << serialize_gamestate(currentGameState_, false) << endl;

        // create test player
        currentGameState_.players[20] = PiPlayer();

        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
                                   boost::bind(&GameLoopServer::handle_receive, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred) {
        if (!error || error == boost::asio::error::message_size) {
            std::string incoming_message(recv_buffer_.data(), bytes_transferred);
            std::string gamestate_signal("gamestate");

            if (!incoming_message.compare(gamestate_signal)) {
                std::string serialized_gamestate = serialize_gamestate(currentGameState_);
                boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

                socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                      boost::bind(&GameLoopServer::handle_send, this, message,
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
            } else {
                // We've got some keystrokes!
                keystrokes_obj ks = deserialize_keystrokes(incoming_message);
                unordered_map<string, vector<string> > input_object;
                vector<string> player;
                vector<string> keystrokes;
                vector<string> raw_strokes;
                raw_strokes.push_back("U");
                raw_strokes.push_back("D");
                raw_strokes.push_back("L");
                raw_strokes.push_back("R");
                player.push_back("victor");
                input_object["player_name"] = player;

                for (int i = 0; i < ks.keystrokes.size(); ++i) {
                    keystrokes.push_back(raw_strokes[ks.keystrokes[i]]);
                }

                input_object["keystrokes"] = keystrokes;

                currentGameState_.map = compute_gamestate(input_object, currentGameState_.map);
                std::cout << incoming_message << std::endl;
            }

            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/) {
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, MAX_RECV_LENGTH> recv_buffer_;
    PiGameState currentGameState_;
};

int main(int argc, char *argv[1]) {
    if (argc != 2) {
        std::cout << "Usage ./server <port>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_service io_service;
        int port = atoi(argv[1]);
        GameLoopServer server(io_service, port);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
