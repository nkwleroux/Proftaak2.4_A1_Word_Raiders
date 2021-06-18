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
#include "VisionCamera.h"

using tigl::Vertex;
using namespace std;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//DO NOT MOVE
int windowHeight = 1080;
int windowWidth = 1920;

std::map<Scenes, Scene*> scenes;
Scene* currentScene = nullptr;
GLFWwindow* window;
VisionCamera* VC;

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

	for (auto const& x : scenes)
	{
		x.second->freeTextures();
	}

	return 0;
}

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		});

	VideoCapture cap(0);
	VC = new VisionCamera(cap);

	SceneIngame* sceneInGame = new SceneIngame();
	SceneSettings* sceneSettings = new SceneSettings();

	scenes[Scenes::STARTUP] = new SceneStartup();
	scenes[Scenes::INGAME] = sceneInGame;
	sceneInGame->VC = VC;
	scenes[Scenes::PAUSE] = new ScenePause();
	scenes[Scenes::SETTINGS] = sceneSettings;
	scenes[Scenes::CREDITS] = new SceneCredits();
	scenes[Scenes::GAMEEND] = new SceneEnding();
	currentScene = scenes[Scenes::STARTUP];
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