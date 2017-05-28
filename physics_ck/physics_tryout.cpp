#include "physics_tryout.hpp"

using namespace std;

//Magic numbers
float acceleration_normal_slowdown_factor = 0.05;
float acceleration_increase = 0.001;
float pioveroneeighty = 0.01745329251;
float turning_speed = .05;

float firstOrderCollisionBuffer = 0.1;
float y_extension = 1;
float x_extension = 1;

float acceleration_cap = .01;
float velocity_cap = .01;

int verbose = 0;

vec2 turn_orient_to_vec(int orientation){
    float x;
    float y;
    x = cos(orientation * pioveroneeighty);
    y = sin(orientation * pioveroneeighty);
    return vec2(x,y);
}

PiGameMap update_position(PiGameMap gm, int current_boat){

    //This next chunk of code can cause of a seg fault if I go off the map.  It'll try to read a tile that isn't there.  Therefore I added the if.

    // if(gm.merchants[current_boat].position.x < 25 &&
    //     gm.merchants[current_boat].position.y < 25 &&
    //     gm.merchants[current_boat].position.x > 0 &&
    //     gm.merchants[current_boat].position.x > 0){
    //     float currStr = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].currentStrength;
    //     float windStr = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].windStrength;
    //     float windDirx = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].windDirection.x;
    //     float windDiry = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].windDirection.y;
    //     float currDiry = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].currentDirection.y;
    //     float currDirx = gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].currentDirection.x;

    //     gm.merchants[current_boat].acceleration.x += (.0001) * currStr * currDirx;
    //     gm.merchants[current_boat].acceleration.y += (.0001) * currStr * currDiry;

    //     gm.merchants[current_boat].acceleration.x += (.0001) * windStr * windDirx;
    //     gm.merchants[current_boat].acceleration.y += (.0001) * windStr * windDiry;
    //   }

    //New velocity

    vec2 newvel = turn_orient_to_vec(gm.merchants[current_boat].orientation);

    printf("Our current direction: x: %f, y: %f\n", newvel.x, newvel.y);

    float newVelX = gm.merchants[current_boat].velocity.x + gm.merchants[current_boat].acceleration.x * newvel.x;
    float newVelY = gm.merchants[current_boat].velocity.y + gm.merchants[current_boat].acceleration.x * newvel.y;

    vec2 newVel = vec2(newVelX,newVelY);

    if(newVel.Length() < velocity_cap){
        gm.merchants[current_boat].velocity = newVel;
    }
    else{
        newVel = newVel.Normalize();
        newVel.x *= velocity_cap;
        newVel.y *= velocity_cap;
        gm.merchants[current_boat].velocity = newVel;
    }

    //Manually setting new positions
    gm.merchants[current_boat].coord_pos.x += gm.merchants[current_boat].velocity.x;
    gm.merchants[current_boat].coord_pos.y += gm.merchants[current_boat].velocity.y;

    //Moving the merchant to the appropriate tile
    vec2 newLoc = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
    gm.merchants[current_boat].position = convert_coord_tile(gm, newLoc);

    return gm;
}

bool is_colliding(vec2 a, vec2 b) {

    if(verbose)
      printf("Boat one x: %f,Boat one y: %f,Boat two x: %f,Boat two y: %f\n", a.x, a.y, b.x, b.y);

    if(((a.x + x_extension + firstOrderCollisionBuffer) > (b.x - x_extension)) && ((a.x - x_extension) < (b.x - x_extension))){
      if(verbose)
        printf("Right wall of a is inside of b\n");
      return 1;
    }
    if(((a.x - x_extension - firstOrderCollisionBuffer) < (b.x + x_extension)) && ((a.x + x_extension) > (b.x + x_extension))){
      if(verbose)
        printf("Left wall of a is inside b");
      return 1;
    }
    if(((a.y + y_extension + firstOrderCollisionBuffer) > (b.y - y_extension)) && ((a.y - y_extension) < (b.y - y_extension))){
      if(verbose)
        printf("Top wall of a is inside b");
      return 1;
    }
    if(((a.y - y_extension - firstOrderCollisionBuffer) < (b.y + y_extension)) && ((a.y + y_extension) > (b.y + y_extension))){
      if(verbose)
        printf("Bottom wall of a is inside b");
      return 1;
    }
    if((a.y == b.y) && (a.x == b.x)){
      if(verbose)
        printf("Right on top of each other, probably due to initalizing in the same place\n");
      return 1;
    }
    return 0;
}

void draw_boat(SDL_Renderer* renderer, PiGameMap gm) {

    SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);

    int x1 = gm.merchants[0].coord_pos.x + x_extension * (float)cos(gm.merchants[0].orientation*pioveroneeighty);
    int y1 = gm.merchants[0].coord_pos.y + y_extension * (float)sin(gm.merchants[0].orientation*pioveroneeighty);

    int x2 = gm.merchants[0].coord_pos.x - x_extension * (float)cos(gm.merchants[0].orientation*pioveroneeighty);
    int y2 = gm.merchants[0].coord_pos.y - y_extension * (float)sin(gm.merchants[0].orientation*pioveroneeighty);

    SDL_RenderDrawLine(renderer, x1 * 2, y1 * 2, x2 * 2, y2 * 2);
}

PiGameMap compute_gamestate(unordered_map<string, vector<string> > input_object, PiGameMap gm){
  /*********Apply Input**********/
  int current_boat;
  float newAcc = 0;

  for(current_boat = 0; current_boat<gm.merchants.size(); current_boat++) { //Cycle through every merchant
    if(gm.merchants[current_boat].merchant_name == input_object["player_name"][0]){ //See if we have a match to the merchant name and the owner of the input
      int i=0;
      while(i < input_object["keystrokes"].size()){ //While we still have a valid input in the input vector
        switch (input_object["keystrokes"][i][0]){ // Read the first letter of the string, which with UP DOWN LEFT RIGHT will be unique
          case 'U':
            newAcc = gm.merchants[current_boat].acceleration.x + acceleration_increase;
            if(newAcc < acceleration_cap){
                gm.merchants[current_boat].acceleration.x = newAcc;
            }
            break;
          case 'D':
            newAcc = gm.merchants[current_boat].acceleration.x - acceleration_increase;
            if(newAcc > -(acceleration_cap)){
                gm.merchants[current_boat].acceleration.x = newAcc;
            }
            break;
          case 'L':
            printf("L Is this happening\n");
            gm.merchants[current_boat].orientation -= turning_speed;
            gm.merchants[current_boat].orientation = fmod(gm.merchants[current_boat].orientation,360);
            break;
          case 'R':
            printf("R Is this happening\n");
            gm.merchants[current_boat].orientation += turning_speed;
            gm.merchants[current_boat].orientation = fmod(gm.merchants[current_boat].orientation,360);
            break;
        }
        i++;
      }
    }
  }

  /********Update Game State*********/

  for(current_boat = 0; current_boat<gm.merchants.size(); current_boat++) {

    gm = update_position(gm, current_boat);

    //Collision Detection.  This is barebones and I am sure box2D will do something better. This is of n^2 complexity so boohoo
    int boat_to_compare;
    for(boat_to_compare = 0; boat_to_compare<gm.merchants.size(); boat_to_compare++) {
      //printf("************************************Check for boat %d against boat %d\n", current_boat, boat_to_compare);
      if(current_boat==boat_to_compare){ // If we are checking collision with ourself
        //printf("This is a check with myself\n");
        continue;
      }
      else{ //Checking collision with another ship
        if(is_colliding(gm.merchants[current_boat].coord_pos, gm.merchants[boat_to_compare].coord_pos)){
          // If this passes, do more granular collision.  This is barebones so I'll just treat this as a collision.
          //I am just going to reverse the accelerations for both ships to see if that fixes it.  I can do something more special later.
          if(verbose)
            printf("Collided\n");
          gm.merchants[current_boat].acceleration.x *= -1;
          gm.merchants[current_boat].acceleration.y *= -1;
          gm.merchants[boat_to_compare].acceleration.x *= -1;
          gm.merchants[boat_to_compare].acceleration.y *= -1;
        }
        else{ // Not close to each other so pass.
          continue;
        }
      }
    }
    //TODO collision with land
  }
  return gm;
}

void print_boat(PiGameMap gmap){
  float posx = gmap.merchants[0].coord_pos.x;
  float posy = gmap.merchants[0].coord_pos.y;
  float velx = gmap.merchants[0].velocity.x;
  float vely = gmap.merchants[0].velocity.y;
  float accx = gmap.merchants[0].acceleration.x;
  float accy = gmap.merchants[0].acceleration.y;
  float orient = gmap.merchants[0].orientation;
  float rot = gmap.merchants[0].rudderRot;

  if(verbose)
    printf("Name: %s\nPosition %f,%f\nVelocity %f,%f\nAcceleration %f,%f\nOrientation: %f\n, rudderRot: %f\n", gmap.merchants[0].merchant_name.c_str(), posx, posy, velx, vely, accx, accy, orient,rot);
}

/*
int main(int argc, char* argv[]){
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
  // Create window
  window = SDL_CreateWindow("test_driving",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             1024 * 1, 768 * 1, SDL_WINDOW_SHOWN);
  if (window == NULL) {
      printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
      exit(1);
  }

  // Create renderer for window
  renderer = SDL_CreateRenderer(window, -1,
                                              SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
      printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
      exit(1);
  }
  // Initialize renderer color to white
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  struct timeval start, current;

  gettimeofday(&start, NULL);
  gettimeofday(&current, NULL);

  SDL_Event e;

  unordered_set<int> keysDown;
  unordered_set<char> buttonsDown;

  bool quit = false;

  PiGameMap map(25);
  map.add_merchant(PiMerchant());
  map.merchants[0].merchant_name = "our_guy";

  // for(merchant m: map.merchants) { //Cycle through every merchant
  //   printf("%s\n", m.merchant_name.c_str());
  // }

  //framerate limiter
  milliseconds now = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );
  milliseconds lastFrame = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  //printf("here we have it%lld\n", lastFrame.count());

  while(!quit) {

    unordered_map<string, vector<string> > input_object;
    // Inserting data in std::map
    vector<string> msg;
    msg.push_back("our_guy");
    vector<string> content;
    input_object["player_name"] = msg;

    now = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    long long int delta = now.count() - lastFrame.count();
    lastFrame = now;

    if(delta < 33)
    {
        usleep(33 - delta);
    }

    while (SDL_PollEvent(&e) != 0) {
        // user requests QUIT
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

    // update() block for keypresses
    if (keysDown.count(SDLK_q)) {
        quit = true;
    }

    // TODO make sure the angles don't just keep increasing infinitely
    if (keysDown.count(SDLK_LEFT)) {
      content.push_back("LEFT");
      input_object["keystrokes"] = content;
      map = compute_gamestate(input_object, map);
      if(verbose)
        map.print_game_map();
      print_boat(map);
      if(verbose)
        printf("\n\n\n");
    }
    if (keysDown.count(SDLK_RIGHT)) {
      content.push_back("RIGHT");
      input_object["keystrokes"] = content;
      map = compute_gamestate(input_object, map);
      if(verbose)
        map.print_game_map();
      print_boat(map);
      if(verbose)
        printf("\n\n\n");
    }

    if (keysDown.count(SDLK_UP)) {
      content.push_back("UP");
      input_object["keystrokes"] = content;
      map = compute_gamestate(input_object, map);
      if(verbose)
        map.print_game_map();
      print_boat(map);
      if(verbose)
        printf("\n\n\n");
    }

    if (keysDown.count(SDLK_DOWN)) {
      content.push_back("DOWN");
      input_object["keystrokes"] = content;
      map = compute_gamestate(input_object, map);
      if(verbose)
        map.print_game_map();
      print_boat(map);
      if(verbose)
        printf("\n\n\n");
    }

    //I want it to run regardless
    input_object["keystrokes"] = content;
      map = compute_gamestate(input_object, map);
      if(verbose)
        map.print_game_map();
      print_boat(map);
      if(verbose)
        printf("\n\n\n");

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    SDL_Rect wholeRect = {25 * 1, 25 * 1, (1024 - 50) * 1, (768 - 50) * 1};
    SDL_RenderDrawRect(renderer, &wholeRect);

    draw_boat(renderer,map);

    // Update screen
    SDL_RenderPresent(renderer);

    // Update the surface
    SDL_UpdateWindowSurface(window);

      // usleep(3000);
  }

  return 0;
}*/