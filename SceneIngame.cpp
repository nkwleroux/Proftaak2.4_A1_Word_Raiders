#include "SceneIngame.h"
#include "Scene.h"
#include "VisionCamera.h"
#include "Texture.h"
#include "tigl.h"
#include "stb_image.h"
#include "Text/Text.h"
#include "GameObject.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "Timer.h"
#include "Word.h"
#include "WordLoader.h"
#include "BoundingBoxComponent.h"
#include "SkyboxComponent.h"
#include "LetterModelComponent.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <iomanip>
#include <map>
#include "Crosshair.h"
#include "GameLogic.h"
#include <unordered_map>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

// Using to make things a bit easier to use
using tigl::Vertex;
using namespace std;
using namespace cv;


SceneIngame::SceneIngame()
{
	// creating the videocapture object
	VideoCapture videoCapture(0);

	// creating visioncamera and gamelogic
	VC = new VisionCamera(videoCapture);
	gameLogic = new GameLogic();


	// Creating a textobject which we can use to draw text
	textObject = new Text("c:/windows/fonts/Verdana.ttf", 64.0f);


	// initlaize skybox textures. we load these by path
	textureSkybox[0] = new Texture("Images/skybox_middle.png"); //middle
	textureSkybox[1] = new Texture("Images/skybox_right.png"); //right
	textureSkybox[2] = new Texture("Images/skybox_left.png"); //left
	textureSkybox[3] = new Texture("Images/skybox_right2.png"); //back
	textureSkybox[4] = new Texture("Images/skybox_bottom.png"); // bottom
	textureSkybox[5] = new Texture("Images/skybox_top.png"); // top


	// Creating a backgroundbox/skybox
	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new SkyboxComponent(50, textureSkybox));
	backgroundBox->addComponent(new BoundingBoxComponent(backgroundBox));

	// Creating the crosshair
	crosshair = new Crosshair();

	// Creating an array of chars with letters we want to be  available
	char chars[] = { 'A','B','C' ,'D' ,'E' ,'F' ,'G' ,'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W' ,'X' ,'Y' ,'Z' };

	// Creating the letter block texture
	Texture* texture = new Texture("resources/LetterBlockTexture.png");

	// For each letter, map the letter
	for (int i = 0; i < sizeof(chars); i++) {
		lettersMap[chars[i]] = new LetterModelComponent(chars[i], texture);
	}

	lastFrameTime = 0;
}

// Draw method for the scene
void SceneIngame::draw()
{
	// Create viewport
	int viewport[4];
	//glGet � return the value or values of a selected parameter
	glGetIntegerv(GL_VIEWPORT, viewport);
	// create projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	// set that same projectionmatrix
	tigl::shader->setProjectionMatrix(projection);
	// create view matrix
	glm::mat4 viewmatrix = glm::lookAt(glm::vec3(0, 0, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// set that same view matrix
	tigl::shader->setViewMatrix(viewmatrix);
	// create model matrix
	glm::mat4 modelmatrix(1.0f);
	// set that same model matrix
	modelmatrix = glm::translate(modelmatrix, glm::vec3((float)((windowWidth / VC->videoWidth) * VC->currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / VC->videoHeight) * VC->currentPoint.y / -125.0f + 4.0)), 0.0f));
	// passtrough the modelmatrix to the shader
	tigl::shader->setModelMatrix(modelmatrix);

	// Set the light in the scene
	tigl::shader->setLightCount(1);
	tigl::shader->setShinyness(1.0f);
	tigl::shader->setLightPosition(0, glm::vec3(0,0,1));
	tigl::shader->setLightDirectional(0, true);
	
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// uncomment if we want to see outlines
	//glpolygonmode(gl_front_and_back, gl_line);

	// drawing the objects
	for (auto& o : dynamicObjectsList) {
		o->draw();
	}

	// Calculate where the crosshair would hit
	//rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y, viewmatrix, projection);

	// Debug use mouse as pointer
	// You can uncomment this and the mouse is our pointer
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

	// Use the shader to enable lightning
	tigl::shader->enableLighting(false);
	skyBox->draw();

	// Draw the backgroundbox
	backgroundBox->draw();

	// 2D objects drawing
	glDisable(GL_DEPTH_TEST);

	//timer functions
	// below we draw the score, lives, and words
	textObject->draw("Score: " + gameLogic->getCurrentScore(), 50.0, 50.0, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getGameTimer()->timeRemainingToString(), 50.0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw("Levens: " + gameLogic->getCurrentLifes(), 50.0, 150, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getShotWord(), windowWidth / 2 - 100, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(gameLogic->getCorrectWord(), windowWidth - 300, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));

	// drawing the crosshair
	crosshair->draw();
}

// Method to generate a random vector, to which the boxes then can bounce
glm::vec3 RandomVec3(float max, bool xCollide, bool yCollide, bool zCollide) {
	// Creating variables for x y and z
	float x = 0, y = 0, z = 0;

	// Using int instead of boolean, so we can randomize
	int isNegative;

	// if x Collides
	if (xCollide) {
		// generate a random x value (for the bounce)
		x = (float(rand()) / float((RAND_MAX)) * max);
		isNegative = (float(rand()) / float((RAND_MAX)) * 2);
		if (isNegative == 1) {
			x *= -1;
		}
	}
	// if y collides
	if (yCollide) {
		// generate a random y value (for the bounce)
		y = (float(rand()) / float((RAND_MAX)) * max);
		isNegative = (float(rand()) / float((RAND_MAX)) * 2);
		if (isNegative == 1) {
			y *= -1;
		}
	}

	if (zCollide) {
		z = (float(rand()) / float((RAND_MAX)) * max);

	// return the random vector, z should always be 0 because we dont need to check that
	return glm::vec3(x, y, 0);
}

void SceneIngame::update() {
	// Set callback for keys
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// If escape is pressed, the game should quit
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
			// If P is pressed, the game should be paused
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

	// If we reset the game logic, boolean should change accordingly
	// Also create new cubes
	if (gameLogic->reset)
	{
		gameLogic->reset = false;
		createLetterCubes();
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

	// If something has been shot
	if (gameLogic->selectedObject != nullptr && gameLogic->selectedObject->getComponent<LetterModelComponent>()->hasBeenShot) {
		dynamicObjectsList.remove(gameLogic->selectedObject);
		gameLogic->selectedObject->getComponent<LetterModelComponent>()->hasBeenShot = false;
		gameLogic->selectedObject = nullptr;
	}

	// If the correct word has been guessed
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
	for (auto& object : dynamicObjectsList) {
		glm::vec3 objectTarget = (object->getComponent<MoveToComponent>()->targetLocation);

		for (auto& nextObject : dynamicObjectsList) {

			//Skip first element so you can compare with the previous one
			// Comparing each element with eachother
			if (nextObject != object) {
				// Compare boundingboxes to eachother
				if (object->getComponent<BoundingBoxComponent>() && nextObject->getComponent<BoundingBoxComponent>() && object->getComponent<BoundingBoxComponent>()->collideWithObject(nextObject)) {
					//create objectbox, retreive the boundingbox
					BoundingBoxComponent* objectBox = object->getComponent<BoundingBoxComponent>();

					// create a temporary vector
					glm::vec3 tempVector = glm::vec3(0);

					// if there is collision on X
					if (objectBox->collisionX) {
						// add values from nextobject
						tempVector += glm::vec3(nextObject->position.x - object->position.x, 0, 0);
					}
					// if there is collision on Y
					if (objectBox->collisionY) {
						// add values from nextobject
						tempVector += glm::vec3(0, nextObject->position.y - object->position.y, 0);
					}
					// if there is collision on X
					if (objectBox->collisionX) {
						// add values from nextobject
						tempVector += glm::vec3(0, 0, nextObject->position.z - object->position.z);
					}

					// change object position accordingly
					object->position -= tempVector / 8.0f;

					// vector to keep track of the difference
					glm::vec3 difference = glm::vec3(objectTarget.x - object->position.x, objectTarget.y - object->position.y, objectTarget.z - object->position.z);

					// create variables to store the target x and y
					int objectTargetX = objectTarget.x;
					int objectTargetY = objectTarget.y;

					// if the x difference is higher or equal than 0
					if (difference.x >= 0) {
						// multiply to negative
						objectTargetX *= -1;
					}

					// if the y difference is higher or equal than 0
					if (difference.y >= 0) {
						// multiply to negative
						objectTargetY *= -1;
					}

					// store vector with object target data
					objectTarget = glm::vec3(objectTargetX, objectTargetY, objectTarget.z);

					// move component, with the data we retreived from the vector
					object->getComponent<MoveToComponent>()->targetLocation = objectTarget;

					// set collision on false again
					objectBox->collisionX = false;
					objectBox->collisionZ = false;
					objectBox->collisionY = false;
					break;

				}
			}
		}

		//Have to fix, Blocks are flying out of the skybox
		// Check collisionw ith the wall lastly
		if (backgroundBox != nullptr && backgroundBox->getComponent<BoundingBoxComponent>()->collideWithWall(object)) {
			// if there is collision with the wall, move to other location
			glm::vec3 oTarget = (object->getComponent<MoveToComponent>()->targetLocation);
			oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
			object->getComponent<MoveToComponent>()->targetLocation = oTarget;
		}

		// calculate the difference again, store in difference vector
		glm::vec3 difference = glm::vec3(objectTarget.x - object->position.x, objectTarget.y - object->position.y, objectTarget.z - object->position.z);
		if (difference.x <= 1.0f && difference.y <= 1.0f) {
			object->getComponent<MoveToComponent>()->targetLocation = RandomVec3(25, true, true, false);
		}

		// lastly update the object again, with the difference in time
		object->update(deltaTime);
	}
}

// free textures by unbinding
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

// method to create all the letter cubes we can shoot
void SceneIngame::createLetterCubes()
{
	// clear object list
	dynamicObjectsList.clear();
	fullObjectsList.clear();

	// create cubes based on the current word
	for (int i = 0; i < gameLogic->getCurrentWord()->getLetters().size(); i++) {
		// create new gameobject
		GameObject* gameObject = new GameObject(i);
		// add the correct letters from the word to the box. mapping it as textures.
		gameObject->addComponent(lettersMap[gameLogic->getCurrentWord()->getLetters()[i]]);
		// adding boundingbox
		gameObject->addComponent(new BoundingBoxComponent(gameObject));
		// adding moving component
		gameObject->addComponent(new MoveToComponent());
		// storing original position
		glm::vec3 originalPosition = glm::vec3(0, 0, 0); //original position
		// move to original position
		gameObject->position = originalPosition;
		// scale to normal size
		gameObject->scale = glm::vec3(1.0f);
		gameObject->getComponent<MoveToComponent>()->targetLocation = originalPosition;
		gameObject->draw();

		// move blocks if they spawn in each other
		for (auto& next : dynamicObjectsList) {
			if (next != gameObject) {
				while (gameObject->getComponent<BoundingBoxComponent>()->collideWithObject(next)) {
					glm::vec3 pos = RandomVec3(25, true, true, false);
					gameObject->position = pos;
					gameObject->getComponent<MoveToComponent>()->targetLocation = RandomVec3(30, true, true, false);
					gameObject->draw();
				}
			}
		}

		// push game obejcts to dynamic and object list
		dynamicObjectsList.push_back(gameObject);
		fullObjectsList.push_back(gameObject);
	}
}

// method that uses raycasting to select an object
void SceneIngame::selectObject() {
	// storing minimal distance as a value
	double minimalDistance = 0;
	// creating a temporary game object
	GameObject* object = nullptr;
	// storing the mouse position
	glm::vec3 mousePos(mouse3d.x, mouse3d.y, mouse3d.z);

	// for each object in the dynamic object list
	for (const auto& o : dynamicObjectsList) {
		// if the distance equals 0
		if (minimalDistance == 0)
		{
			// get distance
			minimalDistance = glm::distance(o->position, mousePos);
			// store object in new variable
			object = o;
			continue;
		}

		// get distance
		double distance = glm::distance(o->position, mousePos);

		// if distance is smaller then minimal distance
		if (distance < minimalDistance)
		{
			object = o;
			minimalDistance = distance;
		}
	}

	// if minimal distance is lower then 10, set the selectedobject
	if (minimalDistance < 10)
	{
		gameLogic->selectedObject = object;
	}
	// else there is no selected object
	else {
		gameLogic->selectedObject = nullptr;
	}
}