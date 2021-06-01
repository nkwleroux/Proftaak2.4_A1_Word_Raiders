#pragma once

#include<iostream>
#include<string>
#include<Vector>

class Word
{
public:
	Word(std::string str);
	Word() = delete;
	~Word() = default;
	int getWordLength();
	std::string getWord();
	std::vector<char> getLetters();
	void createLettersArray();
	void writeLetters();

private:
	std::string word;
	std::vector<char> letters;
};