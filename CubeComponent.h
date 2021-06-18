#pragma once

#include "DrawComponent.h"
#include "Texture.h"
#include "tigl.h"
#include <vector>

// Using to make it a bit easier :)
using tigl::Vertex;

class CubeComponent : public DrawComponent
{
	//Texture of the cubecomponent
	Texture* texture;
public:
	// Global variable for vertexes
	std::vector<Vertex> vertexesList;

	// Constructor, you can pass trough a size float
	CubeComponent(float size);

	//Destructor
	~CubeComponent();

	// Draw function for the cube
	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};

