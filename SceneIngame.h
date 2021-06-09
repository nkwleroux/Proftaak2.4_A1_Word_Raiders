#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class SceneIngame : public Scene {

private:
	void duringGame();
	void checkWord();
	void showWord();
	void clearVector();
	void rayCast(int xOrigin, int yOrigin);
	void initSkyboxTextures();
	void createLetterCubes();
public:
	SceneIngame();
	virtual void draw() override;
	virtual void update() override;
};

