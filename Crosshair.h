#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

class Crosshair
{
private:
	// Boolean to check if hand is open
	bool handIsOpen;
	// Texture for when the hand is open
	Texture* openHand;
	// Texture for when the hand is closed
	Texture* closedHand;
	// Coordinates of crosshair
	glm::vec2 crosshairCoordinate;

public:
	Crosshair();
	// Draw method for the crosshair
	void draw();
	// Method for setting handstyle
	void setHandStyle(bool isOpen);
	// Update method for the crosshair
	void update(glm::vec2 coordinate);
	// Unbind the textures basically freeing them
	void freeTextures();
};

