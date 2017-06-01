#include "physics_tryout.hpp"

using namespace std;

//Magic numbers
float acceleration_normal_slowdown_factor = 0.05;
float acceleration_increase = 0.001;
float pioveroneeighty = 0.01745329251;
float turning_speed = .1 * 4.0;

float firstOrderCollisionBuffer = 0.1;
float y_extension = 1 * 2.0;
float x_extension = 2 * 2.0;

float acceleration_cap = .01 * 5.0;
float velocity_cap = .06 * 5.0;

int gold_cap = 1000;

int verbose = 0;

float gold_payout = 1024;

int tickCount = 0;

int tunt = 1;

float engineMultiplier = 1.0f;

int merchantsColliding[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int piratesColliding[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

vec2 turn_orient_to_vec(int orientation){
    float x;
    float y;
    x = cos(orientation * pioveroneeighty);
    y = sin(orientation * pioveroneeighty);
    return vec2(x,y);
}

float dot_product(vec2 a, vec2 b){
    return a.x*b.x + a.y*b.y;
}

float gold_vel_cap(PiGameMap gm, int current_boat){
    return velocity_cap + velocity_cap * ((gold_cap - gm.merchants[current_boat].goldAmount) / gold_cap);
}

float gold_vel_cap_pirate(PiGameMap gm, int pirate){
    return velocity_cap + velocity_cap * ((gold_cap - gm.pirates[pirate].goldAmount) / gold_cap);
}

int array_empty_checker(int array[]){
    for(int i = 0; i<25; i++){
        if(array[i]==1){
            return 0;
        }
    }
    return 1;
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

    vec2 boat_direction = turn_orient_to_vec(gm.merchants[current_boat].orientation);

    //printf("Our current direction: x: %f, y: %f\n", boat_direction.x, boat_direction.y);

    if(gm.merchants[current_boat].acceleration.x < 0){ // Alternative slowdown to prevent instant reverse
        gm.merchants[current_boat].velocity.x *= .9 * engineMultiplier;
        gm.merchants[current_boat].velocity.y *= .9 * engineMultiplier;
    }
    else{
        float newVelX = gm.merchants[current_boat].velocity.x + gm.merchants[current_boat].acceleration.x * boat_direction.x * engineMultiplier;
        float newVelY = gm.merchants[current_boat].velocity.y + gm.merchants[current_boat].acceleration.x * boat_direction.y * engineMultiplier;

        vec2 newVel = vec2(newVelX,newVelY);

        if(newVel.Length() < gold_vel_cap(gm, current_boat)){
            gm.merchants[current_boat].velocity = newVel;
        }
        else{
            newVel = newVel.Normalize();
            newVel.x *= gold_vel_cap(gm, current_boat);
            newVel.y *= gold_vel_cap(gm, current_boat);
            gm.merchants[current_boat].velocity = newVel;
        }
    }

        //Manually setting new positions
    gm.merchants[current_boat].coord_pos.x += gm.merchants[current_boat].velocity.x * engineMultiplier;
    gm.merchants[current_boat].coord_pos.y += gm.merchants[current_boat].velocity.y * engineMultiplier;

    if(gm.merchants[current_boat].coord_pos.x < 50){
        gm.merchants[current_boat].coord_pos.x = 100;
    }
    if(gm.merchants[current_boat].coord_pos.y < 50){
        gm.merchants[current_boat].coord_pos.y = 100;
    }
    if(gm.merchants[current_boat].coord_pos.x > 29950){
        gm.merchants[current_boat].coord_pos.x = 29900;
    }
    if(gm.merchants[current_boat].coord_pos.y > 29950){
        gm.merchants[current_boat].coord_pos.y = 29900;
    }


    //printf("x: %f, y: %f\n", gm.merchants[current_boat].coord_pos.x, gm.merchants[current_boat].coord_pos.y);

        //Moving the merchant to the appropriate tile
    vec2 newLoc = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
    gm.merchants[current_boat].position = convert_coord_tile(gm, newLoc);

    //printf("We are going this fast: %f\n",gm.merchants[current_boat].velocity.Length());

    //Optional dampening of velocity and acceleration:

    // gm.merchants[current_boat].acceleration.x *= .99 * engineMultiplier;
    // gm.merchants[current_boat].velocity.x *= .99 * engineMultiplier;
    // gm.merchants[current_boat].velocity.y *= .99 * engineMultiplier;

    //This code is for smoothing turns due to collisions.  The boat's true angle is orientation, and rudderRot is the goal.
    if(gm.merchants[current_boat].orientation != gm.merchants[current_boat].rudderRot){
        if(fabs(gm.merchants[current_boat].orientation - gm.merchants[current_boat].rudderRot) < 1){
            gm.merchants[current_boat].orientation = gm.merchants[current_boat].rudderRot;
        }
        if(gm.merchants[current_boat].orientation < gm.merchants[current_boat].rudderRot){
            gm.merchants[current_boat].orientation += 1;
        }
        if(gm.merchants[current_boat].orientation > gm.merchants[current_boat].rudderRot){
            gm.merchants[current_boat].orientation -= 1;
        }
    }

    //Here is the jankiest land collision code of all time!

    if(gm.merchants[current_boat].position.x < 0 || gm.merchants[current_boat].position.y < 0){
        printf("*****************************************************************find me Now\n");
    }

    // int mapTileIter = 0;
    // int anotherMapTileIter = 0;
    // for(mapTileIter = 0; mapTileIter<gm.mapTiles.size(); mapTileIter++){
    //     for(anotherMapTileIter = 0; anotherMapTileIter<gm.mapTiles[mapTileIter].size(); anotherMapTileIter++){
    //         if(gm.mapTiles[mapTileIter][anotherMapTileIter].land_water == 1){
    //             printf("LAND HO\n");
    //         }
    //     }
    // }


    while(gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].land_water == 1){

        gm.merchants[current_boat].coord_pos.x -= gm.merchants[current_boat].velocity.x * engineMultiplier;
        gm.merchants[current_boat].coord_pos.y -= gm.merchants[current_boat].velocity.y * engineMultiplier;
        //printf("I'm on land\n\n");
        vec2 landLocked = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
        gm.merchants[current_boat].position = convert_coord_tile(gm, landLocked);

        if(gm.merchants[current_boat].position.x < 0 || gm.merchants[current_boat].position.y < 0){
            printf("*****************************************************************find me guy\n");
            break;
        }
    }

    return gm;
}

PiGameMap update_position_pirate(PiGameMap gm, int pirate){
    //Manually setting new positions

    // printf("\n\n\n\n\nPirate Velocity x: %f, y: %f\n", gm.pirates[pirate].velocity.x, gm.pirates[pirate].velocity.y);
    // printf("Pirate pos x: %f, y: %f\n\n\n\n\n\n\n", gm.pirates[pirate].coord_pos.x, gm.pirates[pirate].coord_pos.y);
    

    gm.pirates[0].coord_pos.x += gm.pirates[0].velocity.Normalize().x * gold_vel_cap_pirate(gm,0) * engineMultiplier;
    gm.pirates[0].coord_pos.y += gm.pirates[0].velocity.Normalize().y * gold_vel_cap_pirate(gm,0) * engineMultiplier;



    if(gm.pirates[0].coord_pos.x < 50){
        gm.pirates[0].coord_pos.x = 100;
    }
    if(gm.pirates[0].coord_pos.y < 50){
        gm.pirates[0].coord_pos.y = 100;
    }

    if(gm.pirates[0].coord_pos.x > 29950){
        gm.pirates[0].coord_pos.x = 29900;
    }
    if(gm.pirates[0].coord_pos.y > 29950){
        gm.pirates[0].coord_pos.y = 29900;
    }


    //Moving the pirate to the appropriate tile
    vec2 newLoc = vec2(gm.pirates[0].coord_pos.x,gm.pirates[0].coord_pos.y);
    gm.pirates[0].position = convert_coord_tile(gm, newLoc);

    gm.pirates[0].rudderRot = atan2(gm.pirates[0].velocity.y, gm.pirates[0].velocity.x) * 1/pioveroneeighty;

    //This code is for smoothing turns due to collisions.  The boat's true angle is orientation, and rudderRot is the goal.
    if(gm.pirates[0].orientation != gm.pirates[0].rudderRot){
        if(fabs(gm.pirates[0].orientation - gm.pirates[0].rudderRot) < 1){
            gm.pirates[0].orientation = gm.pirates[0].rudderRot;
        }
        else if(gm.pirates[0].orientation < gm.pirates[0].rudderRot){
            gm.pirates[0].orientation += 1;
        }
        else if(gm.pirates[0].orientation > gm.pirates[0].rudderRot){
            gm.pirates[0].orientation -= 1;
        }
    }

    if(gm.pirates[pirate].position.x < 0 || gm.pirates[pirate].position.y < 0){
        printf("*****************************************************************find me piratenowwww\n");
    }
        while(gm.mapTiles[gm.pirates[0].position.x][gm.pirates[0].position.y].land_water == 1){
            gm.pirates[0].coord_pos.x -= gm.pirates[0].velocity.x * engineMultiplier;
            gm.pirates[0].coord_pos.y -= gm.pirates[0].velocity.y * engineMultiplier;

            vec2 landLocked = vec2(gm.pirates[0].coord_pos.x,gm.pirates[0].coord_pos.y);
            gm.pirates[0].position = convert_coord_tile(gm, landLocked);

            if(gm.pirates[pirate].position.x < 0 || gm.pirates[pirate].position.y < 0){
                printf("*****************************************************************find me pirate\n");
                break;
            }
        }

    return gm;
}

bool is_colliding(PiGameMap gm, int current_boat, int boat_to_compare, int use_buffer) {

    float buffer = 0;

    if(use_buffer == 1){
        buffer = 5;
    }

    //Computing the four corners of the current_boat

    vec2 boat_direction = turn_orient_to_vec(gm.merchants[current_boat].orientation);
    boat_direction = boat_direction.Normalize();

    vec2 perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    float xx = ((buffer + x_extension) * boat_direction.x * 5);
    float xy = ((buffer + x_extension) * boat_direction.y * 5);
    float yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    float yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float ax1 = gm.merchants[current_boat].coord_pos.x + xx + yx;
    float ay1 = gm.merchants[current_boat].coord_pos.y + xy + yy;

    float ax2 = gm.merchants[current_boat].coord_pos.x + xx - yx;
    float ay2 = gm.merchants[current_boat].coord_pos.y + xy - yy;

    float ax3 = gm.merchants[current_boat].coord_pos.x - xx + yx;
    float ay3 = gm.merchants[current_boat].coord_pos.y - xy + yy;

    float ax4 = gm.merchants[current_boat].coord_pos.x - xx - yx;
    float ay4 = gm.merchants[current_boat].coord_pos.y - xy - yy;

    //Computing the four corners of the second boat

    boat_direction = turn_orient_to_vec(gm.merchants[boat_to_compare].orientation);
    boat_direction = boat_direction.Normalize();

    perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    xx = ((buffer + x_extension) * boat_direction.x * 5);
    xy = ((buffer + x_extension) * boat_direction.y * 5);
    yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float bx1 = gm.merchants[boat_to_compare].coord_pos.x + xx + yx;
    float by1 = gm.merchants[boat_to_compare].coord_pos.y + xy + yy;

    float bx2 = gm.merchants[boat_to_compare].coord_pos.x + xx - yx;
    float by2 = gm.merchants[boat_to_compare].coord_pos.y + xy - yy;

    float bx3 = gm.merchants[boat_to_compare].coord_pos.x - xx + yx;
    float by3 = gm.merchants[boat_to_compare].coord_pos.y - xy + yy;

    float bx4 = gm.merchants[boat_to_compare].coord_pos.x - xx - yx;
    float by4 = gm.merchants[boat_to_compare].coord_pos.y - xy - yy;

    float d12, d13, d24, d34;

    // I will now check to see if there exists a vertex of the current boat that is within all 4 sides of the other boat.
    //Testing ax1 ay1 against the first wall x1y1 x2y2
    d12 = (ax1-bx1)*(by2-by1)-(ay1-by1)*(bx2-bx1);
    //1 3
    d13 = (ax1-bx1)*(by3-by1)-(ay1-by1)*(bx3-bx1);
    //2 4
    d24 = (ax1-bx2)*(by4-by2)-(ay1-by2)*(bx4-bx2);
    //3 4
    d34 = (ax1-bx3)*(by4-by3)-(ay1-by3)*(bx4-bx3);

    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax2-bx1)*(by2-by1)-(ay2-by1)*(bx2-bx1);
    d13 = (ax2-bx1)*(by3-by1)-(ay2-by1)*(bx3-bx1);
    d24 = (ax2-bx2)*(by4-by2)-(ay2-by2)*(bx4-bx2);
    d34 = (ax2-bx3)*(by4-by3)-(ay2-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax3-bx1)*(by2-by1)-(ay3-by1)*(bx2-bx1);
    d13 = (ax3-bx1)*(by3-by1)-(ay3-by1)*(bx3-bx1);
    d24 = (ax3-bx2)*(by4-by2)-(ay3-by2)*(bx4-bx2);
    d34 = (ax3-bx3)*(by4-by3)-(ay3-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax4-bx1)*(by2-by1)-(ay4-by1)*(bx2-bx1);
    d13 = (ax4-bx1)*(by3-by1)-(ay4-by1)*(bx3-bx1);
    d24 = (ax4-bx2)*(by4-by2)-(ay4-by2)*(bx4-bx2);
    d34 = (ax4-bx3)*(by4-by3)-(ay4-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    return 0;
}

//Yes I know this is a waste of space but the above function works so well that I will just reproduce and rename some variables for a pirate.
bool is_colliding_boat_pirate(PiGameMap gm, int current_boat, int use_buffer) {

    float buffer = 0;

    if(use_buffer == 1){
        buffer = 5;
    }

    //Computing the four corners of the current_boat

    vec2 boat_direction = turn_orient_to_vec(gm.merchants[current_boat].orientation);
    boat_direction = boat_direction.Normalize();

    vec2 perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    float xx = ((buffer + x_extension) * boat_direction.x * 5);
    float xy = ((buffer + x_extension) * boat_direction.y * 5);
    float yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    float yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float ax1 = gm.merchants[current_boat].coord_pos.x + xx + yx;
    float ay1 = gm.merchants[current_boat].coord_pos.y + xy + yy;

    float ax2 = gm.merchants[current_boat].coord_pos.x + xx - yx;
    float ay2 = gm.merchants[current_boat].coord_pos.y + xy - yy;

    float ax3 = gm.merchants[current_boat].coord_pos.x - xx + yx;
    float ay3 = gm.merchants[current_boat].coord_pos.y - xy + yy;

    float ax4 = gm.merchants[current_boat].coord_pos.x - xx - yx;
    float ay4 = gm.merchants[current_boat].coord_pos.y - xy - yy;

    //Computing the four corners of the second boat

    boat_direction = turn_orient_to_vec(gm.pirates[0].orientation);
    boat_direction = boat_direction.Normalize();

    perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    xx = ((buffer + x_extension) * boat_direction.x * 5);
    xy = ((buffer + x_extension) * boat_direction.y * 5);
    yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float bx1 = gm.pirates[0].coord_pos.x + xx + yx;
    float by1 = gm.pirates[0].coord_pos.y + xy + yy;

    float bx2 = gm.pirates[0].coord_pos.x + xx - yx;
    float by2 = gm.pirates[0].coord_pos.y + xy - yy;

    float bx3 = gm.pirates[0].coord_pos.x - xx + yx;
    float by3 = gm.pirates[0].coord_pos.y - xy + yy;

    float bx4 = gm.pirates[0].coord_pos.x - xx - yx;
    float by4 = gm.pirates[0].coord_pos.y - xy - yy;

    float d12, d13, d24, d34;

    // I will now check to see if there exists a vertex of the current boat that is within all 4 sides of the other boat.
    //Testing ax1 ay1 against the first wall x1y1 x2y2
    d12 = (ax1-bx1)*(by2-by1)-(ay1-by1)*(bx2-bx1);
    //1 3
    d13 = (ax1-bx1)*(by3-by1)-(ay1-by1)*(bx3-bx1);
    //2 4
    d24 = (ax1-bx2)*(by4-by2)-(ay1-by2)*(bx4-bx2);
    //3 4
    d34 = (ax1-bx3)*(by4-by3)-(ay1-by3)*(bx4-bx3);

    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax2-bx1)*(by2-by1)-(ay2-by1)*(bx2-bx1);
    d13 = (ax2-bx1)*(by3-by1)-(ay2-by1)*(bx3-bx1);
    d24 = (ax2-bx2)*(by4-by2)-(ay2-by2)*(bx4-bx2);
    d34 = (ax2-bx3)*(by4-by3)-(ay2-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax3-bx1)*(by2-by1)-(ay3-by1)*(bx2-bx1);
    d13 = (ax3-bx1)*(by3-by1)-(ay3-by1)*(bx3-bx1);
    d24 = (ax3-bx2)*(by4-by2)-(ay3-by2)*(bx4-bx2);
    d34 = (ax3-bx3)*(by4-by3)-(ay3-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax4-bx1)*(by2-by1)-(ay4-by1)*(bx2-bx1);
    d13 = (ax4-bx1)*(by3-by1)-(ay4-by1)*(bx3-bx1);
    d24 = (ax4-bx2)*(by4-by2)-(ay4-by2)*(bx4-bx2);
    d34 = (ax4-bx3)*(by4-by3)-(ay4-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    return 0;
}

bool is_colliding_pirate_boat(PiGameMap gm, int boat_to_compare, int use_buffer) {

    float buffer = 0;

    if(use_buffer == 1){
        buffer = 10;
    }

    //Computing the four corners of the current_boat

    vec2 boat_direction = turn_orient_to_vec(gm.pirates[0].orientation);
    boat_direction = boat_direction.Normalize();

    vec2 perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    float xx = ((buffer + x_extension) * boat_direction.x * 5);
    float xy = ((buffer + x_extension) * boat_direction.y * 5);
    float yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    float yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float ax1 = gm.pirates[0].coord_pos.x + xx + yx;
    float ay1 = gm.pirates[0].coord_pos.y + xy + yy;

    float ax2 = gm.pirates[0].coord_pos.x + xx - yx;
    float ay2 = gm.pirates[0].coord_pos.y + xy - yy;

    float ax3 = gm.pirates[0].coord_pos.x - xx + yx;
    float ay3 = gm.pirates[0].coord_pos.y - xy + yy;

    float ax4 = gm.pirates[0].coord_pos.x - xx - yx;
    float ay4 = gm.pirates[0].coord_pos.y - xy - yy;

    //Computing the four corners of the second boat

    boat_direction = turn_orient_to_vec(gm.merchants[boat_to_compare].orientation);
    boat_direction = boat_direction.Normalize();

    perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    xx = ((buffer + x_extension) * boat_direction.x * 5);
    xy = ((buffer + x_extension) * boat_direction.y * 5);
    yy = ((buffer + y_extension) * perpindicular_direction.y * 5);
    yx = ((buffer + y_extension) * perpindicular_direction.x * 5);

    float bx1 = gm.merchants[boat_to_compare].coord_pos.x + xx + yx;
    float by1 = gm.merchants[boat_to_compare].coord_pos.y + xy + yy;

    float bx2 = gm.merchants[boat_to_compare].coord_pos.x + xx - yx;
    float by2 = gm.merchants[boat_to_compare].coord_pos.y + xy - yy;

    float bx3 = gm.merchants[boat_to_compare].coord_pos.x - xx + yx;
    float by3 = gm.merchants[boat_to_compare].coord_pos.y - xy + yy;

    float bx4 = gm.merchants[boat_to_compare].coord_pos.x - xx - yx;
    float by4 = gm.merchants[boat_to_compare].coord_pos.y - xy - yy;

    float d12, d13, d24, d34;

    // I will now check to see if there exists a vertex of the current boat that is within all 4 sides of the other boat.
    //Testing ax1 ay1 against the first wall x1y1 x2y2
    d12 = (ax1-bx1)*(by2-by1)-(ay1-by1)*(bx2-bx1);
    //1 3
    d13 = (ax1-bx1)*(by3-by1)-(ay1-by1)*(bx3-bx1);
    //2 4
    d24 = (ax1-bx2)*(by4-by2)-(ay1-by2)*(bx4-bx2);
    //3 4
    d34 = (ax1-bx3)*(by4-by3)-(ay1-by3)*(bx4-bx3);

    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax2-bx1)*(by2-by1)-(ay2-by1)*(bx2-bx1);
    d13 = (ax2-bx1)*(by3-by1)-(ay2-by1)*(bx3-bx1);
    d24 = (ax2-bx2)*(by4-by2)-(ay2-by2)*(bx4-bx2);
    d34 = (ax2-bx3)*(by4-by3)-(ay2-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax3-bx1)*(by2-by1)-(ay3-by1)*(bx2-bx1);
    d13 = (ax3-bx1)*(by3-by1)-(ay3-by1)*(bx3-bx1);
    d24 = (ax3-bx2)*(by4-by2)-(ay3-by2)*(bx4-bx2);
    d34 = (ax3-bx3)*(by4-by3)-(ay3-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    d12 = (ax4-bx1)*(by2-by1)-(ay4-by1)*(bx2-bx1);
    d13 = (ax4-bx1)*(by3-by1)-(ay4-by1)*(bx3-bx1);
    d24 = (ax4-bx2)*(by4-by2)-(ay4-by2)*(bx4-bx2);
    d34 = (ax4-bx3)*(by4-by3)-(ay4-by3)*(bx4-bx3);
    if( d12 > 0 && d13 < 0 && d24 > 0 && d34 < 0){
        return 1;
    }

    return 0;
}

void draw_boat(SDL_Renderer* renderer, PiGameMap gm, int current_boat) {

    vec2 boat_direction = turn_orient_to_vec(gm.merchants[current_boat].orientation);
    boat_direction = boat_direction.Normalize();

    vec2 perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    float xx = (x_extension * boat_direction.x * 5);
    float xy = (x_extension * boat_direction.y * 5);
    float yy = (y_extension * perpindicular_direction.y * 5);
    float yx = (y_extension * perpindicular_direction.x * 5);

    int x1 = gm.merchants[current_boat].coord_pos.x + xx + yx;
    int y1 = gm.merchants[current_boat].coord_pos.y + xy + yy;

    int x2 = gm.merchants[current_boat].coord_pos.x + xx - yx;
    int y2 = gm.merchants[current_boat].coord_pos.y + xy - yy;

    int x3 = gm.merchants[current_boat].coord_pos.x - xx + yx;
    int y3 = gm.merchants[current_boat].coord_pos.y - xy + yy;

    int x4 = gm.merchants[current_boat].coord_pos.x - xx - yx;
    int y4 = gm.merchants[current_boat].coord_pos.y - xy - yy;

    SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    SDL_RenderDrawLine(renderer, x1, y1, x3, y3);

    SDL_RenderDrawLine(renderer, x2, y2, x4, y4);

    SDL_RenderDrawLine(renderer, x3, y3, x4, y4);

    SDL_RenderDrawPoint(renderer,gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
}

void draw_pirate(SDL_Renderer* renderer, PiGameMap gm, int current_boat) {

    vec2 boat_direction = turn_orient_to_vec(gm.pirates[0].orientation);
    boat_direction = boat_direction.Normalize();

    vec2 perpindicular_direction = vec2(-boat_direction.y, boat_direction.x);
    perpindicular_direction = perpindicular_direction.Normalize();

    float xx = (x_extension * boat_direction.x * 5);
    float xy = (x_extension * boat_direction.y * 5);
    float yy = (y_extension * perpindicular_direction.y * 5);
    float yx = (y_extension * perpindicular_direction.x * 5);

    int x1 = gm.pirates[0].coord_pos.x + xx + yx;
    int y1 = gm.pirates[0].coord_pos.y + xy + yy;

    int x2 = gm.pirates[0].coord_pos.x + xx - yx;
    int y2 = gm.pirates[0].coord_pos.y + xy - yy;

    int x3 = gm.pirates[0].coord_pos.x - xx + yx;
    int y3 = gm.pirates[0].coord_pos.y - xy + yy;

    int x4 = gm.pirates[0].coord_pos.x - xx - yx;
    int y4 = gm.pirates[0].coord_pos.y - xy - yy;

    SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    SDL_RenderDrawLine(renderer, x1, y1, x3, y3);

    SDL_RenderDrawLine(renderer, x2, y2, x4, y4);

    SDL_RenderDrawLine(renderer, x3, y3, x4, y4);

    SDL_RenderDrawPoint(renderer,gm.pirates[0].coord_pos.x,gm.pirates[0].coord_pos.y);
}

PiGameMap compute_gamestate(unordered_map<string, vector<string> > input_object, PiGameMap gm){
    /*********Apply Input**********/
    int current_boat;
    float newAcc = 0;

    for(current_boat = 0; current_boat<gm.merchants.size(); current_boat++) { //Cycle through every merchant
        if((gm.merchants[current_boat].merchant_name == input_object["player_name"][0]) && gm.merchants[current_boat].AI){ //See if we have a match to the merchant name and the owner of the input
            int i=0;
            while(i < input_object["keystrokes"].size()){ //While we still have a valid input in the input vector
                switch (input_object["keystrokes"][i][0]){ // Read the first letter of the string, which with UP DOWN LEFT RIGHT will be unique
                    case 'U':
                    newAcc = gm.merchants[current_boat].acceleration.x + acceleration_increase * engineMultiplier;
                    if(newAcc < acceleration_cap){
                        gm.merchants[current_boat].acceleration.x = newAcc;
                    }
                    break;
                    case 'D':
                    newAcc = gm.merchants[current_boat].acceleration.x - acceleration_increase * engineMultiplier;
                    if(newAcc > -(acceleration_cap)){
                        gm.merchants[current_boat].acceleration.x = newAcc;
                    }
                    break;
                    case 'L':
                    // printf("L Is this happening\n");
                    gm.merchants[current_boat].orientation -= turning_speed * engineMultiplier;
                    gm.merchants[current_boat].orientation = fmod(gm.merchants[current_boat].orientation,360);
                    gm.merchants[current_boat].rudderRot = gm.merchants[current_boat].orientation;
                    break;
                    case 'R':
                    // printf("R Is this happening\n");
                    gm.merchants[current_boat].orientation += turning_speed * engineMultiplier;
                    gm.merchants[current_boat].orientation = fmod(gm.merchants[current_boat].orientation,360);
                    gm.merchants[current_boat].rudderRot = gm.merchants[current_boat].orientation;
                    break;
                    case 'G':
                    if(gm.merchants[current_boat].goldAmount > 10){
                        gm.merchants[current_boat].goldAmount -= 1;
                    }
                    break;
                }
                i++;
            }
        }
    }

    /********Update Game State*********/

    gm = update_position_pirate(gm,0);

    for(current_boat = 0; current_boat<gm.merchants.size(); current_boat++) {
        if(gm.merchants[current_boat].AI == true){

            gm = update_position(gm, current_boat);

            if(gm.mapTiles[gm.merchants[current_boat].position.x][gm.merchants[current_boat].position.y].start_finish == 2){
                gm.merchants[current_boat].goldAmount += (int)(gold_payout / 2);
                gold_payout *= .5;
                gm.merchants[current_boat].AI = false;
            }

            //Run this to see if the pirate is within however many units to lose gold
            if(is_colliding_pirate_boat(gm, current_boat, 1) && (tickCount > 50) && gm.merchants[current_boat].AI == true){
                gm.merchants[current_boat].goldAmount -= 1;
                gm.pirates[0].goldAmount += 1;
                tickCount = 0;
                //printf("lost gold!  Merchant has: %d and pirate has: %d, tickcount is %d\n", gm.merchants[current_boat].goldAmount, gm.pirates[0].goldAmount, tickCount);
            }
            tickCount++;

            //Collision Detection.  This is barebones but I built it from the ground up! Blood and sweat!
            //First with the pirate
            if(is_colliding_pirate_boat(gm, current_boat, 0) || is_colliding_boat_pirate(gm, current_boat, 0)){
                //if(1)
                    //printf("**************************************Collided with pirate\n");
                vec2 combined_dir = vec2(gm.merchants[current_boat].velocity.x + gm.pirates[0].velocity.x, 
                                        gm.merchants[current_boat].velocity.y + gm.pirates[0].velocity.y);
                float combined_magnitude = gm.merchants[current_boat].velocity.Length() + gm.pirates[0].velocity.Length();
                vec2 combined_dir_ortho = vec2(-combined_dir.y, combined_dir.x);

                //Compute resulting direction for current_boat
                float current_boat_resulting_directionX = gm.merchants[current_boat].velocity.x - 2 * 
                                                        dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                                                        combined_dir_ortho.Normalize().x;
                float current_boat_resulting_directionY = gm.merchants[current_boat].velocity.y - 2 * 
                                                        dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                                                        combined_dir_ortho.Normalize().y;
                vec2 resulting_current_boat_vel_dir = vec2(current_boat_resulting_directionX, current_boat_resulting_directionY);
                //Same for other boat
                float boat_to_compare_resulting_directionX = gm.pirates[0].velocity.x - 2 * 
                                                        dot_product(gm.pirates[0].velocity, combined_dir_ortho.Normalize()) * 
                                                        combined_dir_ortho.Normalize().x;

                // printf("new thing: %f\n", gm.pirates[0].velocity.x);
                // printf("subtract: %f\n", dot_product(gm.pirates[0].velocity, combined_dir_ortho.Normalize()) * combined_dir_ortho.Normalize().x);

                float boat_to_compare_resulting_directionY = gm.pirates[0].velocity.y - 2 * 
                                                        dot_product(gm.pirates[0].velocity, combined_dir_ortho.Normalize()) * 
                                                        combined_dir_ortho.Normalize().y;
                vec2 resulting_boat_to_compare_vel_dir = vec2(boat_to_compare_resulting_directionX, boat_to_compare_resulting_directionY);

                // printf("collided instance ship Velocity x: %f, y: %f\n", resulting_current_boat_vel_dir.x, resulting_current_boat_vel_dir.y);
                // printf("collided instance Pirate Velocity x: %f, y: %f\n", resulting_boat_to_compare_vel_dir.x, resulting_boat_to_compare_vel_dir.y);

                //Normalizing both and then giving them each half the velocity of the total
                resulting_boat_to_compare_vel_dir = resulting_boat_to_compare_vel_dir.Normalize();
                resulting_boat_to_compare_vel_dir.x *= combined_magnitude / 2;
                resulting_boat_to_compare_vel_dir.y *= combined_magnitude / 2;
                resulting_current_boat_vel_dir = resulting_current_boat_vel_dir.Normalize();
                resulting_current_boat_vel_dir.x *= combined_magnitude / 2;
                resulting_current_boat_vel_dir.y *= combined_magnitude / 2;

                gm.merchants[current_boat].velocity = resulting_current_boat_vel_dir;

                if(resulting_boat_to_compare_vel_dir.Length()<.00001){
                    // printf("This is a thing \n");
                    resulting_boat_to_compare_vel_dir = vec2(resulting_current_boat_vel_dir.x * 1.1, resulting_current_boat_vel_dir.y * 1.1);
                }

                gm.pirates[0].velocity = resulting_boat_to_compare_vel_dir;

                gm.merchants[current_boat].rudderRot = atan2(gm.merchants[current_boat].velocity.y, gm.merchants[current_boat].velocity.x) * 1/pioveroneeighty;
                gm.pirates[0].rudderRot = atan2(gm.pirates[0].velocity.y, gm.pirates[0].velocity.x) * 1/pioveroneeighty;

                //Keep adding the velocity to the position until they don't collide
                while(is_colliding_pirate_boat(gm, current_boat, 0) || is_colliding_boat_pirate(gm, current_boat, 0)){
                    //Manually setting new positions

                    vec2 vector_for_curr_away_from_comp = vec2(gm.merchants[current_boat].coord_pos.x - gm.pirates[0].coord_pos.x,
                                                                gm.merchants[current_boat].coord_pos.y - gm.pirates[0].coord_pos.y);
                    vec2 vector_for_comp_away_from_curr = vec2(gm.pirates[0].coord_pos.x - gm.merchants[current_boat].coord_pos.x,
                                                                gm.pirates[0].coord_pos.y - gm.merchants[current_boat].coord_pos.y);

                    gm.merchants[current_boat].coord_pos.x += vector_for_curr_away_from_comp.Normalize().x * engineMultiplier;
                    gm.merchants[current_boat].coord_pos.y += vector_for_curr_away_from_comp.Normalize().y * engineMultiplier;

                    gm.pirates[0].coord_pos.x += vector_for_comp_away_from_curr.Normalize().x * engineMultiplier;
                    gm.pirates[0].coord_pos.y += vector_for_comp_away_from_curr.Normalize().y * engineMultiplier;

                    //Moving the merchant to the appropriate tile
                    vec2 newLoc_curr = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
                    gm.merchants[current_boat].position = convert_coord_tile(gm, newLoc_curr);

                    vec2 newLoc_comp = vec2(gm.pirates[0].coord_pos.x,gm.pirates[0].coord_pos.y);
                    gm.pirates[0].position = convert_coord_tile(gm, newLoc_comp);
                }
            }

            //Now with all the other boats
            int boat_to_compare;
            for(boat_to_compare = 0; boat_to_compare<gm.merchants.size(); boat_to_compare++) {
                //printf("************************************Check for boat %d against boat %d\n", current_boat, boat_to_compare);
                if(current_boat==boat_to_compare){ // If we are checking collision with ourself
                    //printf("This is a check with myself\n");
                    continue;
                }
                else{ //Checking collision with another ship
                    if(is_colliding(gm, current_boat, boat_to_compare, 0)){
                        // If this passes, do more granular collision.  This is barebones so I'll just treat this as a collision.
                        //I am just going to reverse the accelerations for both ships to see if that fixes it.  I can do something more special later.
                        if(verbose)
                            printf("Collided\n");
                        vec2 combined_dir = vec2(gm.merchants[current_boat].velocity.x + gm.merchants[boat_to_compare].velocity.x, 
                                                gm.merchants[current_boat].velocity.y + gm.merchants[boat_to_compare].velocity.y);
                        float combined_magnitude = gm.merchants[current_boat].velocity.Length() + gm.merchants[boat_to_compare].velocity.Length();
                        vec2 combined_dir_ortho = vec2(-combined_dir.y, combined_dir.x);
     
                        //Compute resulting direction for current_boat
                        float current_boat_resulting_directionX = gm.merchants[current_boat].velocity.x - 2 * 
                                                                dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                                                                combined_dir_ortho.Normalize().x;
                        float current_boat_resulting_directionY = gm.merchants[current_boat].velocity.y - 2 * 
                                                                dot_product(gm.merchants[current_boat].velocity, combined_dir_ortho.Normalize()) * 
                                                                combined_dir_ortho.Normalize().y;
                        vec2 resulting_current_boat_vel_dir = vec2(current_boat_resulting_directionX, current_boat_resulting_directionY);
                        //Same for other boat
                        float boat_to_compare_resulting_directionX = gm.merchants[boat_to_compare].velocity.x - 2 * 
                                                                dot_product(gm.merchants[boat_to_compare].velocity, combined_dir_ortho.Normalize()) * 
                                                                combined_dir_ortho.Normalize().x;
                        float boat_to_compare_resulting_directionY = gm.merchants[boat_to_compare].velocity.y - 2 * 
                                                                dot_product(gm.merchants[boat_to_compare].velocity, combined_dir_ortho.Normalize()) * 
                                                                combined_dir_ortho.Normalize().y;

                        vec2 resulting_boat_to_compare_vel_dir = vec2(boat_to_compare_resulting_directionX, boat_to_compare_resulting_directionY);

                        //Normalizing both and then giving them each half the velocity of the total
                        resulting_boat_to_compare_vel_dir = resulting_boat_to_compare_vel_dir.Normalize();
                        resulting_boat_to_compare_vel_dir.x *= combined_magnitude / 2;
                        resulting_boat_to_compare_vel_dir.y *= combined_magnitude / 2;
                        resulting_current_boat_vel_dir = resulting_current_boat_vel_dir.Normalize();
                        resulting_current_boat_vel_dir.x *= combined_magnitude / 2;
                        resulting_current_boat_vel_dir.y *= combined_magnitude / 2;

                        gm.merchants[current_boat].velocity = resulting_current_boat_vel_dir;
                        gm.merchants[boat_to_compare].velocity = resulting_boat_to_compare_vel_dir;

                        gm.merchants[current_boat].rudderRot = atan2(gm.merchants[current_boat].velocity.y, gm.merchants[current_boat].velocity.x) * 1/pioveroneeighty;
                        gm.merchants[boat_to_compare].rudderRot = atan2(gm.merchants[boat_to_compare].velocity.y, gm.merchants[boat_to_compare].velocity.x) * 1/pioveroneeighty;

                        //Keep adding the velocity to the position until they don't collide
                        while((is_colliding(gm, current_boat, boat_to_compare,0)) || (is_colliding(gm, boat_to_compare, current_boat,0))){
                            //Manually setting new positions

                            vec2 vector_for_curr_away_from_comp = vec2(gm.merchants[current_boat].coord_pos.x - gm.merchants[boat_to_compare].coord_pos.x,
                                                                        gm.merchants[current_boat].coord_pos.y - gm.merchants[boat_to_compare].coord_pos.y);
                            vec2 vector_for_comp_away_from_curr = vec2(gm.merchants[boat_to_compare].coord_pos.x - gm.merchants[current_boat].coord_pos.x,
                                                                        gm.merchants[boat_to_compare].coord_pos.y - gm.merchants[current_boat].coord_pos.y);

                            gm.merchants[current_boat].coord_pos.x += vector_for_curr_away_from_comp.Normalize().x * engineMultiplier;
                            gm.merchants[current_boat].coord_pos.y += vector_for_curr_away_from_comp.Normalize().y * engineMultiplier;

                            gm.merchants[boat_to_compare].coord_pos.x += vector_for_comp_away_from_curr.Normalize().x * engineMultiplier;
                            gm.merchants[boat_to_compare].coord_pos.y += vector_for_comp_away_from_curr.Normalize().y * engineMultiplier;

                            //Moving the merchant to the appropriate tile
                            vec2 newLoc_curr = vec2(gm.merchants[current_boat].coord_pos.x,gm.merchants[current_boat].coord_pos.y);
                            gm.merchants[current_boat].position = convert_coord_tile(gm, newLoc_curr);

                            vec2 newLoc_comp = vec2(gm.merchants[boat_to_compare].coord_pos.x,gm.merchants[boat_to_compare].coord_pos.y);
                            gm.merchants[boat_to_compare].position = convert_coord_tile(gm, newLoc_comp);
                        }
                    }
                    else{ // Not close to each other so pass.
                        continue;
                    }
                }
            }
        }
    }

    return gm;
}

void print_boat(PiGameMap gmap, int current_boat){
    float posx = gmap.merchants[current_boat].coord_pos.x;
    float posy = gmap.merchants[current_boat].coord_pos.y;
    float velx = gmap.merchants[current_boat].velocity.x;
    float vely = gmap.merchants[current_boat].velocity.y;
    float accx = gmap.merchants[current_boat].acceleration.x;
    float accy = gmap.merchants[current_boat].acceleration.y;
    float orient = gmap.merchants[current_boat].orientation;
    float rot = gmap.merchants[current_boat].rudderRot;

    if(verbose)
        printf("Name: %s\nPosition %f,%f\nVelocity %f,%f\nAcceleration %f,%f\nOrientation: %f\n, rudderRot: %f\n", gmap.merchants[0].merchant_name.c_str(), posx, posy, velx, vely, accx, accy, orient,rot);
}

#ifdef K_TEST_PHYSICS
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
       768 * 1, 768 * 1, SDL_WINDOW_SHOWN);
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

    PiGameMap map = read_png_heightmap("../ai/height.csv", 48, 48, 2048);

    // PiGameMap map(25);
    map.add_merchant(PiMerchant());

    map.add_merchant(PiMerchant());

    // map.add_merchant(PiMerchant());
    map.merchants[0].merchant_name = "our_guy";

    //map.mapTiles[15][2].land_water = 1;
    map.mapTiles[15][2].start_finish = 2;

    // int mapTileIter = 0;
    //     int anotherMapTileIter = 0;
    //     for(mapTileIter = 0; mapTileIter<map.mapTiles.size(); mapTileIter++){
    //         for(anotherMapTileIter = 0; anotherMapTileIter<map.mapTiles[mapTileIter].size(); anotherMapTileIter++){
    //             if(mapTileIter == 0 || anotherMapTileIter == 0){
    //                 map.mapTiles[mapTileIter][anotherMapTileIter].land_water = 0;
    //             }
    //         }
    //     }

    map.merchants[0].coord_pos.x = 150;
    map.merchants[0].coord_pos.y = 150;

    map.merchants[1].coord_pos.x = 500;
    map.merchants[1].coord_pos.y = 500;

    map.pirates[0].coord_pos.x = 200;
    map.pirates[0].coord_pos.y = 200;

    //map.merchants[1].velocity.x = .01;

    // map.merchants[1].goldAmount = 1000;
    map.merchants[0].goldAmount = 1000;

    map.merchants[1].goldAmount = 500;

    map.pirates[0].goldAmount = 0;

        //Moving the merchant to the appropriate tile
    vec2 newLoc = vec2(map.merchants[0].coord_pos.x,map.merchants[0].coord_pos.y);
    map.merchants[0].position = convert_coord_tile(map, newLoc);

    vec2 newLoce = vec2(map.merchants[1].coord_pos.x,map.merchants[1].coord_pos.y);
    map.merchants[1].position = convert_coord_tile(map, newLoce);

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

    int itera = 0;

    while(!quit) {

        itera++;

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
        }
        if (keysDown.count(SDLK_RIGHT)) {
            content.push_back("RIGHT");
        }

        if (keysDown.count(SDLK_UP)) {
            content.push_back("UP");
        }

        if (keysDown.count(SDLK_DOWN)) {
            content.push_back("DOWN");
        }

        //I want it to run regardless
        input_object["keystrokes"] = content;
        map = compute_gamestate(input_object, map);
        if(verbose)
            map.print_game_map();
        print_boat(map, 0);
        if(verbose)
            printf("\n\n\n");

        if(itera % 99 == 0){
            run_astar(map);
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

        SDL_Rect wholeRect = {25 * 1, 25 * 1, (768 - 50) * 1, (768 - 50) * 1};
        SDL_RenderDrawRect(renderer, &wholeRect);

        draw_boat(renderer,map, 0);

        int mapTileIter = 0;
        int anotherMapTileIter = 0;
        for(mapTileIter = 0; mapTileIter<map.mapTiles.size(); mapTileIter++){
            for(anotherMapTileIter = 0; anotherMapTileIter<map.mapTiles[mapTileIter].size(); anotherMapTileIter++){
                if(map.mapTiles[mapTileIter][anotherMapTileIter].land_water == 1 || map.mapTiles[mapTileIter][anotherMapTileIter].start_finish == 2){
                    SDL_Rect newRect = {(int)ceil(convert_tile_coord(map, vec2(mapTileIter,anotherMapTileIter)).x), 
                                        (int)ceil(convert_tile_coord(map, vec2(mapTileIter,anotherMapTileIter)).y), 2048/48, 2048/48};
                    SDL_RenderDrawRect(renderer, &newRect);
                }
            }
        }

        draw_boat(renderer,map, 1);

        draw_pirate(renderer,map, 0);

        // Update screen
        SDL_RenderPresent(renderer);

        // Update the surface
        SDL_UpdateWindowSurface(window);

            // usleep(3000);
    }

    return 0;
}
#endif