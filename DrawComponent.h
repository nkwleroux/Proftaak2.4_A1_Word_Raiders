#pragma once

#include "Component.h"

class DrawComponent : public Component
{
public:
	DrawComponent();
	~DrawComponent();
	//virtual void setTexture(Texture* t) = 0;
	virtual void draw() = 0;
};

