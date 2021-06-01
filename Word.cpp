#include "Word.h"

Word::Word(std::string str) : word(str){
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
	for (const char& c : word) {
		std::cout << c << std::endl;
	}
}

void Word::createLettersArray() {
	for (const char& c : word) {
		letters.push_back(c);
	}
}

