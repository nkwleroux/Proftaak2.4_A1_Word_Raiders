#include "LetterModelComponent.h"
#include "ObjectModelComponent.h"
#include <string>

// Constructor
// We append a texture for a certain letter to the component
LetterModelComponent::LetterModelComponent(char letter, Texture* texture) : ObjectModelComponent{ std::string("resources/Cube_Word_Raiders").append(1,letter).append(".obj"), texture }
{
    blockLetter = letter;
}

// Destructor
LetterModelComponent::~LetterModelComponent() {
}

// Getter for retreiving the letter
char LetterModelComponent::getLetter()
{
    return blockLetter;
}
