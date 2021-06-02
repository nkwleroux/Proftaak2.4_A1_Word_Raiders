#include "Word.h"
#include <cstdlib>
#include <ctime>

Word::Word(std::string str) : word(str){
	createLettersArray();
}

int Word::getWordLength() {
	return word.size();
}

std::string Word::getWord() {
	return word;
}

std::vector<char> Word::getLetters() {
	return letters;
}

void Word::writeLetters() {
	for (int i = 0; i < letters.size(); i++) {
		std::cout << letters.at(i);
	}
	std::cout << std::endl;
}

void Word::createLettersArray() {
	for (const char& c : word) {
		letters.push_back(c);
	}
	createRandomLetters();
}

void Word::createRandomLetters() {
	for (int i = 0; i < getWordLength(); i++) {
		char letter = 'A' + rand() % 26;
		letters.push_back(letter);
	}
}

char Word::getFirstLetter() {
	return word[0];
}

