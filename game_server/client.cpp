//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

// Inspiration from https://gist.github.com/kaimallea/e112f5c22fe8ca6dc627
class UDPClient {
public:
    UDPClient(
        boost::asio::io_service &io_service,
        const std::string &host,
        const std::string &port
    ) : io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), 0)) {
        udp::resolver resolver(io_service_);
        udp::resolver::query query(udp::v4(), host, port);
        udp::resolver::iterator iter = resolver.resolve(query);
        receiver_endpoint_ = *iter;
    }

    ~UDPClient() {
        socket_.close();
    }

    void send(const std::string &message) {
        std::cout << message.size() << std::endl;
        socket_.send_to(boost::asio::buffer(message, message.size()),
                        receiver_endpoint_);

        size_t len = socket_.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint_);

        std::cout.write(recv_buf.data(), len);
    }

    void send_keystrokes(std::vector<int> keystrokes, std::string ship_id) {
        std::cout << ship_id << std::endl;
    }

private:
    boost::asio::io_service &io_service_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    udp::endpoint receiver_endpoint_;
    boost::array<char, 128> recv_buf;
};

int main(int argc, char* argv[])
{
  try {
      if (argc != 3) {
          std::cerr << "Usage: client <host> <port>" << std::endl;
          return 1;
      }

      boost::asio::io_service io_service;
      UDPClient client(io_service, argv[1], argv[2]);

      client.send("{keystrokes: [UP, LEFT]}");
  } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
  }

  return 0;
}
