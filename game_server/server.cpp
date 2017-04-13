/* 
 *
 * server.cpp - codename-ted (2017)
 *
 * The main server loop for the game
 * include the game logic, the interfacing with  networking, and
 * the physics engine and AI
 *
 * Worked on by Chris, David, and Jigar
 *
 */

#include <cstdlib>
#include <iostream>
#include "definitions.h"
#include <boost/asio.hpp>

using boost::asio::ip::udp; //for the game loop
using boost::asio::ip::tcp; //for registration

enum { max_length = 1024 };

/*void register(boost::asio::io_service& io_service, unsigned short port)
{
  //tcp::socket sock(io_service, tcp::endpoint(tcp::v4(), port));
}*/

void server(boost::asio::io_service& io_service, unsigned short port)
{
  //register(io_service, port);  

  udp::socket sock(io_service, udp::endpoint(udp::v4(), port));
  for (;;)
  {
    char data[max_length];
    udp::endpoint sender_endpoint;
    size_t length = sock.receive_from(
        boost::asio::buffer(data, max_length), sender_endpoint);
    data[length] = '!';
    data[length + 1] = '\0';
    sock.send_to(boost::asio::buffer(data, length + 2), sender_endpoint);
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_udp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

