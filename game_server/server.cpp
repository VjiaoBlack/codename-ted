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

#define UP KC_I
#define DOWN KC_K
#define LEFT KC_J
#define RIGHT KC_L

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


        // create test player
        currentGameState_.add_player(0, 0, 0, true, "swag");
        std::cout << serialize_gamestate(currentGameState_, false) << endl;


        start_receive();

    }

private:
    string translate_keystroke(int ks) {

        unordered_map<int, std::string> keystroke_map({
            {UP, "UP"},
            {DOWN, "DOWN"},
            {LEFT, "LEFT"},
            {RIGHT, "RIGHT"}
        });
        return keystroke_map[ks];
    }


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

            printf("%d --- \n", bytes_transferred);

            if (!incoming_message.compare(gamestate_signal)) {
                std::string serialized_gamestate = serialize_gamestate(currentGameState_);
                cout << serialized_gamestate << endl;
                boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

                socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                      boost::bind(&GameLoopServer::handle_send, this, message,
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));


                if (currentGameState_.players.size()) {

                }

            } else {

                // We've got some keystrokes!
                //
                std::cout << incoming_message << std::endl;
                keystrokes_obj ks = deserialize_keystrokes(incoming_message);
                unordered_map<string, vector<string> > input_object;

                for (int i = 0; i < ks.keystrokes.size(); ++i) {
                    std::string key_str = translate_keystroke(ks.keystrokes[i]);
                    input_object["keystrokes"].push_back(key_str);

                    if (ks.keystrokes[i] == KC_W) {
                        currentGameState_.players[20].y += 20.0;
                    }

                    if (ks.keystrokes[i] == KC_A) {
                        currentGameState_.players[20].y -= 20.0;
                    }
                }

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

    unordered_map<int, string> keystroke_translator;

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
