#pragma once

#include "Component.h"
#include <list>
#include <glm/glm.hpp>

class DrawComponent : public Component
{
public:
	DrawComponent();
	~DrawComponent();
	//virtual void setTexture(Texture* t) = 0;
	virtual void draw() = 0;

	virtual	std::list<glm::vec3> getVertices() = 0;

};

