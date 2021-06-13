#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <thread>
#include <filesystem>
#include <iomanip>
#include <stdlib.h>
#include <map>

//Scene inclusions
#include "Scene.h"
#include "SceneStartup.h"
#include "SceneIngame.h"
#include "ScenePause.h"
#include "SceneCredits.h"
#include "SceneSettings.h"
#include "SceneEnding.h"

using tigl::Vertex;
using namespace std;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//DO NOT MOVE
int windowHeight = 1080;
int windowWidth = 1920;
int currentWordLength = 5;
int currentWordAmount = 1;
float timeSpent = 1;
int achievedScore = 1;
bool wonGame = false;

std::map<Scenes, Scene*> scenes;
Scene* currentScene = nullptr;
GLFWwindow* window;

void init();
void update();
void draw();

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

	while (!glfwWindowShouldClose(window))
	{
		draw();
		update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	destroyAllWindows();

	return 0;
}

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
		});

	scenes[Scenes::STARTUP] = new SceneStartup();
	scenes[Scenes::INGAME] = new SceneIngame();
	scenes[Scenes::PAUSE] = new ScenePause();
	scenes[Scenes::SETTINGS] = new SceneSettings();
	scenes[Scenes::CREDITS] = new SceneCredits();
	scenes[Scenes::GAMEEND] = new SceneEnding();
	currentScene = scenes[Scenes::GAMEEND];
}

void update()
{
	currentScene->update();
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->draw();
}



