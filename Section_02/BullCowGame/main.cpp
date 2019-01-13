/*
This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PlayGame();
void PrintIntro();
void GuessLoop();
FText GetValidGuess();
bool WantsToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which we reuse across plays

// the entry point for our application
int main()
{
	PlayGame();
	return 0; // exit the application
}

// runs a loop of the game for as many times as the player wants to play
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	bool bWantsToPlayAgain = false;
	do {
		BCGame.Reset();
		PrintIntro();
		GuessLoop();
		PrintGameSummary();
	} while (WantsToPlayAgain());
	return;
}

void PrintIntro()
{
	const int32 WordLength = BCGame.GetHiddenWordLength();
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n\n";
	std::cout << "          }   {      ___\n";
	std::cout << "          (o o)     (o o)\n";
	std::cout << "   /-------\\ /       \\ /-------\\\n";
	std::cout << "  / | BULL |O         O| COW  | \\\n";
	std::cout << " *  |-,----|           |------|  *\n";
	std::cout << "    ^      ^           ^      ^\n";
	std::cout << "Can you guess the " << WordLength << " letter isogram I'm thinking of?\n\n";
	return;
}

// Loops until it gets a valid guess, then prints the number of bulls and cows in the guess
// Continues until the game is won or the number of tries is met
void GuessLoop()
{
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries())
	{
		FText Guess = GetValidGuess();
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

// Win/loss messages
void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}
}

// Asks the user if they want to play the game again
bool WantsToPlayAgain()
{
	std::cout << "Do you want to play again with the same hidden word (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}
