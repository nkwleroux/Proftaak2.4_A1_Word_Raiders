#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>
#include "Texture.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using namespace cv;
using namespace std;

GLFWwindow* window;

Mat img, imgHSV, mask, imgColor;
int hmin = 45, smin = 110, vmin = 75;
int hmax = 110, smax = 240, vmax = 255;

VideoCapture cap(0);
Point currentPoint;
glm::vec4 currentColor;
glm::vec4 red(1, 0, 0, 1);
glm::vec4 green(0, 1, 0, 1);

Texture* currentCrosshair;
Texture* openCrosshair;
Texture* closedCrosshair;

bool openHand = true;
bool handDetected = false;
int windowHeight = 1000;
int windowWidth = 1920;

vector<vector<int>> myColors{
	{44, 52, 75, 66, 118, 255}, //green
	{0, 194, 75, 18, 246, 255} //red
};
vector<Scalar> myColorValues{{0, 255, 0}};

void init();
void update();
void draw();
void closedAction();
void openAction();

void colorSettings()
{
	VideoCapture cap(0);
	//Mat img;

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
				currentCrosshair= openCrosshair;
			}
			else if (i == 1) {
				openHand = false;
				currentColor = red;
				currentCrosshair = closedCrosshair;
			}
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
	//delete after
	//colorSettings();
	openCrosshair = new Texture("Resources/openHand.png");
	closedCrosshair = new Texture("Resources/closedHand.png");
	currentCrosshair = openCrosshair;

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


void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);
	});
}


void update()
{
	cap.read(img);
	findColor();
	imshow("video", img);
	waitKey(1);
}

void closedAction()
{
	while (true)
	{
		if (handDetected && !openHand) {
			cout << currentPoint.x << "," << currentPoint.y << endl;
		}
		std::this_thread::sleep_for(1000ms);
	}
}

void openAction()
{
	while (true)
	{
		if (handDetected && openHand) {
			//cout << "Open hand detected!" << endl;
			cout << currentPoint.x << "," << currentPoint.y << endl;
		}
		std::this_thread::sleep_for(10ms);
	}
}

void createRectangle(glm::mat4 modelMatrix) {
	
	
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), (float)(windowWidth / 800), 0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->enableTexture(true);
	currentCrosshair->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), currentColor));
	tigl::addVertex(Vertex::PTC(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0), currentColor));
	tigl::addVertex(Vertex::PTC(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1, 1), currentColor));
	tigl::addVertex(Vertex::PTC(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0, 1), currentColor));
	tigl::end();
	
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3((float)((windowWidth/1280) * currentPoint.x)/75 - 4.0f, ((float)((windowHeight / 650) * currentPoint.y) / 75)* -1.0f + 3.5f, 0.0f));
	tigl::shader->setModelMatrix(modelMatrix);

}
