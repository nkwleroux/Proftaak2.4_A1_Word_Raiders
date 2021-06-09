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
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "CrosshairComponent.h"
#include "Timer.h"
#include "Word.h"
#include "WordLoader.h"
#include "BoundingBox.h"
#include "SkyboxComponent.h"
#include "LetterModelComponent.h"
#include "FpsCam.h"

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

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using tigl::Vertex;
using namespace std;
using namespace cv;

extern std::map<Scenes, Scene*> scenes;
extern Scene* currentScene;
extern GLFWwindow* window;

extern int currentWordLength;
extern int currentWordAmount;

Texture* textureSkybox[6];
Texture* textureCrosshair[2];
extern int windowHeight;
extern int windowWidth;
extern FpsCam* camera;
VisionCamera* VC;
Text* textObject;
Text* wordText;
std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
Crosshair* crosshair;

int textureIndex;
Timer* timer;
Timer* oneSecondTimer;
WordLoader* wordLoader;

int currentWordIndex = -1;
int chosenWordsAmount = 0;
bool gameStarted = false;
float rotation = 0;
std::vector<Word*> wordsToGuess;
std::vector<char> correctLetters(currentWordLength);
Word* currentWord;
String shotWord = "";
String correctWord = "";
std::vector<char> shotLetters(currentWordLength);

SceneIngame::SceneIngame()
{
	VideoCapture cap(0);
	VC = new VisionCamera(cap);

	initSkyboxTextures();

	textObject = new Text("c:/windows/fonts/Verdana.ttf", 64.0f);
	wordText = new Text("c:/windows/fonts/Verdana.ttf", 128.0f);

	wordLoader = new WordLoader();
	wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
	currentWord = wordsToGuess.at(chosenWordsAmount);

	timer = new Timer(90);
	oneSecondTimer = new Timer(1);

	camera = new FpsCam(window);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	camera->lastX = x;
	camera->lastY = y;

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new SkyboxComponent(50, textureSkybox));
	backgroundBox->addComponent(new BoundingBox(backgroundBox));

	crosshair = new Crosshair();
	createLetterCubes();

	debugCube = new GameObject(1337);
	debugCube->addComponent(new CubeComponent(1.0f));
	
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

	//drawing text
	for (auto& o : objects) {
		tigl::shader->enableColor(true);
		tigl::shader->enableTexture(false);
		o->draw();
	}

	rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y, viewmatrix, projection);

	debugCube->draw();
	backgroundBox->draw();

	// 2D objects drawing
	tigl::shader->enableTexture(true);
	tigl::shader->enableLighting(false);

	//timer
	textObject->draw("score: 200 stars  ", 50.0, 50.0, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(timer->secondsToGoString(), 50.0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw("levens: ******", 50.0, 150, glm::vec4(1.0f, 1.0f, 1.0f, 0));	
	textObject->draw(shotWord, windowWidth / 2 - 100, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(correctWord, windowWidth - 300, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));

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
	//check if wordlength has changed
	if (currentWord->getWordLength() != currentWordLength) {
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		currentWord = wordsToGuess.at(chosenWordsAmount);
		correctLetters.resize(currentWordLength);
		shotLetters.resize(currentWordLength);
	}

	//check if it is the start of the game
	if (!gameStarted) {
		gameStarted = true;
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		currentWord = wordsToGuess.at(chosenWordsAmount);
		timer->start();
		oneSecondTimer->start();
	}

	//Maybe make into callback function. Now is dependent on update (timing).
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		currentScene = scenes[Scenes::PAUSE];
	}

	VC->update();
	gameLogic();
	drawShootedWords();
	//Dont forget to remove camera update so the user cant move
	//camera->update(window, &lastX, &lastY, &textureIndex);

	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	crosshair->update(VC->getCrossHairCoords());

	debugCube->position = mouse3d;

	int* axis;
	for (auto& o : objects) {
		for (auto& next : objects) {

			//Skip first element so you can compare with the previous one
			if (next != o) {
				if (o->getComponent<BoundingBox>() && next->getComponent<BoundingBox>() &&o->getComponent<BoundingBox>()->collideWithObject(next)) {

					BoundingBox* oBox = o->getComponent<BoundingBox>();

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
		if (backgroundBox != nullptr && backgroundBox->getComponent<BoundingBox>()->collideWithWall(o)) {
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


void SceneIngame::rayCast(int xOrigin, int yOrigin, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	// If no mouseposition is given we don't cast a ray
	if (xOrigin == 0&&yOrigin == 0)
	{
		return;
	}

	int Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	
	yOrigin = Viewport[3] - yOrigin;

	float winZ;
	glReadPixels(xOrigin, yOrigin, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	mouse3d = glm::vec4(glm::unProject(glm::vec3(xOrigin,yOrigin, winZ), viewMatrix, projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3])), winZ);
}

void SceneIngame::clearVector(std::vector<char> *vector) {
	for (int i = 0; i < vector->size(); i++) {
		vector->at(i) = '_';
	}
}

/*
* This function draws the shot word and the correct letters
*/
void SceneIngame::drawShootedWords() {
	shotWord = ""; //clear the shotWord string
	for (int i = 0; i < shotLetters.size(); i++) {
		shotWord += shotLetters.at(i); //fill the string with the letters of the vector
	}

	correctWord = ""; //clear the correctWord string
	for (int i = 0; i < correctLetters.size(); i++) {
		correctWord += correctLetters.at(i); //fill the string with the letters of the vector
	}
}

/*
* This function fills 2 vectors with letters or an _
*/
void SceneIngame::fillVector() {
	for (int i = 0; i < correctLetters.size(); i++) {
		shotLetters.at(i) = '_'; //fill the shotLetter vector with _
		if (i == 0) {
			correctLetters.at(i) = currentWord->getFirstLetter(); //fill the vector with the first letter of the current word
		}
		else {
			correctLetters.at(i) = '_'; //fill the other indexes with an _
		}
	}
	drawShootedWords(); //draw both words
}

bool SceneIngame::checkWord() {
	int correctLettersAmount = 0;
	for (int i = 0; i < currentWordLength; i++) {
		if (currentWord->getWord()[i] == shotLetters.at(i)) { 
			correctLetters.at(i) = currentWord->getWord()[i];
			correctLettersAmount++;
		}
	}

	if (correctLettersAmount == currentWordLength) {
		clearVector(&correctLetters);
		currentWordIndex = -1;
		return true;
	}
	else {
		return false;
	}
}

void SceneIngame::gameLogic() {
	char letter;
	if (currentWordIndex == -1) {
		fillVector();
		currentWordIndex = 0;
		cout << currentWord->getWord();
		return;
	}

	//TODO --> check for lives
	//TODO --> check for timer
	//TODO --> check for color detection
	if (VC->redDetected) {
		if (oneSecondTimer->hasFinished()) {
			oneSecondTimer->start();
			VC->redDetected = false;

			if (chosenWordsAmount < currentWordAmount) {

				if (currentWordIndex < currentWordLength) {
					//tESTCODE
					//cin >> letter;
					letter = currentWord->getWord()[currentWordIndex];
					shotWord += letter;
					shotLetters.at(currentWordIndex) = letter;
					currentWordIndex++;
				}
				else {
					if (checkWord()) {
						chosenWordsAmount++;
						if (chosenWordsAmount < currentWordAmount) {
							currentWord = wordsToGuess.at(chosenWordsAmount);
						}
					}
					else {
						currentWordIndex = 0;
						shotWord = "";
						clearVector(&shotLetters);
					}
				}
			}
			else {
				chosenWordsAmount = 0;
				currentWordIndex = -1;
				gameStarted = false;
				destroyAllWindows();
				currentScene = scenes[Scenes::STARTUP];
			}
		}
	}

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
	for (int i = 0; i < currentWord->getLetters().size(); i++) {
		GameObject* o = new GameObject(i);

		o->addComponent(new LetterModelComponent(currentWord->getLetters().at(i)));
		o->addComponent(new BoundingBox(o));
		o->addComponent(new MoveToComponent());
		//o->getComponent<MoveToComponent>()->target = glm::vec3(rand() % 20, rand() % 20, 0);
		glm::vec3 pos = glm::vec3(0, 0, 0);
		o->position = pos;
		o->scale = glm::vec3(1.0f);
		o->getComponent<MoveToComponent>()->target = pos;
		o->draw();

		for (auto& next : objects) {
			if (next != o) {
				cout << o->getComponent<BoundingBox>()->collideWithObject(next) << endl;
				while (o->getComponent<BoundingBox>()->collideWithObject(next)) {
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