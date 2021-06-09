#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class SceneIngame : public Scene {

private:
    Texture* inGameTexture;
    void duringGame();
    bool checkWord();
    void showWord();
    void clearVector(std::vector<char> *vector);
    void rayCast(int xOrigin, int yOrigin);
    void addShootedLetter(char letter, int index);
    void drawShootedWords();
    void fillVector();
    void gameLogic();

public:
    SceneIngame();
    virtual void draw() override;
    virtual void update() override;
};

