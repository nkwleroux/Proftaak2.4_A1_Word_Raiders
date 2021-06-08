#include "SceneSettings.h"

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
int wordAmount = 5;
int wordLength = 5;

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
				wordAmount--;
				if (wordAmount < 1) {
					wordAmount = 9;
				}
				
			}
			else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
				wordAmount++;
				if (wordAmount > 9) {
					wordAmount = 1;
				}
			}
			else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
				wordLength--;
				if (wordLength < 5) {
					wordLength = 7;
				}
			}
			else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
				wordLength++;
				if (wordLength > 7) {
					wordLength = 5;
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

	wordAmountText->draw(intToString(wordAmount), 1920 / 2 + 210, 1080 / 2 - 125, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	wordAmountText->draw(intToString(wordLength), 1920 / 2 + 210, 1080 / 2 + 95, glm::vec4(0.0f, 0.0f, 0.0f, 0));
}

void SceneSettings::update()
{
	
}

