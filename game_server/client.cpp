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

      if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window* window;
    SDL_Renderer* renderer;


    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    /** Create window */
    window = SDL_CreateWindow("test_driving",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               1024 * 1, 768 * 1, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    /** Create renderer for window */
    renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    /** Initialize renderer color to white */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    struct timeval start, current;

    gettimeofday(&start, NULL);
    gettimeofday(&current, NULL);

    SDL_Event e;


    unordered_set<int> keysDown;
      unordered_set<char> buttonsDown;

      bool quit = false;

      TdBike bike = TdBike(200.0, 200.0, 500.0);

    while(true)
    { 
      char request[max_length];

      while (SDL_PollEvent(&e) != 0) {
          /* user requests QUIT */
          if (e.type == SDL_QUIT) {
              quit = true;
          } else if (e.type == SDL_KEYDOWN) {
              keysDown.insert(e.key.keysym.sym);
          } else if (e.type == SDL_KEYUP) {
              keysDown.erase(e.key.keysym.sym);
          } else if (e.type == SDL_MOUSEBUTTONDOWN) {
              buttonsDown.insert(e.button.button);
          } else if (e.type == SDL_MOUSEBUTTONUP) {
              buttonsDown.erase(e.button.button);
          }
      }

      /** update() block for keypresses */
      if (keysDown.count(SDLK_q)) {
          quit = true;
      }

      if(keysDown.count(SDLK_w)){
        request[0] = 'W';
      } else if (keysDown.count(SDLK_a)){
        request[0] = 'A';
      } else if (keysDown.count(SDLK_s)){
        request[0] = 'S';
      } else if (keysDown.count(SDLK_d)){
        request[0] = 'D';
      } else {
        request[0] = 'x';
      }
      

      size_t request_length = std::strlen(request);
      s.send_to(boost::asio::buffer(request, request_length), endpoint);

      char reply[max_length];
      udp::endpoint sender_endpoint;
      size_t reply_length = s.receive_from(
          boost::asio::buffer(reply, max_length), sender_endpoint);
      std::cout << "Returned Values: ";
      std::cout.write(reply, reply_length);
      std::cout << "Length: " << reply_length << std::endl;

      ptree response;
      std::istringstream is (reply);
      read_json (is, response);
      int code = response.get<int> ("code");
      float wheel = response.get<float> ("wheel");
      float vel = response.get<float> ("vel");

      bike.wheel = wheel;
      bike.vel = vel;

      /** Clear screen */
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

      SDL_Rect wholeRect = {25 * 1, 25 * 1, (1024 - 50) * 1, (768 - 50) * 1};
      SDL_RenderDrawRect(renderer, &wholeRect);

      bike.update();
      bike.draw(renderer);

      /** Update screen */
      SDL_RenderPresent(renderer);

      /** Update the surface */
      SDL_UpdateWindowSurface(window);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
