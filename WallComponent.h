#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "Texture.h"
using tigl::Vertex;

#include <vector>

class WallComponent : public DrawComponent
{
	std::vector<Vertex> verts;
	tigl::VBO* vbo;

public:
	WallComponent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4,
		glm::vec2 texturePos1, glm::vec2 texturePos2, glm::vec2 texturePos3, glm::vec2 texturePos4, 
		glm::vec3 normal);
	~WallComponent();

	Texture* texture;

	virtual void draw() override;

};

