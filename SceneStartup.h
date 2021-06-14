#pragma once
#include "Scene.h"
#include "tigl.h"
class Texture;

class SceneStartup : public Scene
{
private:
	Texture* startupTexture;

public:
	SceneStartup();

	virtual void draw() override;
	virtual void update() override;
	virtual void onKey(int key, int scancode, int action, int mods) override;
	virtual void freeTextures() override;
};

