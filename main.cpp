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
#include "LetterModelComponent.h"
#include "ObjectModelComponent.h"

using tigl::Vertex;
using namespace std;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* camera;

Mat img, imgHSV, mask, imgColor;
int hmin = 45, smin = 110, vmin = 75;
int hmax = 110, smax = 240, vmax = 255;

VideoCapture cap(0);
Point currentPoint;
glm::vec4 currentColor;
glm::vec4 red(1, 0, 0, 1);
glm::vec4 green(0, 1, 0, 1);

int currentCrosshair;
Texture* textures[3];
Texture* openCrosshair;
Texture* closedCrosshair;

bool openHand = true;
bool handDetected = false;
int windowHeight = 1080;
int windowWidth = 1920;

bool appIsRunning = true;

vector<vector<int>> myColors{
	{44, 52, 75, 66, 118, 255}, //green
	//{0, 194, 75, 18, 246, 255} //red
	{hmin, smin, vmin, hmax, smax, vmax} //blue - temp (delete after)
};
vector<Scalar> myColorValues{ {0, 255, 0} };

double lastX, lastY;
int textureIndex;

void init();
void update();
void draw();
void closedAction();
void openAction();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void colorSettings()
{
	VideoCapture cap(0);

	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true)
	{
		cap.read(img);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		cout << "hmin: " << hmin << ", smin: " << smin << ", vmin: " << vmin << endl;
		cout << "hmax: " << hmax << ", smax: " << smax << ", vmax: " << vmax << endl;
		imshow("Image", img);
		imshow("Mask", mask);
		waitKey(1);
	}
}

Point getContours()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Point myPoint(0, 0);
	if (!mask.empty())
	{
		findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		vector<vector<Point>> conPoly(contours.size());
		vector<Rect> boundRect(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);
			//cout << "Area: " << area << endl;

			String objectType;

			if (area > 1000)
			{
				float peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

				boundRect[i] = boundingRect(conPoly[i]);
				myPoint.x = boundRect[i].x + boundRect[i].width / 2;
				myPoint.y = boundRect[i].y + boundRect[i].height / 2;

				drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			}
		}
	}

	return myPoint;
}

void findColor()
{
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);

		Point myPoint = getContours();

		if (myPoint.x != 0 && myPoint.y != 0) {
			handDetected = true;
			if (i == 0) {
				openHand = true;
				currentColor = green;
				currentCrosshair = i;
			}
			else if (i == 1) {
				openHand = false;
				currentColor = red;
				currentCrosshair = i;
			}
			//currentCrosshair = 2;
			circle(img, myPoint, 5, Scalar(255, 255, 0), FILLED);
			currentPoint = myPoint;
		}
		else {
			handDetected = false;
		}
	}
}

//delete after
void display_image()
{

	//Create image - Row, Column, 8bit [signed = -127 to 127, unsigned = 0 - 255] C = Num channels, BGR values.
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

	//Draw a circle in the image - input, center point, size of circle, BGR calue, thickness of border brush/circle (or put FILLED)
	circle(img, Point(256, 256), 155, Scalar(0, 0, 255), FILLED);

	//Draw a rectangle in the image - input, rect(x, y, width, height), BGR values, thickness of border brush/circle (or put FILLED)
	//Can also make from points, replaces rect with 2 points (point 1 = top left, point 2 = bottom right)
	rectangle(img, Point(130, 226), Point(382, 286), Scalar(0, 0, 0), FILLED);

	imshow("Image", img);
	waitKey(1);
}

int main(void)
{
	thread t1(openAction);
	thread t2(closedAction);

	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
GameObject* crosshair;

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
				destroyAllWindows();
				appIsRunning = false;
			}
		});

	textures[0] = new Texture("Images/rainbow.jpg");
	textures[1] = new Texture("Images/container.jpg");
	textures[2] = new Texture("Images/closeHand.png");

	currentCrosshair = 0;

	glfwGetCursorPos(window, &lastX, &lastY);

	camera = new FpsCam(window);

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new CubeComponent(10));
	objects.push_back(backgroundBox);

	crosshair = new GameObject(10);
	crosshair->addComponent(new CrosshairComponent(0.5));
	objects.push_back(crosshair);
	//o->getComponent<CrosshairComponent>()->setTexture(textures[2]); //todo

	for (int i = 1; i < 3; i++) {
		GameObject* o = new GameObject(i);
		o->position = glm::vec3(rand() % 5, 0, -1);
		o->position = glm::vec3(i*3, 0, -1);
		o->addComponent(new MoveToComponent());
		//o->addComponent(new CubeComponent(1.0f));
		o->addComponent(new LetterModelComponent('B'));
		o->getComponent<MoveToComponent>()->target = o->position;
		//o->addComponent(new SpinComponent(1.0f));
		objects.push_back(o);
	}

}

float rotation = 0;

void update()
{
	cap.read(img);
	findColor();
	imshow("Video", img);

	//Dont forget to remove camera update so the user cant move
	camera->update(window, &lastX, &lastY, &textureIndex);

	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	//Test collision detection

	//GameObject* tempObject = new GameObject(0);
	//tempObject->modelMatrix = backgroundBox->modelMatrix;
	//tempObject->position = backgroundBox->position;
	//tempObject->rotation = backgroundBox->rotation;
	//tempObject->scale = backgroundBox->scale;

	//glm::mat4 inverseModelMatrix = glm::inverse(tempObject->modelMatrix);
	//glm::vec4 pointA = glm::vec4(5);

	for (auto& o : objects) {
		//glm::vec4 pointB = o->modelMatrix * (inverseModelMatrix * pointA);
		//if (pointA == pointB) {
		//    cout << "TESTING" << endl;
		//}
		//else {
		//    cout << "NOT TESTING" << endl;
		//}

		if (o != backgroundBox && o != crosshair) {
			/*o->position = glm::vec3(o->position.x+deltaTime, o->position.y, o->position.z);
			o->getComponent<MoveToComponent>()->target = o->position;*/
		}
		o->update(deltaTime);
	}
	rotation += 0.01f;
}



void closedAction()
{
	while (appIsRunning)
	{
		if (handDetected && !openHand) {
			cout << currentPoint.x << "," << currentPoint.y << endl;
		}
		std::this_thread::sleep_for(1000ms);

	}
}

void openAction()
{
	while (appIsRunning)
	{
		if (handDetected && openHand) {
			//cout << "Open hand detected!" << endl;
			cout << currentPoint.x << "," << currentPoint.y << endl;
		}
		std::this_thread::sleep_for(10ms);
	}
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	//tigl::shader->setViewMatrix(camera->getMatrix()); //camera
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	float videoHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	float videoWidth = cap.get(CAP_PROP_FRAME_WIDTH);

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3((float)((windowWidth / videoWidth) * currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / videoHeight) * currentPoint.y / -125.0f + 4.0)), 0.0f));
	tigl::shader->setModelMatrix(modelMatrix);

	tigl::shader->setLightCount(1);
	tigl::shader->setShinyness(100.0f);
	tigl::shader->setLightAmbient(0, glm::vec3(0.5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.5f));
	tigl::shader->setLightSpecular(0,glm::vec3(0.0f));
	tigl::shader->setLightPosition(0, glm::vec3(0, 1, 1));
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->enableLighting(true);


	glEnable(GL_DEPTH_TEST);
	//for outlines only
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto& o : objects) {
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(false);
		if (o == backgroundBox) {
			textures[1]->bind();
			tigl::shader->enableColor(false);
			tigl::shader->enableTexture(true);
			o->draw();
		}
		else if (o == crosshair) {
			//glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			tigl::shader->enableColor(false);
			tigl::shader->enableTexture(true);
			textures[2]->bind();
			o->draw(modelMatrix);

			glDisable(GL_BLEND);
			//glEnable(GL_DEPTH_TEST);
		}
		else {
			o->draw();
		}
	}
	
	glDisable(GL_DEPTH_TEST);
}