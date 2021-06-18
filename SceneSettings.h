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
    Texture* settingsTexture;
    std::string intToString(int number);
    void changeColor();

public:
    SceneSettings();
    virtual void draw() override;
    virtual void update() override;
    virtual void freeTextures() override;
    VisionCamera* VC;
};
