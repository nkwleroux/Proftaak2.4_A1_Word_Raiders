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

extern std::map<Scenes, Scene*> scenes;
extern Scene* currentScene;
extern GLFWwindow* window;
Text* finishedText;
Text* timeUsedText;
Text* scoreText;
extern float timeSpent;
extern int achievedScore;
extern bool wonGame;

SceneEnding::SceneEnding()
{
	endingTexture = new Texture("Images/ending.png");
	finishedText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);
	timeUsedText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);
	scoreText = new Text("c:/windows/fonts/Verdana.ttf", 64.0);

	//timeSpent = timespent;
	//achievedScore = score;

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || cv::waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
			else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
				currentScene = scenes[Scenes::STARTUP];
			}
		});
}

std::string SceneEnding::intToString(int number) {
	std::stringstream stream;
	stream << number;
	std::string s = stream.str();
	return s;
}

void SceneEnding::draw()
{
	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f));
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	tigl::shader->enableTexture(true);
	endingTexture->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
	tigl::end();

	finishedText->draw("You " + getWonText(wonGame), 1920 / 2 - 140, 1080 / 2, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	timeUsedText->draw("Time Used: " + intToString(timeSpent), 1920 / 2 - 140, 1080 / 2 + 95, glm::vec4(0.0f, 0.0f, 0.0f, 0));
	scoreText->draw("Score: " + intToString(achievedScore), 1920 / 2 - 140, 1080 / 2 + 190, glm::vec4(0.0f, 0.0f, 0.0f, 0));
}

std::string SceneEnding::getWonText(bool won){
	if (won){
		return "Won!";
	}
	else{
		return "Lost!";
	}
}



void SceneEnding::update(){

}
void SceneEnding::freeTextures()
{

	endingTexture->unBind();
}
//
//void SceneEnding::updateScore()
//{
//
//}
//
//void SceneEnding::updateTime()
//{
//
//}