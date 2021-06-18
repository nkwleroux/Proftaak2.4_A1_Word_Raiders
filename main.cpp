#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <iostream>
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


// Using namespace to make things a  bit easier
using namespace std;
using namespace cv;
using tigl::Vertex;

//Include libraries
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

//Setting the height and width of the game window
int windowHeight = 1080;
int windowWidth = 1920;

// Create a list of sceneList
std::map<Scenes, Scene*> sceneList;
// Currentscene starts as nothing
Scene* currentScene = nullptr;
// Creating the 3D window
GLFWwindow* window;
VisionCamera* VC;

// Declaring methods, init update and draw
void init();
void update();
void draw();

// Method to set the glViewPort
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	// If check for when glfw cant be initialized
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(windowWidth, windowHeight, "Word Raiders", NULL, NULL);
	// If check for when glfw cant be initialized
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}

	// Set glfw to window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Call the init function from the tigl class
	tigl::init();

	// Calling the init from this class
	init();

	// While the glfwindow isnt closed, call draw updat poll etc.
	while (!glfwWindowShouldClose(window))
	{
		draw();
		update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// If it should be closed, terminate everything
	glfwTerminate();
	destroyAllWindows();

	// Then remove all the textures.
	for (auto const& x : sceneList)
	{
		x.second->freeTextures();
	}

	return 0;
}

void init()
{
	// Set callback for keys
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
		});

	VideoCapture cap(0);
	VC = new VisionCamera(cap);

	// Create all the scenes and add them to the scenelist
	SceneIngame* sceneInGame = new SceneIngame();
	SceneSettings* sceneSettings = new SceneSettings();

	sceneList[Scenes::STARTUP] = new SceneStartup();
	sceneList[Scenes::INGAME] = sceneInGame;
	sceneList[Scenes::PAUSE] = new ScenePause();
	sceneList[Scenes::SETTINGS] = sceneSettings;
	sceneList[Scenes::CREDITS] = new SceneCredits();
	sceneList[Scenes::GAMEEND] = new SceneEnding();
	currentScene = sceneList[Scenes::STARTUP];

	sceneInGame->VC = VC;
	sceneSettings->VC = VC;
}

// Update the current scene, starts as startup scene
void update()
{
	currentScene->update();
}

// Draw function in which we draw the current scene
void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->draw();
}