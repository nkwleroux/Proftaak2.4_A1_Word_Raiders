#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include <iomanip>
#include "stb_image.h"
#include "Texture.h"
#include "FpsCam.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "CrosshairComponent.h"
#include <stdlib.h>
#include "LetterModelComponent.h"
#include "ObjectModelComponent.h"
#include "BoundingBox.h"
#include "SkyboxComponent.h"
#include <map>

//Scene inclusions
#include "Scene.h"
#include "SceneStartup.h"
#include "SceneIngame.h"
#include "ScenePause.h"
#include "SceneCredits.h"
#include "SceneSettings.h"

#include "Timer.h"
#include "Text/Text.h"

using tigl::Vertex;
using namespace std;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* camera;

//DO NOT MOVE
//std::vector<ObjModel*> models;
VisionCamera* VC;
Text* textObject;
Texture* textureCrosshair[2];
int ctr = 1;
std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
GameObject* crosshair;
Texture* textureSkybox[6];

int windowHeight = 1080;
int windowWidth = 1920;
double lastX, lastY;
Timer* timer;
Timer* oneSecondTimer;
WordLoader* wordLoader;

int currentWordLength = 5;
int currentWordAmount = 1;
DIFFICULTY currentDifficulty = easy;
int currentWordIndex = 0;
int chosenWordsAmount = 0;
std::vector<Word*> wordsToGuess;
std::vector<char> correctLetters(currentWordLength);
Word* currentWord;
String wordShot = "";

std::map<Scenes, Scene*> scenes;
Scene* currentScene = nullptr;
//DO NOT MOVE

void init();
void update();
void draw();
void checkWord();
void duringGame();
void initSkyboxTextures();
void createLetterCubes();


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(windowWidth, windowHeight, "Word Raiders", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	tigl::init();

	init();
	//thread t2(&VisionCamera::update, VC);
	thread t3(&FpsCam::update, camera, window);

	while (!glfwWindowShouldClose(window))
	{
		draw();
		update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	VC->appIsRunning = false;
	camera->appIsRunning = false;

	t3.join();

	glfwTerminate();
	destroyAllWindows();

	return 0;
}

GameObject* square;
GameObject* square2;

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
		});
	

	textureCrosshair[0] = new Texture("Images/closeHand.png");
	textureCrosshair[1] = new Texture("Images/openHand.png");
	textObject = new Text("c:/windows/fonts/times.ttf", 64.0);

	wordLoader = new WordLoader();
	wordsToGuess = wordLoader->loadWords(currentWordLength, currentDifficulty);
	currentWord = wordsToGuess.at(chosenWordsAmount);
	textObject->draw(wordShot, windowWidth / 2 - 100 + ctr, 50.0f + ctr, glm::vec4(0.1f, 0.8f, 0.1f, 0));

	timer = new Timer(90);
	timer->start();

	oneSecondTimer = new Timer(1);
	oneSecondTimer->start();

	camera = new FpsCam(window);

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	camera->lastX = x;
	camera->lastY = y;

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	//backgroundBox->addComponent(new CubeComponent(10));
	backgroundBox->addComponent(new SkyboxComponent(50, textureSkybox));
	backgroundBox->addComponent(new BoundingBox(backgroundBox));

	/*crosshair = new GameObject(10);
	crosshair->addComponent(new CrosshairComponent(0.5));
	objects.push_back(crosshair);*/
	//o->getComponent<CrosshairComponent>()->setTexture(textures[2]); //todo

	//for (int i = 1; i < 20; i++) {
	//	GameObject* o = new GameObject(i);
	//	o->position = RandomVec3(i);
	//	//o->position = glm::vec3(rand() % 5, 0, -1);
	//	//o->position = glm::vec3(i*3, 0, -1);
	//	o->addComponent(new MoveToComponent());
	//	o->addComponent(new CubeComponent(0.2f));
	//	//o->addComponent(new LetterModelComponent('B'));
	//	o->getComponent<MoveToComponent>()->target = RandomVec3(7);
	//	o->addComponent(new BoundingBox(o));
	//	//o->addComponent(new SpinComponent(1.0f));
	//	objects.push_back(o);
	//}

	//models.push_back(new ObjModel("resources/Diamond_Word_Raiders.obj"));
	//models.push_back(new ObjModel("resources/scene.obj"));
	//models.push_back(new ObjModel("resources/cube2.obj"));

	//models.push_back(new ObjModel("resources/Cube_Word_Raiders.obj")); //this one

	scenes[Scenes::STARTUP] = new SceneStartup();
	scenes[Scenes::INGAME] = new SceneIngame();
	scenes[Scenes::PAUSE] = new ScenePause();
	scenes[Scenes::SETTINGS] = new SceneSettings();
	scenes[Scenes::CREDITS] = new SceneCredits();
	currentScene = scenes[Scenes::STARTUP];




	/*square = new GameObject(1);
	square->position = glm::vec3(-3, 1, 0);
	square->addComponent(new MoveToComponent());
	square->getComponent<MoveToComponent>()->target = glm::vec3(-25, 0, 0);
	square->addComponent(new CubeComponent(1.0f));
	square->addComponent(new BoundingBox(square));*/
	//square->addComponent(new SpinComponent(0.5));
	//objects.push_back(square);

	//GameObject* square2 = new GameObject(2);
	//square2->position = glm::vec3(-3, 1, 0);
	//square2->addComponent(new MoveToComponent());
	//square2->getComponent<MoveToComponent>()->target = glm::vec3(-10, 0, 0);
	//square2->addComponent(new CubeComponent(1.0f));
	//square2->addComponent(new BoundingBox(square2));

	initSkyboxTextures();
	createLetterCubes();
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

void update()
{
	currentScene->update();
	VC->update();
	duringGame();
	//Dont forget to remove camera update so the user cant move
	//camera->update(window);

	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y);

	int* axis;
	for (auto& o : objects) {
		for (auto& next : objects) {

			//Skip first element so you can compare with the previous one
			if (next != o) {
				if (o->getComponent<BoundingBox>()->collideWithObject(next)) {

					//BoundingBox* nextBox = next->getComponent<BoundingBox>();
					BoundingBox* oBox = o->getComponent<BoundingBox>();

					//glm::vec3 differenceVec = glm::vec3(next->position.x - o->position.x, next->position.y - o->position.y, next->position.z - o->position.z);
					//cout << "difference: " << differenceVec.x << "\t" << differenceVec.y << "\t" << differenceVec.z << "\n";
					//glm::vec3 dimensions = (oBox->max - oBox->min);
					//dimensions = glm::vec3(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);
					//cout << "dimensions: " << dimensions.x << "\t" << dimensions.y << "\t" << dimensions.z << "\n";
					// 
					//cout <<"dimensions" << dimensions.x << "\t" << dimensions.y << "\t" << dimensions.z << "\n";
					//o->position -= differenceVec - dimensions;
					//cout << "pos: " << o->position.x << "\t" << o->position.y << "\t" << o->position.z << "\n";
					//o->position -= differenceVec;

					//cout << next->id << " collide with " << o->id << "\n";

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

					//cout << "pos before: " << o->position.x << "\t" << o->position.y << "\t" << o->position.z << "\n";

					o->position -= temp / 8.0f;

					//cout << "pos after: " << o->position.x << "\t" << o->position.y << "\t" << o->position.z << "\n";

					glm::vec3 oTarget = (o->getComponent<MoveToComponent>()->target);
					//cout << oTarget.x << ", " << oTarget.y << ", " << oTarget.z << "\n";
					oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
					oTarget += RandomVec3(30, oBox->collisionX, oBox->collisionY, oBox->collisionZ);

					//next->getComponent<MoveToComponent>()->target = currTarget;
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
			cout << oTarget.x << ", " << oTarget.y << ", " << oTarget.z << "\n";
			oTarget = glm::vec3(-1 * oTarget.x, -1 * oTarget.y, -1 * oTarget.z);
			o->getComponent<MoveToComponent>()->target = oTarget;
		}
		if (o->getComponent<MoveToComponent>()->target == o->position) {
			o->getComponent<MoveToComponent>()->target = RandomVec3(25,true, true, false) + glm::vec3(-1 * o->position.x, -1 * o->position.y, -1 * o->position.z);;
		}
		o->update(deltaTime);
	}
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->draw();
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	//glm::mat4 modelMatrix(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3((float)((windowWidth / VC->videoWidth) * VC->currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / VC->videoHeight) * VC->currentPoint.y / -125.0f + 4.0)), 0.0f));
	//tigl::shader->setModelMatrix(modelMatrix);

	glEnable(GL_DEPTH_TEST);
	//for outlines only
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	backgroundBox->draw();

	for (auto& o : objects) {
		tigl::shader->enableColor(true);
		tigl::shader->enableTexture(false);
		if (o == crosshair) {
			//glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//		/*tigl::shader->enableColor(true);
			//		tigl::shader->enableTexture(false);*/

			//		tigl::shader->enableColor(false);
			//		tigl::shader->enableTexture(true);
			//		textures[VC->currentCrosshair]->bind();
			//		o->draw(modelMatrix);

			glDisable(GL_BLEND);
		}
		else {

			tigl::shader->enableColor(true);
			tigl::shader->enableTexture(false);
			o->draw();
		}
	}

	//timer
	textObject->draw("Score: 200 stars  ", 50.0 + ctr, 50.0 + ctr, glm::vec4(0.1f, 0.8f, 0.1f, 0));
	textObject->draw(timer->secondsToGoString(), 50.0 + ctr, 100 + ctr, glm::vec4(0.1f, 0.8f, 0.1f, 0));
	textObject->draw("Levens: ******", 50.0 + ctr, 150 + ctr, glm::vec4(0.1f, 0.8f, 0.1f, 0));
	//ctr++;
	textObject->draw(wordShot, windowWidth / 2 - 100 + ctr, 50.0f + ctr, glm::vec4(0.1f, 0.8f, 0.1f, 0));

	glDisable(GL_DEPTH_TEST);
}

void rayCast(int xOrigin, int yOrigin)
{
	//RAY ray = { glm::vec3(xOrigin, yOrigin, 0), glm::vec3(250, 250, 300)};
	// glm::vec3 position;
	// glm::vec3 normal;
	// glm::vec4 color;
	// glm::vec2 texcoord;
	//
	//DRAWING LINES
	glLineWidth(12.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(xOrigin, yOrigin, 0);
	glVertex3f(400, 400, 400);
	glEnd();
}

void clearVector() {
	for (int i = 0; i < correctLetters.size(); i++) {
		correctLetters.at(i) = '.';
	}
}

void showWord() {
	cout << "Letters found: ";
	for (int i = 0; i < correctLetters.size(); i++) {
		cout << correctLetters.at(i);
	}
	cout << endl;
}

void checkWord() {
	int correctLettersAmount = 0;
	for (int i = 0; i < currentWordLength; i++) {
		if (currentWord->getWord()[i] == wordShot[i]) {
			//Ct << currentWord->getWord()[i];
			correctLetters.at(i) = currentWord->getWord()[i];
			correctLettersAmount++;
		}
		else {
			if (!(correctLetters.at(i) >= 65 && correctLetters.at(i <= 90))) {
				correctLetters.at(i) = '.';
			}
		}
	}

	if (correctLettersAmount == currentWordLength) {
		chosenWordsAmount++;
		if (chosenWordsAmount < currentDifficulty) {
			currentWord = wordsToGuess.at(chosenWordsAmount);
			currentWordIndex = 0;
			wordShot = "";
			clearVector();
		}
	}
	else {
		showWord();
	}
}

void duringGame() {

	if (VC->redDetected) {
		if (oneSecondTimer->hasFinished()) {
			oneSecondTimer->start();
			VC->redDetected = false;
			if (chosenWordsAmount <= currentDifficulty) {
				if (currentWordIndex < currentWordLength) {
					wordShot += currentWord->getWord()[currentWordIndex];
					currentWordIndex++;
					cout << wordShot << endl;

				}
				else {
					checkWord();
				}
			}
			else {
				cout << "No words left!" << endl;
			}
		}
	}
}

void initSkyboxTextures() {
	textureSkybox[0] = new Texture("Images/skybox_middle.png"); //middle
	textureSkybox[1] = new Texture("Images/skybox_right.png"); //right
	textureSkybox[2] = new Texture("Images/skybox_left.png"); //left
	textureSkybox[3] = new Texture("Images/skybox_right2.png"); //back
	textureSkybox[4] = new Texture("Images/skybox_bottom.png"); // bottom
	textureSkybox[5] = new Texture("Images/skybox_top.png"); // top
}

void createLetterCubes()
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