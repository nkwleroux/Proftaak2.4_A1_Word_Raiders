#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "ObjModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include "stb_image.h"
#include "Texture.h"
#include "FpsCam.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "CubeComponent.h"
#include "MoveToComponent.h"
#include "SpinComponent.h"
#include "TimerJumper.h"
#include "CrosshairComponent.h"
#include "VisionCamera.h"
#include "WallComponent.h"

using tigl::Vertex;
using namespace std;
using namespace cv;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* camera;
std::vector<ObjModel*> models;
VisionCamera* VC;

Texture* textures[3];
Texture* textureSkybox[6];
//WallComponent* skybox[6];

int windowHeight = 1080;
int windowWidth = 1920;

double lastX, lastY;
int textureIndex;

void init();
void update();
void draw();
void skybox(Texture** textures);
void initSkybox();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	VideoCapture cap(0);
	VC = new VisionCamera(cap);

	thread t1(&VisionCamera::openAction, VC);
	thread t2(&VisionCamera::closedAction, VC);

	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VC->appIsRunning = false;

	t1.join();
	t2.join();

	glfwTerminate();
	destroyAllWindows();

	return 0;
}

std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;
GameObject* crosshair;

void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE || waitKey(100) == 1) { //todo make it so that if esc is pressed on either screens that app closes
				glfwSetWindowShouldClose(window, true);
			}
		});

	textures[0] = new Texture("Images/closeHand.png");
	textures[1] = new Texture("Images/openHand.png");
	textures[2] = new Texture("Images/container.jpg");

	glfwGetCursorPos(window, &lastX, &lastY);

	camera = new FpsCam(window);

	backgroundBox = new GameObject(0);
	backgroundBox->position = glm::vec3(0, 0, 5);
	backgroundBox->addComponent(new CubeComponent(10));
	objects.push_back(backgroundBox);

	crosshair = new GameObject(10);
	crosshair->addComponent(new CrosshairComponent(0.5));
	objects.push_back(crosshair);
	//o->getComponent<CrosshairComponent>()->setTexture(textures[2]); //todo

	for (int i = 1; i < 6; i++) {
		GameObject* o = new GameObject(i);
		o->position = glm::vec3(rand() % 5, 0, -1);
		o->position = glm::vec3(i, 0, -1);
		o->addComponent(new CubeComponent(0.2));
		o->addComponent(new MoveToComponent());
		o->getComponent<MoveToComponent>()->target = o->position;
		//o->addComponent(new SpinComponent(1.0f));
		objects.push_back(o);
	}

	//models.push_back(new ObjModel("resources/Diamond_Word_Raiders.obj"));
	//models.push_back(new ObjModel("resources/scene.obj"));
	//models.push_back(new ObjModel("resources/cube2.obj"));

	//models.push_back(new ObjModel("resources/Cube_Word_Raiders.obj")); //this one

	initSkybox();
}

float rotation = 0;

void update()
{
	VC->update();

	//Dont forget to remove camera update so the user cant move
	camera->update(window, &lastX, &lastY, &textureIndex);

	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	for (auto& o : objects) {
		if (o != backgroundBox && o != crosshair) {
			/*o->position = glm::vec3(o->position.x+deltaTime, o->position.y, o->position.z);
			o->getComponent<MoveToComponent>()->target = o->position;*/
		}
		o->update(deltaTime);
	}

	rotation += 0.01f;
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix()); //camera
	//tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	//glm::mat4 modelMatrix(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3((float)((windowWidth / VC->videoWidth) * VC->currentPoint.x / 120.0f - 8.0), (float)(((windowHeight / VC->videoHeight) * VC->currentPoint.y / -125.0f + 4.0)), 0.0f));
	//tigl::shader->setModelMatrix(modelMatrix);

	glEnable(GL_DEPTH_TEST);
	//for outlines only
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//for (auto& o : objects) {
	//	if (o == backgroundBox) {
	//		textures[2]->bind();
	//		tigl::shader->enableColor(false);
	//		tigl::shader->enableTexture(true);
	//		o->draw();
	//	}
	//	else if (o == crosshair) {
	//		glEnable(GL_BLEND);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//		/*tigl::shader->enableColor(true);
	//		tigl::shader->enableTexture(false);*/

	//		tigl::shader->enableColor(false);
	//		tigl::shader->enableTexture(true);
	//		textures[VC->currentCrosshair]->bind();
	//		o->draw(modelMatrix);

	//		glDisable(GL_BLEND);
	//	}
	//	else {
	//		
	//		tigl::shader->enableColor(true);
	//		tigl::shader->enableTexture(false);
	//		o->draw();
	//	}
	//}
	//
	//tigl::shader->enableTexture(true);
	//tigl::shader->enableLighting(false);

	//for (int i = 0; i < models.size(); i++) {
	//	if (models[i]->materialIndex != -1)
	//	{
	//		tigl::shader->enableColor(false);
	//		tigl::shader->enableTexture(true);
	//	}
	//	else {
	//		tigl::shader->enableColor(true);
	//		tigl::shader->enableTexture(false);
	//	}
	//	models[i]->draw();
	//}
	glLoadIdentity();
	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);
	glEnable(GL_TEXTURE_2D);

	skybox(textureSkybox);

	/*for each (WallComponent var in collection_to_loop)
	{

	}
	skybox[0]->draw*/

	glDisable(GL_DEPTH_TEST);
}

void initSkybox() {
	float x = 0;
	float y = 0;
	float z = 0;
	float width = 50;
	float height = 50;
	float length = 50;

	// Center the skybox
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	textureSkybox[0] = new Texture("Images/skybox_middle.png"); //middle
	//skybox[0] = new WallComponent(glm::vec3(x + width, y, z),
	//	glm::vec3(x + width, y + height, z),
	//	glm::vec3(x, y + height, z),
	//	glm::vec3(x, y, z),
	//	glm::vec2(1, 0),
	//	glm::vec2(1, 1),
	//	glm::vec2(0, 1),
	//	glm::vec2(0, 0),
	//	glm::vec3(0, -1, 0));

	textureSkybox[1] = new Texture("Images/skybox_right.png"); //right
	textureSkybox[2] = new Texture("Images/skybox_left.png"); //left
	textureSkybox[3] = new Texture("Images/skybox_right2.png"); //back
	textureSkybox[4] = new Texture("Images/skybox_bottom.png"); // bottom
	textureSkybox[5] = new Texture("Images/skybox_top.png"); // top
}

void skybox(Texture** texture) {
	float x = 0;
	float y = 0;
	float z = 0;
	float width = 50;
	float height = 50;
	float length = 50;

	// Center the skybox
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	glm::vec3 p(0, 0, 0);

	//middle
	texture[0]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z),				glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z),	glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z),			glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z),						glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::end();

	//right
	texture[1]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z),						glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z + length),			glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length),	glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z),			glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::end();

	//left
	texture[2]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z),			glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z + length),	glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z + length),			glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z),						glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::end();

	//back
	texture[3]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z + length),					glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z + length),			glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length),	glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z + length),			glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::end();

	//bottom
	texture[4]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y, z + length), glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z + length), glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y, z), glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::end();

	//top
	texture[5]->bind();
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z), glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length), glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z + length), glm::vec2(0, 1), glm::vec3(0, -1, 0)));
	tigl::addVertex(Vertex::PTN(p + glm::vec3(x, y + height, z), glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	tigl::end();
}