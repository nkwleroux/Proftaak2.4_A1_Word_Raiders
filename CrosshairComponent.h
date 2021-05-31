#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "Texture.h"
using tigl::Vertex;

#include <vector>

class CrosshairComponent : public DrawComponent
{
	std::vector<Vertex> verts;
	Texture* texture;
public:
	CrosshairComponent(float size);
	~CrosshairComponent();

	//virtual void setTexture(Texture* t) override;
	virtual void draw() override;

};

