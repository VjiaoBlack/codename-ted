/** 
 * game.h
 *
 * Victor Jiao
 *  
 * A basic skeleton for client-side code. 
 */

#ifndef _GAME_H_
#define _GAME_H_

using namespace std;

enum kRunState {
	kRunState_Stopped,
	kRunState_Running,
	kRunState_NumTypes
};

class TdGame {
public:
    // TdMap map; // transfer this at beginning of game start / map select
	
	kRunState fRunState;    

    TdGame() {
        fRunState = kRunState_Running;
    }
};

#endif