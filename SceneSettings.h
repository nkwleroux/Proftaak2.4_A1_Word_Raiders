#pragma once
#include "Scene.h"
#include "tigl.h"
#include <sstream>
#include "VisionCamera.h"

class Texture;

class SceneSettings :
    public Scene
{

private:
	// Variable for the texture we should load to this scene,
    Texture* settingsTexture;
	// Metho dto convert integer to string variable type
    std::string intToString(int number);

public:
	// Vision camera
	VisionCamera* VC;

	//Constructor
    SceneSettings();
	// Draw function for the scene
	virtual void draw() override;
	// Update function for the scene
	virtual void update() override;
	// Function that frees/unbinds the textures for the scene
	virtual void freeTextures() override;
	// Function that changes the VisionCamera settings
	void changeColor(int index);
};
