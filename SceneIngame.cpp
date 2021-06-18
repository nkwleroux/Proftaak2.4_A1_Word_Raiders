#include "SceneIngame.h"
#include "Scene.h"
#include "VisionCamera.h"
#include "Texture.h"
#include "tigl.h"
#include "stb_image.h"
#include "tigl.h"
#include "Text/Text.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "Timer.h"
#include "Word.h"
#include "WordLoader.h"
#include "BoundingBoxComponent.h"
#include "SkyboxComponent.h"
#include "LetterModelComponent.h"

#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include <iomanip>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <map>
#include "Crosshair.h"
#include "GameLogic.h"
#include <unordered_map>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using tigl::Vertex;
using namespace std;
using namespace cv;


SceneIngame::SceneIngame()
{
	VideoCapture cap(0);
	VC = new VisionCamera(cap);
	gameLogic = new GameLogic();

	initSkyboxTextures();

	// Initialise text object with verdana font
	textObject = new Text("c:/windows/fonts/Verdana.ttf", 64.0f);

	// Create skybox
	skyBox = new GameObject(0);
	skyBox->position = glm::vec3(0, 0, 5);
	skyBox->addComponent(new SkyboxComponent(50, textureSkybox));
	skyBox->addComponent(new BoundingBoxComponent(skyBox));

	crosshair = new Crosshair();

	// Create letterModelComponents for all letters
	char chars[] = { 'A','B','C' ,'D' ,'E' ,'F' ,'G' ,'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W' ,'X' ,'Y' ,'Z' };
	Texture* texture = new Texture("resources/LetterBlockTexture.png");
	for (int i = 0; i < sizeof(chars); i++) {
		lettersMap[chars[i]] = new LetterModelComponent(chars[i], texture);
	}

	lastFrameTime = 0;
}


void SceneIngame::draw()
{
	glm::vec3 cameraPosition(0, 0, 30);
	// Get the viewport variables
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Set the projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);

	// Set the viewmatrix
	glm::mat4 viewmatrix = glm::lookAt(cameraPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	tigl::shader->setViewMatrix(viewmatrix);

	// Set the modelMatrix to a standard matrix
	glm::mat4 modelmatrix(1.0f);
	tigl::shader->setModelMatrix(modelmatrix);

	// Set the light in the scene
	tigl::shader->setLightCount(1);
	tigl::shader->setShinyness(1.0f);
	tigl::shader->setLightPosition(0, glm::vec3(0,0,1));
	tigl::shader->setLightDirectional(0, true);
	
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	tigl::shader->enableLighting(true);

	// drawing objects
	for (auto& o : dynamicObjectsList) {
		o->draw();
	}

	// Calculate where the crosshair would hit
	//rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y, viewmatrix, projection);

	// Debug use mouse as pointer
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			VC->redDetected = true;
		}
		rayCast(xpos, ypos, viewmatrix, projection);
	}

	// Disable lighting when drawing the skybox and 2d aspects
	tigl::shader->enableLighting(false);
	skyBox->draw();

	// 2D objects drawing
	glDisable(GL_DEPTH_TEST);

	// Draw all the gui details
	textObject->draw("Score: " + gameLogic->getScore(), 50.0, 50.0, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getGameTimer()->timeRemainingToString(), 50.0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw("Levens: " + gameLogic->getCurrentLifes(), 50.0, 150, glm::vec4(1.0f, 1.0f, 1.0f, 0));	
	textObject->draw(gameLogic->getShotWord(), windowWidth / 2 - 100, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getCorrectWord(), windowWidth - 300, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));

	// Draw the crosshair
	crosshair->draw();
}

/// <summary>
/// Helper function to generate a random vector 
/// </summary>
/// <param name="max"> Maximum value </param>
/// <param name="xCollide"> If true it will randomise x component </param>
/// <param name="yCollide"> If true it will randomise y component </param>
/// <param name="zCollide"> If true it will randomise z component </param>
/// <returns> Vector with random components if specified </returns>
glm::vec3 RandomVec3(float max, bool xCollide, bool yCollide, bool zCollide) {
	float x = 0, y = 0, z = 0;
	int isNegative;

	if (xCollide) {
		x = (float(rand()) / float((RAND_MAX)) * max);

		isNegative = (float(rand()) / float((RAND_MAX)) * 2);
		if (isNegative == 1) {
			x *= -1;
		}
	}

	if (yCollide) {
		y = (float(rand()) / float((RAND_MAX)) * max);

		isNegative = (float(rand()) / float((RAND_MAX)) * 2);
		if (isNegative == 1) {
			y *= -1;
		}
	}

	if (zCollide) {
		z = (float(rand()) / float((RAND_MAX)) * max);

		isNegative = (float(rand()) / float((RAND_MAX)) * 2);
		if (isNegative == 1) {
			z *= -1;
		}
	}

	return glm::vec3(x, y, z);
}

void SceneIngame::update() {
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
			if (key == GLFW_KEY_P && action == GLFW_PRESS)
			{
				currentScene = sceneList[Scenes::PAUSE];
			}
		});

	// Update vision component
	VC->update();
	crosshair->setHandStyle(!VC->currentCrosshair);

	// Select object where mouse is hovering over
	selectObject();

	if (gameLogic->reset)
	{
		gameLogic->reset = false;
		createLetterCubes();		
		//return;
	}

	// Update the gameLogic
	bool hasFinished = gameLogic->update(&(VC->redDetected));

	// If has finished all the words are guessed or the timer has run out
	if (hasFinished)
	{
		currentScene = sceneList[Scenes::GAMEEND];
		gameLogic->gameStarted = false;
		return;
	}

	// If the selectedObject is not null and is an letterModelComponent
	if (gameLogic->selectedObject != nullptr && gameLogic->selectedObject->getComponent<LetterModelComponent>()) {
		// Check if it as been shot
		if (gameLogic->selectedObject->getComponent<LetterModelComponent>()->hasBeenShot)
		{
			// Remove it from the list and dereference the object
			dynamicObjectsList.remove(gameLogic->selectedObject);
			gameLogic->selectedObject->getComponent<LetterModelComponent>()->hasBeenShot = false;
			gameLogic->selectedObject = nullptr;
		}
	}

	// If the word is correct, clear the dynamic objects and readd the new objects
	if (gameLogic->wordCorrect) {
		dynamicObjectsList.clear();
		for (auto const& o : fullObjectsList)
		{
			dynamicObjectsList.push_back(o);
		}
		gameLogic->wordCorrect = false;
	}

	// Update the position of the crosshair
	crosshair->update(VC->getCrossHairCoords());

	// Calculate the time that has past, used to move the component by the same amount every second
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Check for collisions
	for (auto& o : dynamicObjectsList) {
		glm::vec3 oTarget = (o->getComponent<MoveToComponent>()->targetLocation);

		for (auto& next : dynamicObjectsList) {

			//Skip first element so you can compare with the previous one
			if (next != o) {
				if (o->getComponent<BoundingBoxComponent>() && next->getComponent<BoundingBoxComponent>() && o->getComponent<BoundingBoxComponent>()->collideWithObject(next)) {

					BoundingBoxComponent* oBox = o->getComponent<BoundingBoxComponent>();

					glm::vec3 temp = glm::vec3(0);

					if (oBox->collisionX) {
						temp += glm::vec3(next->position.x - o->position.x, 0, 0);
					}
					if (oBox->collisionY) {

						temp += glm::vec3(0, next->position.y - o->position.y, 0);
					}
					if (oBox->collisionX) {
						temp += glm::vec3(0, 0, next->position.z - o->position.z);
					}

					o->position -= temp / 8.0f;

					glm::vec3 difference = glm::vec3(oTarget.x - o->position.x, oTarget.y - o->position.y, oTarget.z - o->position.z);

					int oTargetX = oTarget.x;
					int oTargetY = oTarget.y;

					if (difference.x >= 0) {
						oTargetX *= -1;
					}

					if (difference.y >= 0) {
						oTargetY *= -1;
					}
					
					oTarget = glm::vec3(oTargetX, oTargetY, oTarget.z);
					
					o->getComponent<MoveToComponent>()->targetLocation = oTarget;

					oBox->collisionX = false;
					oBox->collisionZ = false;
					oBox->collisionY = false;
					break;

				}
			}
		}

		// Check collision with skybox
		if (skyBox != nullptr && skyBox->getComponent<BoundingBoxComponent>()->collideWithWall(o)) {
			glm::vec3 oTarget = (o->getComponent<MoveToComponent>()->targetLocation);

			oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
			o->getComponent<MoveToComponent>()->targetLocation = oTarget;
		}

		glm::vec3 difference = glm::vec3(oTarget.x - o->position.x, oTarget.y - o->position.y, oTarget.z - o->position.z);
		if (difference.x <= 1.0f && difference.y <= 1.0f) {
			o->getComponent<MoveToComponent>()->targetLocation = RandomVec3(25, true, true, false);
		}

		// Update every object
		o->update(deltaTime);
	}
}

void SceneIngame::freeTextures()
{
	for (int i = 0; i < 6; i++)
	{
		textureSkybox[i]->unBind();
	}
	crosshair->freeTextures();
}

void SceneIngame::rayCast(int xOrigin, int yOrigin, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	// If no mouseposition is given we don't cast a ray
	if (xOrigin == 0 && yOrigin == 0)
	{
		return;
	}

	// get the viewports
	int Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);

	// Calculate the real y
	yOrigin = Viewport[3] - yOrigin;

	// Calculate the Z of the point
	float winZ;
	glReadPixels(xOrigin, yOrigin, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	// Calculate the 3d coordinates of the intersection with the viewport
	mouse3d = glm::vec4(glm::unProject(glm::vec3(xOrigin, yOrigin, winZ), viewMatrix, projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3])), winZ);
}

void SceneIngame::initSkyboxTextures() {
	textureSkybox[0] = new Texture("Images/skybox_middle.png"); //middle
	textureSkybox[1] = new Texture("Images/skybox_right.png"); //right
	textureSkybox[2] = new Texture("Images/skybox_left.png"); //left
	textureSkybox[3] = new Texture("Images/skybox_right2.png"); //back
	textureSkybox[4] = new Texture("Images/skybox_bottom.png"); // bottom
	textureSkybox[5] = new Texture("Images/skybox_top.png"); // top
}

void SceneIngame::createLetterCubes()
{
	// Clear all the previous objects
	dynamicObjectsList.clear();
	fullObjectsList.clear();

	// Add new objects that are used by the new word
	for (int i = 0; i < gameLogic->getCurrentWord()->getLetters().size(); i++) {
		GameObject* o = new GameObject(i);
		o->addComponent(lettersMap[gameLogic->getCurrentWord()->getLetters()[i]]);
		o->addComponent(new BoundingBoxComponent(o));
		o->addComponent(new MoveToComponent());
		glm::vec3 pos = glm::vec3(0, 0, 0); //original position
		o->position = pos;
		o->scale = glm::vec3(1.0f);
		o->getComponent<MoveToComponent>()->targetLocation = pos;
		o->draw();

		// move blocks if they spawn in each other
		for (auto& next : dynamicObjectsList) {
			if (next != o) {
				while (o->getComponent<BoundingBoxComponent>()->collideWithObject(next)) {
					glm::vec3 pos = RandomVec3(25, true, true, false);
					o->position = pos;
					o->getComponent<MoveToComponent>()->targetLocation = RandomVec3(30, true, true, false);
					o->draw();
				}
			}
		}
		dynamicObjectsList.push_back(o);
		fullObjectsList.push_back(o);
	}
}

void SceneIngame::selectObject() {
	double minimalDistance = 0;
	GameObject* object = nullptr;
	glm::vec3 mousePos(mouse3d.x, mouse3d.y, mouse3d.z);

	// Compare every object of the distance to that object is the smallest, that object is hit
	for (const auto& o : dynamicObjectsList) {
		if (minimalDistance == 0)
		{
			minimalDistance = glm::distance(o->position, mousePos);
			object = o;
			continue;
		}

		double distance = glm::distance(o->position, mousePos);

		if (distance < minimalDistance)
		{
			object = o;
			minimalDistance = distance;
		}
	}

	// If the distance to the closest object is too large we know we didn't hit any objects
	if (minimalDistance < 10)
	{
		gameLogic->selectedObject = object;
	}
	else {
		gameLogic->selectedObject = nullptr;
	}
}