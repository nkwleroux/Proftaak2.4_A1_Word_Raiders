#include "GameLogic.h"
#include "WordLoader.h"
#include "Scene.h"
#include "LetterModelComponent.h"
#include "Timer.h"


float timeSpent;
int achievedScore;
bool wonGame;
int currentLives;

int currentWordLength;
int currentWordAmount;


GameLogic::GameLogic() {
	// Initiate variables to standard values
	reset = false;
	wonGame = false;
	timeSpent = 0;
	currentWordIndex = -1;
	currentWordAmount = 1;
	currentWordLength = 5;
	achievedScore = 0;
	currentLives = 3;

	// Initiate timers
	gameTimer = new Timer(10);
	antiSpamTimer = new Timer(1);

	// Load words from json file
	wordLoader = new WordLoader();
}

GameLogic::~GameLogic()
{}

void GameLogic::checkForStartingConditions() {
	//check if it is the start of the game
	if (!gameStarted) {
		// If the game has not been started! :
		// set boolean gamestarted to true
		gameStarted = true;
		reset = true;
		// we start with 3 lives
		currentLives = 3;
		achievedScore = 0;
		// load in a word of a selected length
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		// draw the words
		wordLoader->printWords(wordsToGuess);
		currentWord = wordsToGuess[0];
		// reset and start game timer
		gameTimer->reset();
		gameTimer->start();
		antiSpamTimer->start();

		// Size of correct and shot letters
		correctLetters = std::vector<char>(currentWordLength);
		shotLetters = std::vector<char>(currentWordLength);
	}
}

// Set game to the end screen
void GameLogic::setEndScreen() {
	gameTimer->stop();
	timeSpent = gameTimer->timeRemaining();
}

// Update function for the game logic
bool GameLogic::update(bool* redDetected) {
	checkForStartingConditions();

	// If the currentWordIndex == -1 we know a new word is the current word
	if (currentWordIndex == -1) {
		fillVector();
		currentWordIndex = 0;
		// Print current word
		std::cout << currentWord->getWord() << std::endl;
		return false;
	}

	// If the timer has been finished, remove a life and reset game timmer
	if (gameTimer->hasFinished())
	{
		if (currentLives>1)
		{
			currentLives--;
			gameTimer->reset();
		}
		// If there are no more lifes left
		else {
			setEndScreen();
			wonGame = false;
			return true;
		}
	}

	// Check if the player want to fire
	if (*redDetected) {
		*redDetected = false;
		// Check if the player can fire
		if (antiSpamTimer->hasFinished()) {
			antiSpamTimer->start();

			// Check if an object is selected
			if (selectedObject != nullptr)
			{
				// If the object is a letter model
				if (selectedObject->getComponent<LetterModelComponent>())
				{
					// Get the letter of that lettermodel and shoot it
					char shotLetter = selectedObject->getComponent<LetterModelComponent>()->getLetter();
					shootLetter(shotLetter);			
				}
			}

			// If we have shot as many letters as the wordLength
			if (currentWordIndex == currentWordLength)
			{
				// Check the word if it is correct
				if (checkWord()) {
					// 100 point becuase the word is correct
					achievedScore += 100;

					// If it is correct we delete the word and set the new current word
					wordsToGuess.erase(wordsToGuess.begin());
					if (wordsToGuess.size() > 0) {
						currentWord = wordsToGuess[0];
						std::cout << currentWord << std::endl;
						wordLoader->printWords(wordsToGuess);
						reset = true;
					}
					// If there are no words left we are done and return true
					else {
						wonGame = true;
						// Time left added to score
						achievedScore += gameTimer->timeRemaining()*5;
						// Current lives is a multiplier for the score
						achievedScore *= currentLives;
						setEndScreen();
						return true;
					}
				}
				// We remove all the shot characters
				else {
					currentWordIndex = 0;
					shotWord = "";
					clearVector(&shotLetters);
				}

				wordCorrect = true;
			}

		}
	}

	shotWord = ""; //clear the shotWord string
	for (int i = 0; i < shotLetters.size(); i++) {
		shotWord += shotLetters[i]; //fill the string with the letters of the vector
	}

	correctWord = ""; //clear the correctWord string
	for (int i = 0; i < correctLetters.size(); i++) {
		correctWord += correctLetters[i]; //fill the string with the letters of the vector
	}

	return false;
}



//// Getter for which word has been hit
std::string GameLogic::getShotWord()
{
	return shotWord;
}

// Getter for the correct/right word
std::string GameLogic::getCorrectWord()
{
	return correctWord;
}

// Getter for the current assigned word
Word* GameLogic::getCurrentWord()
{
	return currentWord;
}

// Getter for retreiving the timer
Timer* GameLogic::getGameTimer()
{
	return gameTimer;
}

// Getter for retreiving the current lifes and score
std::string GameLogic::getCurrentLifes()
{
	std::string stringLevens;
	stringLevens.append(currentLives,'*');
	return stringLevens;
}

// Retreive the current score
std::string GameLogic::getCurrentScore()
{
	return std::to_string(achievedScore);
}

void GameLogic::clearVector(std::vector<char>* vector) {
	for (int i = 0; i < vector->size(); i++) {
		vector->at(i) = '_';
	}
}

/*
* This function fills 2 vectors with letters or an _
*/
void GameLogic::fillVector() {
	for (int i = 0; i < correctLetters.size(); i++) {
		shotLetters.at(i) = '_'; //fill the hasBeenShot vector with _
		if (i == 0) {
			correctLetters.at(i) = currentWord->getFirstLetter(); //fill the vector with the first letter of the current word
		}
		else {
			correctLetters.at(i) = '_'; //fill the other indexes with an _
		}
	}
}

// Function for checking the word, and if the guessed letters make up the correct word
bool GameLogic::checkWord() {
	int correctLettersAmount = 0;
	for (int i = 0; i < currentWordLength; i++) {
		if (currentWord->getWord()[i] == shotLetters.at(i)) {
			correctLetters.at(i) = currentWord->getWord()[i];
			correctLettersAmount++;
		}
	}

	if (correctLettersAmount == currentWordLength) {
		clearVector(&correctLetters);
		currentWordIndex = -1;
		return true;
	}
	else {
		return false;
	}
}

// With this function we can hit a letter
void GameLogic::shootLetter(char shotLetter) {
	std::cout << "shotletter" << std::endl;
	shotLetters.at(currentWordIndex) = shotLetter;
	currentWordIndex++;
	achievedScore += 5;
	selectedObject->getComponent<LetterModelComponent>()->hasBeenShot = true;
}