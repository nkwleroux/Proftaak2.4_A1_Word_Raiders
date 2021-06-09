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
	WordLoader();
	~WordLoader() = default;

	std::vector<Word*> loadWords(int wordType, int wordAmount);
	void printWords(std::vector<Word*> words);

private:
	bool checkIfNumberExists(int arr[], int number, int size);
};


