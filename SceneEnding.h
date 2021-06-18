#pragma once
#include "Scene.h"
#include "tigl.h"
#include <sstream>
#include "Text/text.h"

class Texture;

class SceneEnding : public Scene{

private:
	// Variable for the texture we should load to this scene,
    Texture* endingTexture;
	// Method to convert integer to string variable type
    std::string intToString(int number);
	// Method to get the winning text (your score etc)
    std::string getWonText(bool won);

	// Text that should be shown variables
	Text* text;
public:
    SceneEnding();
	// Draw function for the scene
	virtual void draw() override;
	// Update function for the scene
	virtual void update() override;
	// Function that frees/unbinds the textures for the scene
	virtual void freeTextures() override;
};
