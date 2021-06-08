#include "BombModelComponent.h"
#include "ObjectModelComponent.h"
#include <string>

BombModelComponent::BombModelComponent() : ObjectModelComponent{ std::string("models/bomb/bomb").append(".obj")}
{
}

BombModelComponent::~BombModelComponent() {
}
