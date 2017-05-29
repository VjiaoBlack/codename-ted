#include <ctime>
#include <iostream>
#include <string>
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

    static pointer create(boost::asio::io_service& io_service) {
        return pointer(new TCPConnection(io_service));
    }

    // Returns the socket if we need it
    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        message_ = "Cowabunga!";
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&TCPConnection::handle_write, shared_from_this()));
    }

private:
    TCPConnection(boost::asio::io_service &io_service)
        : socket_(io_service) {

        }

    void handle_write() {

    }

    tcp::socket socket_;
    std::string message_;
};

class RegistrationServer {
// The class that actually updates the gamestate and performs setup
public:
    RegistrationServer(boost::asio::io_service &io_service, int port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
            start_accept();
        }
private:
    void start_accept() {
        TCPConnection::pointer new_connection = TCPConnection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&RegistrationServer::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(TCPConnection::pointer new_connection,
        const boost::system::error_code &error) {
            if (!error) {
                new_connection->start();
            }

        start_accept();
    }

    tcp::acceptor acceptor_;
};


class GameLoopServer {
public:
    float **heightMap;

    GameLoopServer(boost::asio::io_service& io_service, int port)
        : socket_(io_service, udp::endpoint(udp::v4(), port))
        , currentGameState_() {

            currentGameState_.add_player(0,0,0,true,"Clone");
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

            if (!incoming_message.compare(gamestate_signal)) {
                std::string serialized_gamestate = serialize_gamestate(currentGameState_);
                cout << serialized_gamestate << endl;
                boost::shared_ptr<std::string> message(new std::string(serialized_gamestate));

                socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                      boost::bind(&GameLoopServer::handle_send, this, message,
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));

                // if i have this here, physics updates twice when keys are pressed
                // unordered_map<string, vector<string> > input_object;
                // input_object["player_name"].push_back("Clone");

                // if (currentGameState_.players.size()) {
                //     currentGameState_.map = compute_gamestate(input_object, currentGameState_.map);
                // }

            } else {
                // We've got some keystrokes!
                std::cout << incoming_message << std::endl;
                keystrokes_obj ks = deserialize_keystrokes(incoming_message);
                unordered_map<string, vector<string> > input_object;

                for (int i = 0; i < ks.keystrokes.size(); ++i) {
                    std::string key_str = translate_keystroke(ks.keystrokes[i]);
                    input_object["keystrokes"].push_back(key_str);
                }

                input_object["player_name"].push_back("Clone");

                currentGameState_.map = compute_gamestate(input_object, currentGameState_.map);
                //run_astar(currentGameState_.map);

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
