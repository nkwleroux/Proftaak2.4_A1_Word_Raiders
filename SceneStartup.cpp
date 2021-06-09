#include "SceneStartup.h"
#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>
#include "imgui.h"

extern std::map<Scenes, Scene*> scenes;
extern Scene* currentScene;
extern GLFWwindow* window;

SceneStartup::SceneStartup()
{
	startupTexture = new Texture("Images/startup.png");
}

void SceneStartup::draw()
{
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	startupTexture->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3( 1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1,-1, 0), glm::vec2(0, 0)));
	tigl::end();
}

void SceneStartup::onKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		
	}
}

void SceneStartup::update()
{
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentScene = scenes[Scenes::INGAME];
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		currentScene = scenes[Scenes::SETTINGS];
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
	{
		currentScene = scenes[Scenes::CREDITS];
	}
}
