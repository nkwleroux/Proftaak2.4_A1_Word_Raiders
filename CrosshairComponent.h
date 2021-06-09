#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "Texture.h"

using tigl::Vertex;

#include <vector>

class CrosshairComponent : public DrawComponent
{
	std::vector<Vertex> verts;
private:
	Texture* textureCrosshair[2];
	
	void initTextures();

public:
	int currentCrosshair = 0;

	CrosshairComponent(float size);
	~CrosshairComponent();

	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;
};

