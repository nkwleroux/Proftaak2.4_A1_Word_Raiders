#include "GameLogic.h"
#include "WordLoader.h"
#include "Scene.h"
#include "LetterModelComponent.h"

GameLogic::GameLogic() {
	// Initiate variables to standard values
	gameStarted = false;
	reset = false;
	currentWordLength = 5;
	currentWordAmount = 3;
	currentWordIndex = -1;

	// Initiate timers
	gameTimer = new Timer(90);
	oneSecondTimer = new Timer(1);

	// Load words from json file
	wordLoader = new WordLoader();

	checkForStartingConditions();
}

GameLogic::~GameLogic()
{}

void GameLogic::checkForStartingConditions() {
	//check if it is the start of the game
	if (!gameStarted) {
		gameStarted = true;
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		currentWord = wordsToGuess[chosenWordsAmount];
		gameTimer->start();
		oneSecondTimer->start();

		correctLetters = std::vector<char>(currentWordLength);
		shotLetters = std::vector<char>(currentWordLength);
	}
}

bool GameLogic::update(bool* redDetected) {
	checkForStartingConditions();

	// If the currentWordIndex == -1 we know a new word is the current word
	if (currentWordIndex == -1) {
		fillVector();
		currentWordIndex = 0;
		// todo remove for debugging
		std::cout << currentWord->getWord() << std::endl;
		return false;
	}

	//TODO --> check for lives
	//TODO --> check for timer


	// Check if the player want to fire
	if (*redDetected) {
		*redDetected = false;
		// Check if the player can fire
		if (oneSecondTimer->hasFinished()) {
			oneSecondTimer->start();

			// Check if an objcet is selected
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
					// If it is correct we delete the word and set the new current word
					wordsToGuess.pop_back();
					if (wordsToGuess.size() > 0) {
						currentWord = wordsToGuess[wordsToGuess.size() - 1];
						reset = true;
					}
					// If there are no words left we are done and return true
					else {
						return true;
					}
				}
				// We remove all the shot characters 
				else {
					currentWordIndex = 0;
					shotWord = "";
					clearVector(&shotLetters);
				}
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

std::string GameLogic::getShotWord()
{
	return shotWord;
}

std::string GameLogic::getCorrectWord()
{
	return correctWord;
}

Word* GameLogic::getCurrentWord()
{
	return currentWord;
}

Timer* GameLogic::getGameTimer()
{
	return gameTimer;
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
		shotLetters.at(i) = '_'; //fill the shotLetter vector with _
		if (i == 0) {
			correctLetters.at(i) = currentWord->getFirstLetter(); //fill the vector with the first letter of the current word
		}
		else {
			correctLetters.at(i) = '_'; //fill the other indexes with an _
		}
	}
}

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

void GameLogic::shootLetter(char shotLetter) {
	shotLetters.at(currentWordIndex) = shotLetter;
	currentWordIndex++;
}