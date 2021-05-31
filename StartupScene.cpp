#include "StartupScene.h"
#include "Texture.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <map>
#include "imgui.h"

//extern std::map<Scenes, Scene*> scenes;
//extern Scene* currentScene;
extern GLFWwindow* window;

StartupScene::StartupScene()
{
	startupTexture = new Texture("data/startup.png");

	glm::vec4 color(1.0f, 0, 1.0f, 1.0f);

	std::vector<tigl::Vertex> vertices;
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, -1), color, glm::vec3(0, 0, -1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1,  1, -1), color, glm::vec3(0, 0, -1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3( 1,  1, -1), color, glm::vec3(0, 0, -1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3( 1, -1, -1), color, glm::vec3(0, 0, -1)));

	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, 1), color, glm::vec3(0, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, 1), color, glm::vec3(0, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, 1), color, glm::vec3(0, 0, 1)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, 1), color, glm::vec3(0, 0, 1)));

	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, -1), color, glm::vec3(1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, 1), color, glm::vec3(1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, 1), color, glm::vec3(1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, -1), color, glm::vec3(1, 0, 0)));

	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, -1), color, glm::vec3(-1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, 1), color, glm::vec3(-1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, 1), color, glm::vec3(-1, 0, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, -1), color, glm::vec3(-1, 0, 0)));

	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, -1), color, glm::vec3(0, 1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, 1), color, glm::vec3(0, 1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, 1), color, glm::vec3(0, 1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, -1), color, glm::vec3(0, 1, 0)));

	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, -1), color, glm::vec3(0, -1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, 1), color, glm::vec3(0, -1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, 1), color, glm::vec3(0, -1, 0)));
	vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, -1), color, glm::vec3(0, -1, 0)));

	kubusVbo = tigl::createVbo(vertices);
	time = 0;
	menuIndex = 0;
}

void StartupScene::draw()
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


	glEnable(GL_DEPTH_TEST);
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightPosition(0, glm::vec3(1, 1, 1));
	tigl::shader->setLightAmbient(0, glm::vec3(0.2f, 0.2f, 0.2f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));
	tigl::shader->setLightSpecular(0, glm::vec3(0.1f, 0.1f, 0.1f));
	tigl::shader->setProjectionMatrix(glm::ortho(0.0f, 1400.0f, 0.0f, 800.0f, -100.0f, 100.0f));
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(200, 600 - 200 * menuIndex, 0));
	modelMatrix = glm::rotate(modelMatrix, time, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 50));
	tigl::shader->setModelMatrix(modelMatrix);

	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);

	tigl::drawVertices(GL_QUADS, kubusVbo);
	tigl::shader->enableLighting(false);
	glDisable(GL_DEPTH_TEST);


	ImGui::Begin("Startup screen inspector");
	ImGui::InputFloat("Time", &time);
	ImGui::InputInt("Menuoptie", &menuIndex);
	ImGui::BeginChild("Dingen");
	ImGui::LabelText("Hallo!", "Wereld");
	ImGui::EndChild();
	ImGui::End();

}

void StartupScene::onKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		menuIndex = (menuIndex + 1) % 4;
	}
}

void StartupScene::update()
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		//currentScene = scenes[Scenes::INGAME];
	}
	time += 0.01f;
}
