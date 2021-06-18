#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "Texture.h"
#include <vector>

// Using to make things a bit easier
using tigl::Vertex;

class SkyboxComponent : public DrawComponent
{
	// Variable for list of vertexes
	std::vector<std::vector<Vertex>> vertexList;
	// Variables for x y and z axis
	float x = 0, y = 0, z = 0;

public:
	// Constructor in which we can pass size and textures.
	SkyboxComponent(float size, Texture** textures);
	// 	Constructor in which we can pass size and textures and the length!
	SkyboxComponent(float height, float width, float length, Texture** textures);
	// Destructor
	~SkyboxComponent();

	// Textures variable
	Texture** textures;

	// Draw function for the SkyboxComponent
	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};

