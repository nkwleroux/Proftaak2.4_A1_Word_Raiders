#pragma once
#include <vector>
#include "WordLoader.h"
#include "Timer.h"
#include "Word.h"
#include "GameObject.h"


class GameLogic
{
private:
	// Wordloader, which can generate words for us
	WordLoader* wordLoader;

	// Variable to store the index and current lives
	int currentWordIndex;
	int currentLives;

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

	// Gametimer to keep track of the time passed
	Timer* gameTimer;
	Timer* antiSpamTimer;

	// Method to check if starting conditions are met
	void checkForStartingConditions();
	// Change the game to the end screen
	void setEndScreen();
	// Method for clearing vector
	void clearVector(std::vector<char>* vector);
	// Method for filling vector
	void fillVector();
	// Method to check if the word is correct
	bool checkWord();
	// Method to "shoot/hit" a letter
	void shootLetter(char shotLetter);

	int wordIndex = 0;

public:
	// Constructor
	GameLogic();
	// Destructor
	~GameLogic();

	// Variable to store the current selected object
	GameObject* selectedObject;

	// Update function
	bool update(bool* redDetected);

	// Getter for which word has been hit
	std::string getShotWord();
	// Getter for the correct/right word
	std::string getCorrectWord();
	// Getter for the current assigned word
	Word* getCurrentWord();
	// Getter for retreiving the timer
	Timer* getGameTimer();
	// Getter for retreiving the current lifes and score
	std::string getCurrentLifes();
	std::string getCurrentScore();

	// Booleans for reset, if we have the correct word, and if the game has been started
	bool reset;
	bool wordCorrect;
	bool gameStarted = false;
};