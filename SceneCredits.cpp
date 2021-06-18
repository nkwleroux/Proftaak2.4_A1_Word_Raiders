#include "SceneCredits.h"
#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>

// Declaring variables
extern std::map<Scenes, Scene*> sceneList;
extern Scene* currentScene;
extern GLFWwindow* window;

SceneCredits::SceneCredits()
{
	// Loading in the image that should be drawn
	creditsTexture = new Texture("Images/credits.png");
}

// Draw method for the scene
void SceneCredits::draw()
{
	// Setting view, model, and texture. And then passing them to the shader.
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	// Bind texture
	creditsTexture->bind();
	tigl::begin(GL_QUADS);
	// Add vertices
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();
}

void SceneCredits::update()
{
	// If backspace is pressed, return to startup scene
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
	{
		currentScene = sceneList[Scenes::STARTUP];
	}
}

// Free textures by unbinding them
void SceneCredits::freeTextures()
{
	creditsTexture->unBind();
}
