#pragma once
#include "ObjectModelComponent.h"

class LetterModelComponent :
    public ObjectModelComponent
{
private:
    char blockLetter;

public:
	// Constructor
    LetterModelComponent(char letter, Texture* texture);
	// Destructor
    ~LetterModelComponent();
	// Getter for retreiving the letter
    char getLetter();
	// Boolean for if the letter has been shot or not
    bool hasBeenShot = false;
};

