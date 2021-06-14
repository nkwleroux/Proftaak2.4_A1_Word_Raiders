#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

class Crosshair
{
private:
	bool handIsOpen;
	Texture* openHand;
	Texture* closedHand;
	glm::vec2 crosshairCoordinate;

public:
	Crosshair();
	void draw();
	void setHandStyle(bool isOpen);
	void update(glm::vec2 coordinate);
	void freeTextures();
};

