#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
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
#define DROP_GOLD KC_G

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

    void start();

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
            player_names = {"jeff", "barack", "bernard", "ruth", "hillary"};
            start_accept();
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
        const boost::system::error_code &error);

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
        : socket_(io_service, udp::endpoint(udp::v4(), port))
        , timer_(io_service, boost::posix_time::microseconds(10))
        , count_(0) {
            currentGameState_ = inputGameState;
            player_names_ = {"jeff", "barack", "bernard", "ruth", "hillary"};
            timer_.async_wait(boost::bind(&GameLoopServer::advance_timer, this));
            start_receive();
    }

private:
    string translate_keystroke(int ks);

    void advance_timer();

    queue<unordered_map<string, vector<string> > > remove_all_keys();

    int add_keys_to_queue(vector<int> ks, string name);

    void start_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
                                   boost::bind(&GameLoopServer::handle_receive, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred);

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/) {
    }

    // Timer members
    boost::asio::deadline_timer timer_;
    queue<unordered_map<string, vector<string> > > incoming_objects;
    int count_;

    // game state members
    unordered_map<int, string> keystroke_translator;
    vector<string> player_names_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, MAX_RECV_LENGTH> recv_buffer_;
    PiGameState *currentGameState_;
};
