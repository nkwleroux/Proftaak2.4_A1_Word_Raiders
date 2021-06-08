#include "WordLoader.h"
#include <ctime>
#include <cstdlib>

using json = nlohmann::json;

WordLoader::WordLoader() {
}

std::vector<Word*> WordLoader::loadWords(int wordType, const DIFFICULTY diff) {
	std::vector<Word*> words;
	int* numberArr = new int[diff];
	json j;
	std::ifstream loader("words.json");
	loader >> j;
	json wordlist;
	switch (wordType) {
	case 5: wordlist = j["fiveletterwords"]["wordslist"];
		break;
	case 6:	wordlist = j["sixletterwords"]["wordslist"];
		break;
	case 7:	wordlist = j["sevenletterwords"]["wordslist"];
		break;
	}
	int i = 0;
	srand(time(0));
	while (i < diff) {
		int rng = rand() % wordlist.size();
		if (checkIfNumberExists(numberArr, rng, diff))
		{
			numberArr[i] = rng;
			std::string word = wordlist[rng];
			std::transform(word.begin(), word.end(), word.begin(), ::toupper);
			Word* wrd = new Word(word);
			words.push_back(wrd);
			i++;
		}
	}
	return words;
}

bool WordLoader::checkIfNumberExists(int arr[], int number, int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == number) {
			return false;
		}
	}
	return true;
}


void WordLoader::printWords(std::vector<Word*> words) {
	for (Word* w : words) {
		//std::cout << w->getWord() << std::endl;
		w->writeLetters();
	}
}
