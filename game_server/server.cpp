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
#include <sstream>
#include <map>
//#include "definitions.h"
#include "TdBike.h"
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::udp; //for the game loop
using boost::asio::ip::tcp; //for registration
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

enum { max_length = 1024 };

/*void register(boost::asio::io_service& io_service, unsigned short port)
{
  //tcp::socket sock(io_service, tcp::endpoint(tcp::v4(), port));
}*/

void server(boost::asio::io_service& io_service, unsigned short port)
{
  //register(io_service, port);  

  udp::socket sock(io_service, udp::endpoint(udp::v4(), port));

  bool quit = false;
  int code = 0;
  TdBike bike = TdBike(200.0, 200.0, 500.0);

  while (true)
  {
    code = 0;

    char data[max_length];
    udp::endpoint sender_endpoint;
    size_t length = sock.receive_from(
        boost::asio::buffer(data, max_length), sender_endpoint);

    char input = data[0];

    switch(input) {
        case 'W' :
          if (bike.vel < 2.0) {
            bike.vel += 0.01;
          }
          break;
        case 'A' :
          if (bike.wheel < 1.6f) {
            bike.wheel += 0.02;
          }
          break;
        case 'S'  :
          if (bike.vel > 0.0) {
            bike.vel -= 0.0095;
          }
          break;
        case 'D' :
          if (bike.wheel > -1.6f) {
            bike.wheel -= 0.02;
          }
          break;
        default : 
          code = 1;
      }

      if (bike.vel > 0.0005) {
        bike.vel -= 0.0005;
      } else {
        bike.vel = 0;
      }

      std::cout << input << std::endl;

      ptree response;
      response.put("code", code);
      response.put("vel", (int)bike.vel);
      response.put("wheel", (int)bike.wheel);
      std::ostringstream buf;
      write_json (buf, response, false);
      std::string json = buf.str();
      length = json.length();
      json += '\0';

      std::cout << "JSON:" << json << "\n";


    sock.send_to(boost::asio::buffer(json, length+1), sender_endpoint);
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

