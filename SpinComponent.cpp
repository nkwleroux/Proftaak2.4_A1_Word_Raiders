#include "SpinComponent.h"
#include "GameObject.h"

#include <iostream>

SpinComponent::SpinComponent(float speed)
{
	this->speed = speed;
}


SpinComponent::~SpinComponent()
{
}

void SpinComponent::update(float elapsedTime)
{
	gameObject->rotation.y += elapsedTime * speed;
}
