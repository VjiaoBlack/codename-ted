/**
 * game.cpp
 *
 * Victor Jiao
 *
 * A basic skeleton for client-side code. 
 */

#include <iostream>

#include "game.h"

int main(int argc, char* argv[]) {
    TdGame *game = new TdGame();

    // acquire resources from server here
    // game->acquireResources();
    
    while (game->fRunState != kRunState_Stopped) {
        // get new data from server, if any
        // TODO: server team figure this out w/ graphics team
        // game->getNewData();
        
        // draw stuff
        // game->drawStuff();
        cout << "Game is running" << endl;
    }

    delete game;
}