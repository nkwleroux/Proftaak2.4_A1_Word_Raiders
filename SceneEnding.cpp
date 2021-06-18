#include "SceneEnding.h"
#include "WordLoader.h"
#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/highgui.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>
#include "Text/Text.h"

// Create list of scenes, currentscene , and window variable
extern std::map<Scenes, Scene*> sceneList;
extern Scene* currentScene;
extern GLFWwindow* window;

// Text that should be shown when "game over" variables
Text* finishedText;
Text* timeOverText;
Text* scoreText;

extern float timeSpent;
extern int achievedScore;
extern bool wonGame;


// Constructor
SceneEnding::SceneEnding()
{
	// texture the screen should show
	endingTexture = new Texture("Images/ending.png");
	// Loading text object which we can use to draw text
	finishedText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);
	timeOverText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);
	scoreText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);
}

// method that converts integer to string
std::string SceneEnding::intToString(int number) {
	std::stringstream stream;
	stream << number;
	std::string s = stream.str();
	return s;
}

// Draw method for the scene
void SceneEnding::draw()
{
	// Setting view, model, and texture. And then passing them to the shader.
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	// Bind texture
	endingTexture->bind();
	tigl::begin(GL_QUADS);
	// Add vertices

	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();

	// Draw the text on the screen
	finishedText->draw("You " + getWonText(wonGame), 1920 / 2 - 140, 1080 / 2, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	timeOverText->draw("Time over: " + intToString(timeSpent), 1920 / 2 - 140, 1080 / 2 + 95, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	scoreText->draw("Score: " + intToString(achievedScore), 1920 / 2 - 140, 1080 / 2 + 190, glm::vec4(0.0f, 0.0f, 0.0f, 0));
}

// Getter to retreive the won text
std::string SceneEnding::getWonText(bool won){
	if (won){
		return "Won!";
	}
	else{
		return "Lost!";
	}
}


// Update method for the scene
void SceneEnding::update(){
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
		// if escape is pressed, ending scene should close
			if (key == GLFW_KEY_ESCAPE || cv::waitKey(100) == 1) {
				glfwSetWindowShouldClose(window, true);
			}
		// if Q is pressed, we should show the startup scene again
			if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
				currentScene = sceneList[Scenes::STARTUP];
			}
		});
}

// free textures by unbinding them
void SceneEnding::freeTextures()
{
	endingTexture->unBind();
}