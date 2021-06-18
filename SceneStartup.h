#pragma once
#include "Scene.h"
#include "tigl.h"
class Texture;

class SceneStartup : public Scene
{
private:
	// Variable for the texture we should load to this scene,
	Texture* startupTexture;

public:
	//Constructor
	SceneStartup();
	// Draw function for the scene
	virtual void draw() override;
	// Update function for the scene
	virtual void update() override;
	// Function that frees/unbinds the textures for the scene
	virtual void freeTextures() override;
};

