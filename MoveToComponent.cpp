#include "MoveToComponent.h"
#include "GameObject.h"

// Constructor
MoveToComponent::MoveToComponent()
{
}

// Destructor
MoveToComponent::~MoveToComponent()
{
}

// Update function in which you can pass the elapsedtime since last update
void MoveToComponent::update(float elapsedTime)
{
	gameObject->position = (1 - speed) * gameObject->position + speed * targetLocation;
}
