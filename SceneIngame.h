#pragma once
#include "Scene.h"
class SceneIngame :
    public Scene
{

public:
    SceneIngame();
    virtual void draw() override;
    virtual void update() override;
};

