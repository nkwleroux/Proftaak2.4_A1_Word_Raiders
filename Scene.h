#pragma once
class Scene
{
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void onKey(int key, int scancode, int action, int mods) {};
	virtual void freeTextures() = 0;
};



enum class Scenes
{
	STARTUP,
	INGAME,
	SETTINGS,
	CREDITS,
	PAUSE,
	GAMEEND,
};
