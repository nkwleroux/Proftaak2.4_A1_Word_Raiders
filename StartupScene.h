#pragma once
#include "Scene.h"
#include "tigl.h"
class Texture;

class StartupScene : public Scene
{
private:
	Texture* startupTexture;
	tigl::VBO* kubusVbo;

	float time;

	int menuIndex;

public:
	StartupScene();


	virtual void draw() override;
	virtual void update() override;
	virtual void onKey(int key, int scancode, int action, int mods) override;
};

