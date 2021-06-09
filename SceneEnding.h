#pragma once
#include "Scene.h"
#include "tigl.h"
#include <sstream>

class Texture;

class SceneEnding : public Scene{

private:
    Texture* endingTexture;
    std::string intToString(int number);
    std::string getWonText(bool won);

public:
    SceneEnding();
    virtual void draw() override;
    virtual void update() override;
};
