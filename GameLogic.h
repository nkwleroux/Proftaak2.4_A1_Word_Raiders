#pragma once
#include <vector>
#include "WordLoader.h"
#include "Timer.h"
#include "Word.h"
#include "GameObject.h"


class GameLogic
{
private:
	WordLoader* wordLoader;

	int currentWordLength;
	int currentWordAmount;

	int currentWordIndex = -1;
	int chosenWordsAmount = 0;
	
	// List containing all the words that have to be guessed
	std::vector<Word*> wordsToGuess;
	// List of all the letters that are already guessed correctly
	std::vector<char> correctLetters;
	// List of all the characters shot
	std::vector<char> shotLetters;

	// The current word that needs to be guessed
	Word* currentWord;

	// String to display all the shot letters
	std::string shotWord = "";
	// String to display all the correct letters on their spot
	std::string correctWord = "";

	bool gameStarted;

	Timer* gameTimer;
	Timer* oneSecondTimer;

	void checkForStartingConditions();
	void clearVector(std::vector<char>* vector);
	void fillVector();
	bool checkWord();
	void shootLetter(char shotLetter);

public:
	GameLogic();
	~GameLogic();

	GameObject* selectedObject;

	bool update(bool* redDetected);
	std::string getShotWord();
	std::string getCorrectWord();
	Word* getCurrentWord();
	Timer* getGameTimer();

	bool reset;
};

