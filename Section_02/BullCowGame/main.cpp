/*
This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/

#pragma once

#include "FBullCowGame.h"

// to make syntax Unreal friendly
using int32 = int;

// function prototypes as outside a class
// instantiate a new game, which we reuse across plays

// the entry point for our application
int main()
{
	FBullCowGame BCGame;
	BCGame.PlayGame();
	return 0; // exit the application
}
