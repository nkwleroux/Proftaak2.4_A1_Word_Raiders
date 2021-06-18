#pragma once
#pragma once

#include<iostream>
#include<string>
#include<Vector>

class Word
{
public:
	/*
	* Constructor of the word class.
	* std::string str = The string value of the word
	*/
	Word(std::string str);

	/*
	* Deconstructor of the word class.
	*/
	Word() = delete;
	~Word() = default;

	/*
	* This function returns the length of the word.
	*/
	int getWordLength();

	/*
	* This function returns the word object.
	*/
	std::string getWord();

	/*
	* This function returns the array of letters the word consists of
	*/
	std::vector<char> getLetters();

	/*
	* This function creates a vector with letters of the word
	*/
	void createLettersArray();

	/*
	* This function prints the letters of the word in the console.
	*/
	void writeLetters();

	/*
	* This function adds random letters to the vector.
	* The function adds as many letters as the original length of the word.
	* So when wordlength = 5, also 5 extra letters are added. In total there are 10 letters.
	*/
	void createRandomLetters();

	/*
	* This function returns the first letter of the word
	*/
	char getFirstLetter();

private:
	//string attribute of the word
	std::string word;

	//vector attribute with the letters of the word and random letters
	std::vector<char> letters;

	/*
	* This function adds a letter to a vector.
	*/
	void addLetter(char letter);
};

