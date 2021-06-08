#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class SceneIngame :
    public Scene
{

private:
    Texture* inGameTexture;

public:
    SceneIngame();
    virtual void draw() override;
    virtual void update() override;
};

