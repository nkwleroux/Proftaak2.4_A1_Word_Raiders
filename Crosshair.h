#pragma once
#include "Texture.h"

class Crosshair
{
private:
	bool handIsOpen;
	Texture* openHand;
	Texture* closedHand;
	float x, y;

public:
	Crosshair();
	void draw();
	void setHandStyle(bool isOpen);
	void update(float x, float y);
};

