#pragma once
#include "Scene.h"
#include "tigl.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Scene.h"
#include "Texture.h"
#include "VisionCamera.h"
#include <unordered_map>
#include "GameObject.h"
#include "GameLogic.h"
#include "Text/Text.h"
#include "Crosshair.h"
#include "LetterModelComponent.h"

class GameObject;
class Texture;


// Map used to navigate between sceneList
extern std::map<Scenes, Scene*> sceneList;

// Currently selected scene
extern Scene* currentScene;

// Window in which everything is displayed
extern GLFWwindow* window;

// Variables that keep the size of the display window
extern int windowHeight;
extern int windowWidth;

/// <summary>
/// Scene in which the gameplay takes place
/// </summary>
class SceneIngame : public Scene {

private:

	// Location where the ray intersects whith an object
	glm::vec4 mouse3d;

	// Textures used by the sky box
	Texture* textureSkybox[6];

	// Texture used to draw the crosshair
	Texture* textureCrosshair[2];

	// Vision camera object which is used to detect where the player wants to shoot
	VisionCamera* VC;

	// Text object used to draw the gui elements
	Text* textObject;

	// Map used to store the letterblock texture
	std::unordered_map<char, LetterModelComponent*> lettersMap;

	// List used to store all the letter cube objects
	std::list<GameObject*> fullObjectsList;

	// List that dynamicly changes, like when letters are shot
	std::list<GameObject*> dynamicObjectsList;

	// Time used to calculate how how much time has passed since last frame
	double lastFrameTime;

	// Object containing the boundingbox
	GameObject* skyBox;

	// Crosshair object used to draw a crosshair with
	Crosshair* crosshair;

	// GameLogic object that contains all the code behind the game
	GameLogic* gameLogic;

	/// <summary>
	/// Method used to cast an ray in the viewport
	/// Method will return a 3d point where the ray hit 
	/// </summary>
	/// <param name="xOrigin"> X coordinate of the crosshair </param>
	/// <param name="yOrigin"> Y coordinate of the crosshair </param>
	/// <param name="viewMatrix"> Matrix containing where in the scene is looked at </param>
	/// <param name="projectionMatrix"> Matrix containing how the scene is viewed </param>
	void rayCast(int xOrigin, int yOrigin, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

	/// <summary>
	/// Initialise skybox textures that are used to draw the skybox
	/// </summary>
	void initSkyboxTextures();

	/// <summary>
	/// Method that creates all the cubes that are used in the word
	/// </summary>
	void createLetterCubes();

	/// <summary>
	/// Method to select the object that is closest to the raycast location
	/// </summary>
	void selectObject();

public:

	/// <summary>
	/// Constructor to initialise all the variables
	/// </summary>
	SceneIngame();

	/// <summary>
	/// Method used to draw all the sceneList components
	/// </summary>
	virtual void draw() override;

	/// <summary>
	/// Method used to update all the scene components
	/// </summary>
	virtual void update() override;

	/// <summary>
	/// Method used to unbind all the textures
	/// </summary>
	virtual void freeTextures() override;
};

