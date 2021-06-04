#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Vector>
#include "json.hpp"
#include "Word.h"

typedef const enum DIFFICULTY { easy = 3, medium = 5, hard = 7 };

class WordLoader
{

public:
	WordLoader();
	~WordLoader() = default;

	std::vector<Word*> loadWords(int wordType, DIFFICULTY diff);
	void printWords(std::vector<Word*> words);

private:
	bool checkIfNumberExists(int arr[], int number, int size);
};


