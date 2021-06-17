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
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();
}

void SceneStartup::freeTextures()
{
	startupTexture->unBind();
}

void SceneStartup::update()
{

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;
			case GLFW_KEY_S:
				if (action == GLFW_PRESS)
				{
					currentScene = scenes[Scenes::INGAME];
				}
				break;
			case GLFW_KEY_D:
				if (action == GLFW_PRESS)
				{
					currentScene = scenes[Scenes::SETTINGS];
				}
				break;
			case GLFW_KEY_C:
				if (action == GLFW_PRESS)
				{
					currentScene = scenes[Scenes::CREDITS];
				}
				break;
			}
		});
}
