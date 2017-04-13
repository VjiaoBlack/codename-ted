/*
 * 
 * client.cpp - codename-ted (2017)
 *
 * The the client component of the game server
 * includes interfacing with the graphics aengine and networking
 * 
 * Worked on by Chris, David, and Jigar
 *
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "definitions.h"
#include <boost/asio.hpp>

using boost::asio::ip::udp; //for the game loop
using boost::asio::ip::tcp; //for registration

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_udp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

    udp::resolver resolver(io_service);
    udp::endpoint endpoint = *resolver.resolve({udp::v4(), argv[1], argv[2]});

    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_length = std::strlen(request);
    s.send_to(boost::asio::buffer(request, request_length), endpoint);

    char reply[max_length];
    udp::endpoint sender_endpoint;
    size_t reply_length = s.receive_from(
        boost::asio::buffer(reply, max_length), sender_endpoint);
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
