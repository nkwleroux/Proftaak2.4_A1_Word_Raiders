#pragma once
#include "Scene.h"
#include "tigl.h"
#include "glm/glm.hpp"

class GameObject;
class Texture;

class SceneIngame : public Scene {

private:
    void duringGame();
    bool checkWord();
    void showWord();
    void clearVector(std::vector<char> *vector);
    void rayCast(int xOrigin, int yOrigin);
    void addShootedLetter(char letter, int index);
    void drawShootedWords();
    void fillVector();
    void gameLogic();

	glm::vec4 mouse3d;
	GameObject* debugCube;
public:
	SceneIngame();
	virtual void draw() override;
	virtual void update() override;
};

