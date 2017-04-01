/**
 * sample.h
 *
 * Victor Jiao & Whomever Else Worked A Lot on This File
 *
 * 
 * File description goes here. This file is to give a generic sample to the style used in our 
 * project. File lines should be capped at 100 characters long (80 feels too short to me tbh).
 *
 * NOTE: Remember to use capital letters for actual file names
 * 
 * Please feel free to make changes as needed, although it'd be nice to talk about them face to 
 * face, over instant messenger, or even through a github issues thing (if it's a bigger, more 
 * formal issue). 
 *
 * We will also be working on branches, so I guess try not to push to the master branch. This will
 * allow us to keep a clear working branch, and as we merge project branches into master 
 * intermittently, this will make sure everyone's still working on their own version of the 
 * project, but that every version still includes as much up-to-date working functionality as
 * possible, and that no one is going to push into the production branch and break things.
 *
 * Optimally, there should also be code review, but I guess whatever. Just make sure you don't do
 * anything really bad. I'm pretty bad myself at monitoring my code style and working with git
 * branches, but I'm sure that the importance of keeping everything relatively clean is pretty
 * obvious, so I'm making an effort to try to set good standards ahead of time. Please disagree 
 * with me through messenger if you feel differently!
 */

#include <iostream>
#include <stdlib.h>

/* use std cuz we won't implement our own */
using namespace std;

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

class TdObject {
private:
	TdRectangle fRect; // Short comments go here

	/** Longer comments go here cuz that makes sense */
	int	fAge;
	string fBlah;

public:
	TdObject() {
		fRect = TdRectangle(3, 4, 0, 0);
		fAge = 0;
		fBlah = ""; // TODO: is this the right syntax? (real TODOs should be a bit more... legit)
	}

	/** We only need one line for this, so we define it in the class declaration. */
	void setAge(int age) { fAge = age; }

	/**
	 * doSomethingComplicated (install SublimeLinter plugin to get this style automatically)
	 * takes in an int and double and does some math with them, but returns nothing
	 * @param wowInput     any int
	 * @param wowMoreInput any double
	 * @param return 	   void
	 */
	void doSomethingComplicated(int wowInput, double wowMoreInput);
};

#endif
