#include "SceneIngame.h"
#include "Scene.h"
#include "VisionCamera.h"
#include "Texture.h"
#include "tigl.h"
#include "stb_image.h"
#include "tigl.h"
#include "ObjModel.h"
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

Texture* textures[3];
std::vector<ObjModel*> models;
extern int windowHeight;
extern int windowWidth;
VisionCamera* VC;
Text* textObject;
Text* wordText;
int ctr = 1;
std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
GameObject* crosshair;

double lastX, lastY;
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

	inGameTexture = new Texture("Images/shapes.png");
	
	textures[0] = new Texture("Images/openHand.png");
	textures[1] = new Texture("Images/closedHand.png");
	textures[2] = new Texture("Images/container.jpg");

	textObject = new Text("c:/windows/fonts/Verdana.ttf", 64.0f);
	wordText = new Text("c:/windows/fonts/Verdana.ttf", 128.0f);

	wordLoader = new WordLoader();
	wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
	currentWord = wordsToGuess.at(chosenWordsAmount);

	timer = new Timer(90);
	oneSecondTimer = new Timer(1);

	glfwGetCursorPos(window, &lastX, &lastY);

	//camera = new FpsCam(window);

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new CubeComponent(10));
	objects.push_back(backgroundBox);

	crosshair = new GameObject(10);
	crosshair->addComponent(new CrosshairComponent(0.5));
	objects.push_back(crosshair);

	VC->appIsRunning = false;

	//o->getComponent<CrosshairComponent>()->setTexture(textures[2]); //todo

	for (int i = 1; i < 6; i++) {
		GameObject* o = new GameObject(i);
		o->position = glm::vec3(rand() % 5, 0, -1);
		o->position = glm::vec3(i, 0, -1);
		o->addComponent(new CubeComponent(0.2));
		o->addComponent(new MoveToComponent());
		o->getComponent<MoveToComponent>()->target = o->position;
		//o->addComponent(new SpinComponent(1.0f));
		objects.push_back(o);
	}
}

void SceneIngame::draw()
{
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	inGameTexture->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	//tigl::shader->setviewmatrix(camera->getmatrix()); //camera
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	//tigl::shader->setmodelmatrix(glm::mat4(1.0f));

	glm::mat4 modelmatrix(1.0f);
	//modelmatrix = glm::translate(modelmatrix, glm::vec3((float)((windowwidth / videowidth) * currentpoint.x / 120.0f - 8.0), (float)(((windowheight / videoheight) * currentpoint.y / -125.0f + 4.0)), 0.0f));
	modelmatrix = glm::translate(modelmatrix, glm::vec3((float)((windowWidth / VC->videoWidth) * VC->currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / VC->videoHeight) * VC->currentPoint.y / -125.0f + 4.0)), 0.0f));
	tigl::shader->setModelMatrix(modelmatrix);

	glEnable(GL_DEPTH_TEST);
	//for outlines only
	//glpolygonmode(gl_front_and_back, gl_line);
	//
	//

	//drawing text
	for (auto& o : objects) {
		if (o == backgroundBox) {
			textures[2]->bind();
			tigl::shader->enableColor(false);
			tigl::shader->enableTexture(true);
			o->draw();
		}
		else if (o == crosshair) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			/*tigl::shader->enablecolor(true);
			tigl::shader->enabletexture(false);*/

			tigl::shader->enableColor(false);
			tigl::shader->enableTexture(true);
			textures[VC->currentCrosshair]->bind();
			o->draw(modelmatrix);

			glDisable(GL_BLEND);
		}
		else {

			tigl::shader->enableColor(true);
			tigl::shader->enableTexture(false);
			o->draw();
		}
	}

	tigl::shader->enableTexture(true);
	tigl::shader->enableLighting(false);

	for (int i = 0; i < models.size(); i++) {
		if (models[i]->materialIndex != -1)
		{
			tigl::shader->enableColor(false);
			tigl::shader->enableTexture(true);
		}
		else {
			tigl::shader->enableTexture(false);
		}
		models[i]->draw();
	}
	
	//timer
	textObject->draw("score: 200 stars  ", 50.0 + ctr, 50.0, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(timer->secondsToGoString(), 50.0, 100, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw("levens: ******", 50.0 + ctr, 150, glm::vec4(1.0f, 1.0f, 1.0f, 0));	
	textObject->draw(shotWord, windowWidth / 2 - 100, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));
	textObject->draw(correctWord, windowWidth - 300, 100.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0));

	glDisable(GL_DEPTH_TEST);
}

void SceneIngame::update(){
	//check if wordlength has changed
	if (currentWord->getWordLength() != currentWordLength) {
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		currentWord = wordsToGuess.at(chosenWordsAmount);
		correctLetters.resize(currentWordLength);
		shotLetters.resize(currentWordLength);
	}

	//check if it is the start of the game
	if (!gameStarted){
		gameStarted = true;
		wordsToGuess = wordLoader->loadWords(currentWordLength, currentWordAmount);
		currentWord = wordsToGuess.at(chosenWordsAmount);
		timer->start();
		oneSecondTimer->start();
	}

	

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		currentScene = scenes[Scenes::PAUSE];
	}

		//cap.read(img);
	//findColor();
	//imshow("Video", img);

	VC->update();
	gameLogic();
	drawShootedWords();
	//Dont forget to remove camera update so the user cant move
	//camera->update(window, &lastX, &lastY, &textureIndex);

	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	rayCast(VC->getCrossHairCoords().x, VC->getCrossHairCoords().y);

	for (auto& o : objects) {
		if (o != backgroundBox && o != crosshair) {
			/*o->position = glm::vec3(o->position.x+deltaTime, o->position.y, o->position.z);
			o->getComponent<MoveToComponent>()->target = o->position;*/
		}
		o->update(deltaTime);
	}

	//rotation += 0.01f;
}


void SceneIngame::rayCast(int xOrigin, int yOrigin)
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

/*
* This function
*/
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
