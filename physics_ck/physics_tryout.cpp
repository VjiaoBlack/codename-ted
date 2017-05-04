#include "physics_tryout.hpp"
#include <math.h>

using namespace std;

//Magic numbers
int firstOrderCollisionBuffer = 1;
int acceleration_normal_slowdown_factor = 1;
int acceleration_increase = 1;
float pioveroneeighty = 0.01745329251;
int turning_speed = 10;

//TODO factor in the delta thing to make this run framerate independent
gameMap main_loop(unordered_map<string, vector<string> > input_object, gameMap gm){

  /*********Apply Input**********/
  int iter;

  for(iter = 0; iter<gm.merchants.size(); iter++) { //Cycle through every merchant
    if(gm.merchants[iter].merchant_name == input_object["player_name"][0]){ //See if we have a match to the merchant name and the owner of the input
      int i=0;
      while(i < input_object["keystrokes"].size()){ //While we still have a valid input in the input vector
        switch (input_object["keystrokes"][i][0]){ // Read the first letter of the string, which with UP DOWN LEFT RIGHT will be unique
          case 'U':
            gm.merchants[iter].acceleration.x += acceleration_increase * (float)sin(gm.merchants[iter].rudderRot*pioveroneeighty);
            gm.merchants[iter].acceleration.y += acceleration_increase * (float)cos(gm.merchants[iter].rudderRot*pioveroneeighty);
            break;
          case 'D':
            gm.merchants[iter].acceleration.x -= acceleration_increase * (float)sin(gm.merchants[iter].rudderRot*pioveroneeighty);
            gm.merchants[iter].acceleration.y -= acceleration_increase * (float)cos(gm.merchants[iter].rudderRot*pioveroneeighty);
            break;
          case 'L':
            gm.merchants[iter].rudderRot -= turning_speed;
            break;
          case 'R':
            gm.merchants[iter].rudderRot += turning_speed;
            break;
        }
        i++;
      }
    }      
  }

  /********Update Game State*********/

  for(iter = 0; iter<gm.merchants.size(); iter++) {

    float currStr = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].currentStrength;
    float windStr = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].windStrength;
    float windDirx = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].windDirection.x;
    float windDiry = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].windDirection.y;
    float currDiry = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].currentDirection.y;
    float currDirx = gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].currentDirection.x;

    gm.merchants[iter].acceleration.x += currStr * currDirx;
    gm.merchants[iter].acceleration.y += currStr * currDiry;

    gm.merchants[iter].acceleration.x += windStr * windDirx;
    gm.merchants[iter].acceleration.y += windStr * windDiry;

    gm.merchants[iter].orientation += gm.merchants[iter].rudderRot;
    gm.merchants[iter].velocity.x += gm.merchants[iter].acceleration.x;
    gm.merchants[iter].velocity.y += gm.merchants[iter].acceleration.y;

    //Updating position, we also update the map tiles
    if(iter == 0){
      gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].is_ship = 0;
    }
    gm.merchants[iter].position.x += gm.merchants[iter].velocity.x;
    gm.merchants[iter].position.y += gm.merchants[iter].velocity.y;
    if(iter == 0)
      gm.mapTiles[gm.merchants[iter].position.x][gm.merchants[iter].position.y].is_ship = 1;

    //Normal slowdown of acceleration
    if(gm.merchants[iter].acceleration.x > 0)
      gm.merchants[iter].acceleration.x -= acceleration_normal_slowdown_factor;
    else{
      gm.merchants[iter].acceleration.x += acceleration_normal_slowdown_factor;
    }
    if(gm.merchants[iter].acceleration.y > 0)
      gm.merchants[iter].acceleration.y -= acceleration_normal_slowdown_factor;
    else{
      gm.merchants[iter].acceleration.y += acceleration_normal_slowdown_factor;
    }

    //Collision Detection.  This is barebones and I am sure box2D will do something better. This is of n^2 complexity so boohoo
    int jter;
    for(jter = 0; jter<gm.merchants.size(); jter++) {
      if(gm.merchants[iter].merchant_name == gm.merchants[jter].merchant_name){ // If we are checking collision with ourself
        continue;
      }
      else{ //Checking collision with another ship
        if((abs(gm.merchants[iter].position.x - gm.merchants[jter].position.x) < firstOrderCollisionBuffer) && 
           (abs(gm.merchants[iter].position.y - gm.merchants[jter].position.y) < firstOrderCollisionBuffer)){  // If this passes, do more granular collision.  This is barebones so I'll just treat this as a collision.
          //I am just going to reverse the accelerations for both ships to see if that fixes it.  I can do something more special later.
          printf("Collided\n");
          gm.merchants[iter].acceleration.x *= -1;
          gm.merchants[iter].acceleration.y *= -1;
          gm.merchants[jter].acceleration.x *= -1;
          gm.merchants[jter].acceleration.y *= -1;
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

void print_boat(gameMap gmap){
  float posx = gmap.merchants[0].position.x;
  float posy = gmap.merchants[0].position.y;
  float velx = gmap.merchants[0].velocity.x;
  float vely = gmap.merchants[0].velocity.y;
  float accx = gmap.merchants[0].acceleration.x;
  float accy = gmap.merchants[0].acceleration.y;

  printf("Name: %s\nPosition %f,%f\nVelocity %f,%f\nAcceleration %f,%f\n", gmap.merchants[0].merchant_name.c_str(), posx, posy, velx, vely, accx, accy);
}

int main(int argc, char* argv[]){
  gameMap map = create_random_map();
  map.print_game_map(); 

  map.merchants[0].merchant_name = "our_guy";

  // for(merchant m: map.merchants) { //Cycle through every merchant
  //   printf("%s\n", m.merchant_name.c_str());
  // }

  unordered_map<string, vector<string> > input_object;
  // Inserting data in std::map
  vector<string> msg;
  msg.push_back("our_guy");
  vector<string> content;
  content.push_back("UP");
  content.push_back("UP");
  content.push_back("UP");
  content.push_back("UP");
  input_object["player_name"] = msg;
  input_object["keystrokes"] = content;

  // printf("%s\n", input_object["player_name"][0].c_str());

  printf("\n\n\n");

  map = main_loop(input_object, map);

  map.print_game_map(); 

  return 0;
}