#include "Word.h"
#include <cstdlib>
#include <ctime>

/*
* Constructor of the word class.
* std::string str = The string value of the word
*/
Word::Word(std::string str) : word(str) {
	//create a vector of letters the word consists of.
	createLettersArray();
}

/*
* This function returns the length of the word.
*/
int Word::getWordLength() {
	return word.size();
}

/*
* This function returns the word object.
*/
std::string Word::getWord() {
	return word;
}

/*
* This function returns the array of letters the word consists of
*/
std::vector<char> Word::getLetters() {
	return letters;
}

/*
* This function prints the letters of the word in the console.
*/
void Word::writeLetters() {
	for (int i = 0; i < letters.size(); i++) {
		std::cout << letters.at(i);
	}
	std::cout << std::endl;
}

/*
* This function creates a vector of letters.
*/
void Word::createLettersArray() {
	for (const char& c : word) { //for every letter of the word
		letters.push_back(c); //add letter to the vector
	}
	//Add random letters to the vector
	createRandomLetters();
}

/*
* This function adds random letters to the vector.
* The function adds as many letters as the original length of the word.
* So when wordlength = 5, also 5 extra letters are added. In total there are 10 letters.
*/
void Word::createRandomLetters() {
	for (int i = 0; i < getWordLength(); i++) {
		char letter = 'A' + rand() % 26; //generate random letter between A and Z.
		letters.push_back(letter); //add letter to the vector
	}
}

/*
* This function returns the first letter of the word
*/
char Word::getFirstLetter() {
	return word[0];
}

