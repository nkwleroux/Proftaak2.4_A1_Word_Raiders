#pragma once

#include "Component.h"

class PlayerComponent : public Component
{
	// Moving speed is 5
	float speed = 5;
public:
	//Constructor
	PlayerComponent();
	//Destructor
	~PlayerComponent();
	// Update function for PlayerComponent
	virtual void update(float elapsedTime) override;
};

