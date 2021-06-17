#pragma once
#include "ObjectModelComponent.h"

class LetterModelComponent :
    public ObjectModelComponent
{
private:
    char blockLetter;

public:
    LetterModelComponent(char letter, Texture* texture);
    ~LetterModelComponent();
    char getLetter();
};

