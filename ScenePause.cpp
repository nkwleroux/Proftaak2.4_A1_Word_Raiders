#include "ScenePause.h"

#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>

extern std::map<Scenes, Scene*> sceneList;
extern Scene* currentScene;
extern GLFWwindow* window;

ScenePause::ScenePause()
{
	// Loading in the texture that should be displayed on this screen
	pauseTexture = new Texture("Images/pause.png");
}

// Draw method for the scene
void ScenePause::draw()
{
	// Setting view, model, and texture. And then passing them to the shader.
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	// Bind texture
	pauseTexture->bind();
	tigl::begin(GL_QUADS);
	// Add vertices
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();
}

void ScenePause::update()
{
	// If S is pressed return to the game
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentScene = sceneList[Scenes::INGAME];
	}
	// If Q is pressed quit the game!
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		currentScene = sceneList[Scenes::STARTUP];
	}
}

// Free textures by unbinding them
void ScenePause::freeTextures()
{
	pauseTexture->unBind();
}
