#pragma once
#include "Texture.h"

class Crosshair
{
private:
	bool handIsOpen;
	Texture* openHand;
	Texture* closedHand;

public:
	Crosshair();
	void draw();
	void setHandStyle(bool isOpen);
	void update(double crosshairX, double crosshairY);
};

