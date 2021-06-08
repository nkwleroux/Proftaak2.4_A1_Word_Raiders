#include "SceneSettings.h"
#include "WordLoader.h"

#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/highgui.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>
#include "Text/Text.h"

extern std::map<Scenes, Scene*> scenes;
extern Scene* currentScene;
extern GLFWwindow* window;
Text* wordAmountText;
extern int currentWordLength;
extern int currentWordAmount;

SceneSettings::SceneSettings()
{
	settingsTexture = new Texture("Images/settings.png");
	wordAmountText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || cv::waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
			else if (key == GLFW_KEY_BACKSPACE || cv::waitKey(100) == 1) {
				currentScene = scenes[Scenes::STARTUP];
			}
			else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
				currentWordLength--;
				if (currentWordLength < 5) {
					currentWordLength = 7;
				}	
			}
			else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
				currentWordLength++;
				if (currentWordLength > 7) {
					currentWordLength = 5;
				}
			}
			else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
				
				currentWordAmount--;
				if (currentWordAmount < 1) {
					currentWordAmount = 9;
				}
			}
			else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
				currentWordAmount++;
				if (currentWordAmount > 9) {
					currentWordAmount = 1;
				}
			}
		});
}

std::string SceneSettings::intToString(int number) {
	std::stringstream stream;
	stream << number;
	std::string s = stream.str();
	return s;
}

void SceneSettings::draw()
{
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	settingsTexture->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();

	wordAmountText->draw(intToString(currentWordLength), 1920 / 2 + 210, 1080 / 2 - 125, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	wordAmountText->draw(intToString(currentWordAmount), 1920 / 2 + 210, 1080 / 2 + 95, glm::vec4(0.0f, 0.0f, 0.0f, 0));
}

void SceneSettings::update()
{
	
}

