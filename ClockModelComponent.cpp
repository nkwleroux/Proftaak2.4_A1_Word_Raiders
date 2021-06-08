#include "ClockModelComponent.h"
#include "ObjectModelComponent.h"
#include <string>

ClockModelComponent::ClockModelComponent() : ObjectModelComponent{ std::string("models/clock/Clock_obj").append(".obj")}
{
}

ClockModelComponent::~ClockModelComponent() {
}
