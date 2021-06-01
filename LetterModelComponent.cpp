#include "LetterModelComponent.h"
#include "ObjectModelComponent.h"
#include <string>

LetterModelComponent::LetterModelComponent(char letter) : ObjectModelComponent{ std::string("resources/Cube_Word_Raiders").append(1,letter).append(".obj") }
{
}

LetterModelComponent::~LetterModelComponent() {
}
