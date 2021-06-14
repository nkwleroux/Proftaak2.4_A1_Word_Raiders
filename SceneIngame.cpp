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

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using tigl::Vertex;
using namespace std;
using namespace cv;

extern std::map<Scenes, Scene*> scenes;
extern Scene* currentScene;
extern GLFWwindow* window;

Texture* textureSkybox[6];
Texture* textureCrosshair[2];
extern int windowHeight;
extern int windowWidth;
VisionCamera* VC;
Text* textObject;
Text* wordText;
std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
Crosshair* crosshair;
GameLogic* gameLogic;

int textureIndex;
float rotation = 0;

SceneIngame::SceneIngame()
{
	VideoCapture cap(0);
	VC = new VisionCamera(cap);
	gameLogic = new GameLogic();

	initSkyboxTextures();

	textObject = new Text("c:/windows/fonts/Verdana.ttf", 64.0f);
	wordText = new Text("c:/windows/fonts/Verdana.ttf", 128.0f);

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new SkyboxComponent(50, textureSkybox));
	backgroundBox->addComponent(new BoundingBoxComponent(backgroundBox));

	crosshair = new Crosshair();
	createLetterCubes();
}

void SceneIngame::draw()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	glm::mat4 viewmatrix = glm::lookAt(glm::vec3(0, 0, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	tigl::shader->setViewMatrix(viewmatrix);

	glm::mat4 modelmatrix(1.0f);
	modelmatrix = glm::translate(modelmatrix, glm::vec3((float)((windowWidth / VC->videoWidth) * VC->currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / VC->videoHeight) * VC->currentPoint.y / -125.0f + 4.0)), 0.0f));
	tigl::shader->setModelMatrix(modelmatrix);

	glEnable(GL_DEPTH_TEST);
	//for outlines only
	//glpolygonmode(gl_front_and_back, gl_line);

	// drawing objects
	for (auto& o : objects) {
		o->draw();
	}

	// Calculate where the crosshair would hit
	rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y, viewmatrix, projection);
	
	// Debug use mouse as pointer
	//{
	//	double xpos, ypos;
	//	glfwGetCursorPos(window, &xpos, &ypos);
	//	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	//	if (state == GLFW_PRESS)
	//	{
	//		VC->redDetected = true;
	//	}
	//	rayCast(xpos, ypos, viewmatrix, projection);
	//}

	tigl::shader->enableLighting(false);

	backgroundBox->draw();

	// 2D objects drawing

	//timer
	textObject->draw("score: 200 stars  ", 50.0, 50.0, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getGameTimer()->secondsToGoString(), 50.0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw("levens: ******", 50.0, 150, glm::vec4(1.0f, 1.0f, 1.0f, 0));	
	textObject->draw(gameLogic->getShotWord(), windowWidth / 2 - 100, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getCorrectWord(), windowWidth - 300, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));

	crosshair->draw();
	glDisable(GL_DEPTH_TEST);
}

glm::vec3 RandomVec3(float max, bool xCollide, bool yCollide, bool zCollide) {
	float x = 0, y = 0, z = 0;
	if (xCollide)
		x = (float(rand()) / float((RAND_MAX)) * max);
	if (yCollide)
		y = (float(rand()) / float((RAND_MAX)) * max);
	if (zCollide)
		z = (float(rand()) / float((RAND_MAX)) * max);
	return glm::vec3(x, y, 0);
}

void SceneIngame::update() {
	//Maybe make into callback function. Now is dependent on update (timing).
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		currentScene = scenes[Scenes::PAUSE];
	}

	// Update vision component
	VC->update();
	crosshair->setHandStyle(VC->currentCrosshair == 0);


	// Select object where mouse is hovering over
	selectObject();
	// Update the gameLogic
	bool hasFinished = gameLogic->update(&(VC->redDetected));
	// If has finished all the words are guessed or the timer has run out
	if (hasFinished)
	{
		currentScene = scenes[Scenes::STARTUP];
		return;
	}

	if (gameLogic->reset)
	{
		createLetterCubes();
		gameLogic->reset = false;
		return;
	}

	// Update the position of the crosshair
	crosshair->update(VC->getCrossHairCoords());

	// Calculate the time that has past, used to move the component by the same amount every second
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Check for collisions
	for (auto& o : objects) {
		for (auto& next : objects) {

			//Skip first element so you can compare with the previous one
			if (next != o) {
				if (o->getComponent<BoundingBoxComponent>() && next->getComponent<BoundingBoxComponent>() &&o->getComponent<BoundingBoxComponent>()->collideWithObject(next)) {

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

					glm::vec3 oTarget = (o->getComponent<MoveToComponent>()->target);
					oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
					oTarget += RandomVec3(30, oBox->collisionX, oBox->collisionY, oBox->collisionZ);

					o->getComponent<MoveToComponent>()->target = oTarget;

					oBox->collisionX = false;
					oBox->collisionZ = false;
					oBox->collisionY = false;
					break;

				}
			}
		}
		if (backgroundBox != nullptr && backgroundBox->getComponent<BoundingBoxComponent>()->collideWithWall(o)) {
			glm::vec3 oTarget = (o->getComponent<MoveToComponent>()->target);
			oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
			o->getComponent<MoveToComponent>()->target = oTarget;
		}
		if (o->getComponent<MoveToComponent>()->target == o->position) {
			o->getComponent<MoveToComponent>()->target = RandomVec3(25, true, true, false) + glm::vec3(-1 * o->position.x, -1 * o->position.y, -1 * o->position.z);;
		}
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
	if (xOrigin == 0&&yOrigin == 0)
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
	mouse3d = glm::vec4(glm::unProject(glm::vec3(xOrigin,yOrigin, winZ), viewMatrix, projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3])), winZ);
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
	objects.clear();
	for (int i = 0; i < gameLogic->getCurrentWord()->getLetters().size(); i++) {
		GameObject* o = new GameObject(i);

		o->addComponent(new LetterModelComponent(gameLogic->getCurrentWord()->getLetters()[i]));
		o->addComponent(new BoundingBoxComponent(o));
		o->addComponent(new MoveToComponent());
		//o->getComponent<MoveToComponent>()->target = glm::vec3(rand() % 20, rand() % 20, 0);
		glm::vec3 pos = glm::vec3(0, 0, 0);
		o->position = pos;
		o->scale = glm::vec3(1.0f);
		o->getComponent<MoveToComponent>()->target = pos;
		o->draw();

		for (auto& next : objects) {
			if (next != o) {
				while (o->getComponent<BoundingBoxComponent>()->collideWithObject(next)) {
					glm::vec3 pos = glm::vec3(rand() % 20, rand() % 20, 0);
					o->position = pos;
					o->getComponent<MoveToComponent>()->target = glm::vec3(rand() % 30, rand() % 30, 0);
					//o->getComponent<MoveToComponent>()->target = glm::vec3(30, 30, 0);
					//o->getComponent<MoveToComponent>()->target = glm::vec3(-1*(next->getComponent<MoveToComponent>()->target.x), -1 * (next->getComponent<MoveToComponent>()->target.y), 0);
					o->draw();
					//o->update(0);
					//next->draw();
					//next->update(0);
				}
			}
		}
		objects.push_back(o);
	}
}

void SceneIngame::selectObject() {
	double minimalDistance = 0;
	GameObject* object = nullptr;
	glm::vec3 mousePos(mouse3d.x, mouse3d.y, mouse3d.z);

	for (const auto& o : objects) {
		if (minimalDistance == 0)
		{
			minimalDistance = glm::distance(o->position, mousePos);
			object = o;
			continue;
		}

		double distance = glm::distance(o->position, mousePos);

		if (distance< minimalDistance)
		{
			object = o;
			minimalDistance = distance;
		}
	}
	
	if (minimalDistance < 10)
	{
		gameLogic->selectedObject = object;
	}
	else {
		gameLogic->selectedObject = nullptr;
	}
}