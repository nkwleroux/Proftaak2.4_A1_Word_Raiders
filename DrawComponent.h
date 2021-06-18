#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

class DrawComponent : public Component
{
public:
	// Constructor
	DrawComponent();

	// Destructor
	~DrawComponent();

	//  =0 means nullpointer
	virtual void draw() = 0;

	// Getter for the vertices
	virtual	std::vector<glm::vec3> getVertices() = 0;

};

