#pragma once
#include "Scene.h"
#include "tigl.h"
#include "glm/glm.hpp"

class GameObject;
class Texture;

class SceneIngame : public Scene {

private:
	void duringGame();
	void checkWord();
	void showWord();
	void clearVector();
	void rayCast(int xOrigin, int yOrigin, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	void initSkyboxTextures();
	void createLetterCubes();

	glm::vec4 mouse3d;
	GameObject* debugCube;
public:
	SceneIngame();
	virtual void draw() override;
	virtual void update() override;
};

