#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "../ai/astar.hpp"
#include "../physics_ck/physics_tryout.hpp"
#include "json_serializer.hpp"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

#define UP KC_I
#define DOWN KC_K
#define LEFT KC_J
#define RIGHT KC_L

#define MAX_RECV_LENGTH 1000000

// TCPConnection from
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/tutorial/tutdaytime7.html

class TCPConnection
// Handles basic connectivity
    : public boost::enable_shared_from_this<TCPConnection>
{
public:
    typedef boost::shared_ptr<TCPConnection> pointer;

    static pointer create(boost::asio::io_service& io_service, int registration_number) {
        return pointer(new TCPConnection(io_service, registration_number));
    }

    // Returns the socket if we need it
    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        message_ = '0' + registration_number_;
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&TCPConnection::handle_write, shared_from_this()));
    }

private:
    TCPConnection(boost::asio::io_service &io_service, int registration_number)
        : socket_(io_service) {
            registration_number_ = registration_number;
        }

    void handle_write() {

    }

    int registration_number_;
    tcp::socket socket_;
    std::string message_;
};

class RegistrationServer {
// The class that actually updates the gamestate and performs setup
public:
    RegistrationServer(boost::asio::io_service &io_service, int port, PiGameState *inputGameState)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
            currentGameState_ = inputGameState;
            current_registered_ = 0;
            game_has_not_started_ = true;
            player_names = {"jeff", "barack", "bernard", "ruth", "hillary"};
            start_accept();
        }

    bool game_has_not_started() {
        return game_has_not_started_;
    }

    PiGameState* get_starting_gamestate() {
        return currentGameState_;
    }

private:
    void start_accept() {
        TCPConnection::pointer new_connection = TCPConnection::create(acceptor_.get_io_service(), current_registered_);

        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&RegistrationServer::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(TCPConnection::pointer new_connection,
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

        start_accept();
    }

    bool game_has_not_started_;
    int current_registered_;
    vector<int> registered_players_;
    vector<string> player_names;
    boost::array<char, MAX_RECV_LENGTH> recv_buffer_;
    tcp::acceptor acceptor_;
    PiGameState *currentGameState_;
};


class GameLoopServer {
public:
    float **heightMap;

    GameLoopServer(boost::asio::io_service& io_service, int port, PiGameState *inputGameState)
        : socket_(io_service, udp::endpoint(udp::v4(), port)) {
            currentGameState_ = inputGameState;
            player_names_ = {"jeff", "barack", "bernard", "ruth", "hillary"};
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

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/) {
    }

    unordered_map<int, string> keystroke_translator;
    vector<string> player_names_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, MAX_RECV_LENGTH> recv_buffer_;
    PiGameState *currentGameState_;
};
