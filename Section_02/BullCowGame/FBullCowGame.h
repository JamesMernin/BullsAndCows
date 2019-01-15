/*
The game logic (no view code or direct user interaction)
The game is a simple guess the word game based on Mastermind
*/

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <map>
#include <algorithm>
#include <vector>

#define TMap std::map

// to make syntax Unreal friendly
using FString = std::string;
using FText = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

class FBullCowGame
{
public:
	FBullCowGame(); // constructor

	void Reset();

	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	int32 GetMinLetters() const;
	int32 GetMaxLetters() const;
	bool IsGameWon() const;
	FString GetHiddenWord(int32) const;
	EGuessStatus CheckGuessValidity(FString) const;

	FBullCowCount SubmitValidGuess(FString);
	void LoadDictionary();
	void PlayGame();
	void PrintIntro();
	int32 SetWordLength();
	void SetMaxTries(int32);
	void GuessLoop();
	FText GetValidGuess();
	bool WantsToPlayAgain();
	void PrintGameSummary();

// ^^ Please try to ignore this and focus on the interface above ^^
private:
	// see constructor for initialization
	int32 MyCurrentTry;
	int32 MinLetters;
	int32 MaxLetters;
	int32 MaxTries;
	FString MyHiddenWord;
	TMap<int32, std::vector<FString>> Dictionary;
	bool bGameIsWon;

	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
};
