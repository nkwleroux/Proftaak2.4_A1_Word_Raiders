#pragma once
#include "Scene.h"
#include "tigl.h"

class Texture;

class SceneCredits :
    public Scene
{

private:
    Texture* creditsTexture;

public:
    SceneCredits();
    virtual void draw() override;
    virtual void update() override;
};

