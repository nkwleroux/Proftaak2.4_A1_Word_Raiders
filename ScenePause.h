#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class ScenePause :
    public Scene
{

private:
	// Variable for the texture we should load to this scene,
    Texture* pauseTexture;

public:
    ScenePause();
	// Draw function for the scene
	virtual void draw() override;
	// Update function for the scene
	virtual void update() override;
	// Function that frees/unbinds the textures for the scene
	virtual void freeTextures() override;
};

