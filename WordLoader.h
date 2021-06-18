#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Vector>
#include "json.hpp"
#include "Word.h"

class WordLoader
{

public:
	/*
	* This is the constructor of the wordloader class.
	* The constructor doesn't have parameters
	*/
	WordLoader();

	/*
	* This is the deconstructor of the wordloader class.
	*/
	~WordLoader() = default;

	/*
	* This function loads the words from the json list.
	* int wordType = length of the word: 5, 6 or 7 letters.
	* int wordAmount = the amount of words to load.
	*/
	std::vector<Word*> loadWords(int wordType, int wordAmount);

	/*
	* This function prints all words of a given vector.
	* This function is used during testing. Is not necessary after launching the game
	*/
	void printWords(std::vector<Word*> words);

private:
	/*
	* This function checks if a random number is already used, to avoid duplicate words
	*/
	bool checkIfNumberExists(int arr[], int number, int size);
};


