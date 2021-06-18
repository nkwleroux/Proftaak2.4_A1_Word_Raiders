#pragma once
class Scene
{
public:
	// Each scene should have a draw method
	virtual void draw() = 0;
	// Each scene should have a update method
	virtual void update() = 0;
	// Each scene should have a onkey method
	virtual void onKey(int key, int scancode, int action, int mods) {};
	// Each scene should have a freeTextures method
	virtual void freeTextures() = 0;
};


// Enum of the types of scenes we have
enum class Scenes
{
	STARTUP,
	INGAME,
	SETTINGS,
	CREDITS,
	PAUSE,
	GAMEEND,
};
