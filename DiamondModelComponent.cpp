#include "DiamondModelComponent.h"
#include "ObjectModelComponent.h"
#include <string>

DiamondModelComponent::DiamondModelComponent() : ObjectModelComponent{ std::string("models/diamond/test").append(".obj") }
{
}

DiamondModelComponent::~DiamondModelComponent() {
}
