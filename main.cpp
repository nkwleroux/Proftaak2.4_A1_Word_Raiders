#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "ObjModel.h"
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
int windowHeight = 1080;
int windowWidth = 1920;
int currentWordLength = 5;
int currentWordAmount = 1;

std::map<Scenes, Scene*> scenes;
Scene* currentScene = nullptr;
//DO NOT MOVE

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
	if (!window){
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