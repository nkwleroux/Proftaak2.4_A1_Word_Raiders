#pragma once

#include "DrawComponent.h"
#include "Texture.h"
#include "tigl.h"

using tigl::Vertex;

#include <vector>

class CubeComponent : public DrawComponent
{
	Texture* texture;
public:
	std::vector<Vertex> verts;

	CubeComponent(float size);
	~CubeComponent();

	//virtual void setTexture(Texture* t) override;
	virtual void draw() override;


	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};

