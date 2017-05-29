/**
 * client.hpp
 *
 * worked on by Jigar, Chris
 */
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "json_serializer.hpp"
#include "../physics_ck/physics_tryout.hpp"


#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

#define MAX_RECV_LENGTH 100000

class TCPClient {
public:
    TCPClient(boost::asio::io_service &io_service,
              const std::string &host,
              const std::string &port)
              : io_service_(io_service)
              , socket_(io_service, tcp::endpoint(tcp::v4(), 0)) {
              tcp::resolver resolver(io_service_);
              tcp::resolver::query query(tcp::v4(), host, port);
              endpoint_iterator_ = resolver.resolve(query);
              receiver_endpoint_ = *endpoint_iterator_;
              }

    ~TCPClient() {
        socket_.close();
    }

    size_t send(const std::string &message);
    int register_player();
    void start_game(int player_id);

private:
    boost::asio::io_service &io_service_;
    tcp::socket socket_;
    tcp::endpoint sender_endpoint_;
    tcp::endpoint receiver_endpoint_;
    tcp::resolver::iterator endpoint_iterator_;
    boost::array<char, MAX_RECV_LENGTH> recv_buf;
    boost::system::error_code error;
};

// Inspiration from https://gist.github.com/kaimallea/e112f5c22fe8ca6dc627
class UDPClient {
public:
    UDPClient(boost::asio::io_service &io_service,
              const std::string &host,
              const std::string &port,
              int player_id)
        : io_service_(io_service)
        , socket_(io_service, udp::endpoint(udp::v4(), 0)) {
        player_id_ = player_id;
        udp::resolver resolver(io_service_);
        udp::resolver::query query(udp::v4(), host, port);
        udp::resolver::iterator iter = resolver.resolve(query);
        receiver_endpoint_ = *iter;
    }

    ~UDPClient() {
        socket_.close();
    }

    size_t send(const std::string &message);
    void send_keystrokes(std::vector<int> keystrokes);
    PiGameState get_gamestate();
    int player_id_;

private:
    boost::asio::io_service &io_service_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    udp::endpoint receiver_endpoint_;
    boost::array<char, MAX_RECV_LENGTH> recv_buf;
};

#endif
