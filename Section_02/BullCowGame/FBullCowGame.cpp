#pragma once

#include "FBullCowGame.h"

FBullCowGame::FBullCowGame() { Reset(); } // default constructor
FBullCowGame BCGame; // instantiate a new game, which we reuse across plays

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
int32 FBullCowGame::GetMinLetters() const { return MinLetters; }
int32 FBullCowGame::GetMaxLetters() const { return MaxLetters; }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

void FBullCowGame::Reset()
{
	LoadDictionary();
	MyHiddenWord = "";
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

// runs a loop of the game for as many times as the player wants to play
void FBullCowGame::PlayGame()
{
	do {
		BCGame.Reset();
		BCGame.PrintIntro();
		int32 WordLength = BCGame.SetWordLength();
		BCGame.MyHiddenWord = BCGame.GetHiddenWord(WordLength);
		BCGame.SetMaxTries(WordLength);
		BCGame.GuessLoop();
		BCGame.PrintGameSummary();
	} while (BCGame.WantsToPlayAgain());
	return;
}

void FBullCowGame::LoadDictionary()
{
	FString Word = "";
	std::ifstream File;
	const FString& DictionaryName = "C:\\Users\\jmern\\Documents\\repos\\UnrealCourse\\Section_02\\isograms.txt";
	File.open(DictionaryName);

	MinLetters = 27;
	MaxLetters = 0;

	if (!File.is_open())
	{
		std::cout << "File cannot be opened.\n";
		exit(1);
	}

	while (File >> Word)
	{
		MaxLetters = std::max(MaxLetters, (int32)Word.length());
		MinLetters = std::min(MinLetters, (int32)Word.length());

		Dictionary[(int32)Word.length()].push_back(Word);
	}

	return;
}

void FBullCowGame::PrintIntro()
{
	const int32 WordLength = BCGame.GetHiddenWordLength();
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n\n";
	std::cout << "          }   {      ___\n";
	std::cout << "          (o o)     (o o)\n";
	std::cout << "   /-------\\ /       \\ /-------\\\n";
	std::cout << "  / | BULL |O         O| COW  | \\\n";
	std::cout << " *  |-,----|           |------|  *\n";
	std::cout << "    ^      ^           ^      ^\n\n";
	std::cout << "Rules: Within the given number of turns, the goal of the game is to correctly guess an isogram chosen at random.\n";
	std::cout << "An isogram is a word that contains no repeating letters.\n";
	std::cout << "Each time you guess, you will get back the number of letters in their correct position in the word (Bulls) \n";
	std::cout << "and the number of letters in the wrong position (Cows).\n\n";
	return;
}

int32 FBullCowGame::SetWordLength() // TODO Fix the user input of this function
{
	std::cout << "With all that said and done, how long do you want the word to be?\n";
	std::cout << "Pick a number between " << BCGame.GetMinLetters() << " and " << BCGame.GetMaxLetters() << ".\n";
	std::string WordLengthInput = "";
	int WordLength = 0;
	do {
		std::getline(std::cin, WordLengthInput);
		std::stringstream convert(WordLengthInput);
		if (!(convert >> WordLength))
		{
			WordLength = 0;
		}
		if (WordLength < BCGame.GetMinLetters() || WordLength > BCGame.GetMaxLetters())
		{
			std::cout << "That's not a valid length. Try again.\n";
			std::cout << "Pick a number between " << BCGame.GetMinLetters() << " and " << BCGame.GetMaxLetters() << ".\n";
			std::cin >> WordLength;
		}
	} while (WordLength < BCGame.GetMinLetters() || WordLength > BCGame.GetMaxLetters());
	return WordLength;
}

FString FBullCowGame::GetHiddenWord(int32 WordLength) const
{
	std::random_device RD;

	std::uniform_int_distribution<unsigned long> UniformDistr(0, BCGame.Dictionary[WordLength].size() - 1);
	return BCGame.Dictionary[WordLength][UniformDistr(RD)];
}

void FBullCowGame::SetMaxTries(int32 WordLength)
{
	TMap<int32, int32>WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,20}, {8,25}, {9,30}, {10,35} };
	BCGame.MaxTries = WordLengthToMaxTries[WordLength];
	std::cout << "With that word length, the maximum number of tries is " << BCGame.MaxTries << "\n\n";
	return;
}

// Loops until it gets a valid guess, then prints the number of bulls and cows in the guess
// Continues until the game is won or the number of tries is met
void FBullCowGame::GuessLoop()
{
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.MaxTries)
	{
		FText Guess = GetValidGuess();
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}
	return;
}

// loop continually until the user gives a valid guess
FText FBullCowGame::GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.MaxTries;
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
void FBullCowGame::PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
		if (BCGame.GetCurrentTry() - 1 == 1)
		{
			std::cout << "You managed to guess the word on the first try. Lucky you!\n";
		}
		std::cout << "\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
		std::cout << "The hidden word was: " << BCGame.MyHiddenWord << "\n\n";
	}
}

// Asks the user if they want to play the game again
bool FBullCowGame::WantsToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))// if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length if wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}


bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	//setup our map
	TMap<char, bool> LetterSeen;

	// loop through the word and set is seen to true
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // handle mixed case

		if (LetterSeen[Letter])
		{
			return false; // we do NOT have an isogram
		}
		else
		{
			LetterSeen[Letter] = true;
		}
	}
	return true; // no letters repeat
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	return true;
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	// loop through all letters in the hidden word
	int32 WordLength = BCGame.MyHiddenWord.length(); // assuming same length as guess
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match then
			if (Guess[GChar] == BCGame.MyHiddenWord[MHWChar])
			{
				if (GChar == MHWChar) // if they're in the same place
				{
					BullCowCount.Bulls++; // increment Bulls
				}
				else
				{
					BullCowCount.Cows++; // must be a Cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}