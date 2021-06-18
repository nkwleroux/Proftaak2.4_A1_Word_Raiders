#include "SceneStartup.h"
#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>
#include "imgui.h"

extern std::map<Scenes, Scene*> sceneList;
extern Scene* currentScene;
extern GLFWwindow* window;

SceneStartup::SceneStartup()
{
	// Load in the texture
	startupTexture = new Texture("Images/startup.png");
}

// Draw method for the scene
void SceneStartup::draw()
{
	// Enable textures, set matrices, and bind
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	startupTexture->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();
}

// Free textures by unbinding them
void SceneStartup::freeTextures()
{
	startupTexture->unBind();
}

// Update function for the scene class
void SceneStartup::update()
{
	// Callback for key presses
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// Switch case to perform action when a certain type of key is pressed
			switch (key) {
			// Escape closes the screen
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;
			case GLFW_KEY_S:
				// S starts the game
				if (action == GLFW_PRESS)
				{
					currentScene = sceneList[Scenes::INGAME];
				}
				break;
			case GLFW_KEY_D:
				// D opens the settings menu
				if (action == GLFW_PRESS)
				{
					currentScene = sceneList[Scenes::SETTINGS];
				}
				break;
			case GLFW_KEY_C:
				// C opens the credits menu
				if (action == GLFW_PRESS)
				{
					currentScene = sceneList[Scenes::CREDITS];
				}
				break;
			}
		});
}
