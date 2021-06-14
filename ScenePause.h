#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class ScenePause :
    public Scene
{

private:
    Texture* pauseTexture;

public:
    ScenePause();
    virtual void draw() override;
    virtual void update() override;
    virtual void freeTextures() override;
};

