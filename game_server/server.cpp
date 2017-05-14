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


using boost::asio::ip::udp;

#define MAX_RECV_LENGTH 1000000

std::string serialize_game_state() {
    std::string sweg("yooooooo");
    return sweg;
}

class GameLoopServer {
public:
    gameMap initGameMap = create_blank_map();
    pirate initPirate = create_basic_pirate(vec2(0.0, 0.0));
    unordered_map<int, player_t> players;
    unordered_map<int, lobby_t> lobbies;
    float **heightMap;

    GameLoopServer(boost::asio::io_service& io_service, int port)
        : socket_(io_service, udp::endpoint(udp::v4(), port))
        , currentGameState_(initGameMap, initPirate, players, lobbies) {
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
    gamestate currentGameState_;
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
