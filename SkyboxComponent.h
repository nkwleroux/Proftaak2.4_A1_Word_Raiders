#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "Texture.h"
using tigl::Vertex;

#include <vector>

class SkyboxComponent : public DrawComponent
{
	std::vector<std::vector<Vertex>> verts;
	float x = 0, y = 0, z = 0;

public:
	SkyboxComponent(float size, Texture** textures);
	SkyboxComponent(float height, float width, float length, Texture** textures);

	~SkyboxComponent();

	Texture** textures;

	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};

