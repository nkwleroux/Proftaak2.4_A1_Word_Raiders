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

// Declaring variables
extern std::map<Scenes, Scene*> sceneList;
extern Scene* currentScene;
extern GLFWwindow* window;
// Storing variables about the current word
extern int currentWordLength;
extern int currentWordAmount;
Text* wordAmountText;

void SceneSettings::changeColor(int index)
{
	VC->colorSettings(&VC->myColors.at(index));
}

// Constructor
SceneSettings::SceneSettings()
{
	// Set variables in the constructor
	settingsTexture = new Texture("Images/settings.png");
	wordAmountText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
	
}

// Method to convert integer to string type
std::string SceneSettings::intToString(int number) {
	std::stringstream stream;
	stream << number;
	std::string s = stream.str();
	return s;
}

// Draw method for the scene
void SceneSettings::draw()
{
	// Setting view, model, and texture. And then passing them to the shader.
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	// Bind texture
	settingsTexture->bind();
	tigl::begin(GL_QUADS);
	// Add vertices
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();

	// Draw the current wordt length and amount of words.
	wordAmountText->draw(intToString(currentWordLength), 1920 / 2 + 210, 1080 / 2 - 125, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	wordAmountText->draw(intToString(currentWordAmount), 1920 / 2 + 210, 1080 / 2 + 95, glm::vec4(0.0f, 0.0f, 0.0f, 0));
}

// Update function for the settings screen
void SceneSettings::update()
{
	// Callback for when a key is pressed
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// Escape closes the application
			SceneSettings* handler = reinterpret_cast<SceneSettings*>(glfwGetWindowUserPointer(window));
			if (key == GLFW_KEY_ESCAPE) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
			// Backspace returns to the startup screen
			else if (key == GLFW_KEY_BACKSPACE) {
				currentScene = sceneList[Scenes::STARTUP];
			}
			// D allows you to decrease the word length
			else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
				currentWordLength--;
				if (currentWordLength < 5) {
					currentWordLength = 7;
				}
			}
			// G allows you to increase the word length
			else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
				currentWordLength++;
				if (currentWordLength > 7) {
					currentWordLength = 5;
				}
			}

			// C allows you to decrease the amount of words
			else if (key == GLFW_KEY_C && action == GLFW_PRESS) {

				currentWordAmount--;
				if (currentWordAmount < 1) {
					currentWordAmount = 9;
				}
			}
			// C allows you to increase the amount of words
			else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
				currentWordAmount++;
				if (currentWordAmount > 9) {
					currentWordAmount = 1;
				}
			}
			else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
				handler->changeColor(0);
			}
			else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
				handler->changeColor(1);
			}
		});

}


// Free textures by unbinding them
void SceneSettings::freeTextures()
{
	settingsTexture->unBind();
}


