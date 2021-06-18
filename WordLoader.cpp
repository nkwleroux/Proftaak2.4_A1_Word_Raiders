#include "WordLoader.h"
#include <ctime>
#include <cstdlib>

using json = nlohmann::json;

/*
* This is the constructor of the wordloader class.
* The constructor doesn't have parameters
*/
WordLoader::WordLoader() {
}

/*
* This function loads the words from the json list.
* int wordType = length of the word: 5, 6 or 7 letters.
* int wordAmount = the amount of words to load.
*/
std::vector<Word*> WordLoader::loadWords(int wordType, const int wordAmount) {
	//create a new vector where the words can be stored.
	std::vector<Word*> words; 
	int* numberArr = new int[wordAmount];
	//create a json object
	json j; 
	//create a new json loader. The file name is the only parameter
	std::ifstream loader("words.json"); 
	// load all words of the file in the json object
	loader >> j;
	json wordlist;
	switch (wordType) {
		// if the selected wordType = 5
		case 5: wordlist = j["fiveletterwords"]["wordslist"]; //load all words with length 5
			break;
		// if the selected wordType = 6
		case 6:	wordlist = j["sixletterwords"]["wordslist"]; //load all words with length 6
			break;
		// if the selected wordType = 7
		case 7:	wordlist = j["sevenletterwords"]["wordslist"]; //load all words with length 7
			break;
	}

	//create variables for the random function
	int i = 0;
	srand(time(0));

	while (i < wordAmount) { //do for every word
		int rng = rand() % wordlist.size(); //generate a random nummer
		if (checkIfNumberExists(numberArr, rng, wordAmount))
		{
			numberArr[i] = rng; //add the random nummer to the array
			std::string word = wordlist[rng]; //get word from the wordList by index
			std::transform(word.begin(), word.end(), word.begin(), ::toupper); //capitalize the word
			Word* wrd = new Word(word); //create word object of the string
			words.push_back(wrd); //add the wordobject to the vector with all words
			i++; //increase the index
		}
	}
	return words; //return the vector with words
}

/*
* This function checks if a random number is already used, to avoid duplicate words
*/
bool WordLoader::checkIfNumberExists(int arr[], int number, int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == number) { 
			return false; //return false if random number occurs yet
		}
	}
	return true; //return true if random number does not occur yet
}

/*
* This function prints all words of a given vector.
* This function is used during testing. Is not necessary after launching the game
*/
void WordLoader::printWords(std::vector<Word*> words) {
	for (Word* w : words) {
		//std::cout << w->getWord() << std::endl;
		w->writeLetters();
	}
}
