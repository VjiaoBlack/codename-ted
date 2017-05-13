//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

#define MAX_LENGTH 1000

std::string serialize_game_state()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class udp_server
{
public:
  udp_server(boost::asio::io_service& io_service, int port)
    : socket_(io_service, udp::endpoint(udp::v4(), port))
  {
    start_receive();
  }

private:
  void start_receive()
  {
        socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code& error,
      std::size_t bytes_transferred)
  {
    if (!error || error == boost::asio::error::message_size)
    {
      boost::shared_ptr<std::string> message(
          new std::string(serialize_game_state()));

      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
          boost::bind(&udp_server::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

      std::cout << bytes_transferred << std::endl;
      std::cout.write(recv_buffer_.data(), bytes_transferred);
      start_receive();
    }
  }

  void handle_send(boost::shared_ptr<std::string> /*message*/,
      const boost::system::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, MAX_LENGTH> recv_buffer_;
};

int main(int argc, char *argv[1])
{
    if (argc != 2) {
        std::cout << "Usage ./server <port>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_service io_service;
        int port = atoi(argv[1]);
        udp_server server(io_service, port);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

  return 0;
}
