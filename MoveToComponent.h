#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class MoveToComponent : public Component
{
public:
	// Speed of the moving component
	float speed = 0.01f;
	// Where the moving component should go
	glm::vec3 targetLocation;

	// Constructor
	MoveToComponent();

	// Destructor
	~MoveToComponent();

	// Update function in which you can pass the elapsedtime since last update
	virtual void update(float elapsedTime) override;
};

