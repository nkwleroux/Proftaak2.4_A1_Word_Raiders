#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>
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
#include <limits>
#include "BoundingBox.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using namespace cv;
using namespace std;

GLFWwindow* window;
FpsCam* camera;

Texture* textures[5];
double lastX, lastY;
int textureIndex;

void init();
void update();
void draw();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
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

	glfwTerminate();

    return 0;
}

std::list<GameObject*> objects;
double lastFrameTime = 0;
GameObject* backgroundBox;

void init()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    //textures[0] = new Texture("rainbow.jpg");
    textures[1] = new Texture("container.jpg");

    glfwGetCursorPos(window, &lastX, &lastY);

    camera = new FpsCam(window);

    backgroundBox = new GameObject(0);
    backgroundBox->position = glm::vec3(0, 0, 5);
    backgroundBox->addComponent(new CubeComponent(10));
    objects.push_back(backgroundBox);

    float xMin, xMax, yMin, yMax, zMin, zMax;

    xMin = yMin = zMin = numeric_limits<float>::max();
    xMax = yMax = xMax = numeric_limits<float>::min();
    std::vector<Vertex> verts;


    for (int i = 1; i < 6; i++) {
        GameObject* o = new GameObject(i);
        o->position = glm::vec3(rand() % 5, 0, -1);
        o->addComponent(new CubeComponent(0.2));
        o->addComponent(new MoveToComponent());
        o->getComponent<MoveToComponent>()->target = o->position;
        o->addComponent(new SpinComponent(1.0f));
        verts = o->verts = o->getComponent<CubeComponent>()->getVerts();


        objects.push_back(o);

        for (auto  &vert : o->verts) {
            if (vert.position.x < xMin) {
                xMin = vert.position.x;
            }
            else if (vert.position.x > xMax) {
                xMax = vert.position.x;
            }
            else if (vert.position.y < xMin) {
                yMin = vert.position.y;
            }
            else if (vert.position.y > yMax) {
                yMax = vert.position.y;
            }
            else if (vert.position.z < zMin) {
                zMin = vert.position.z;
            }
            else if (vert.position.z > zMax) {
                zMax = vert.position.z;
            }
        }

        
    }
}

void update()
{
    //Dont forget to remove camera update so the user cant move
    camera->update(window, &lastX, &lastY, &textureIndex);

    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    GameObject* tempObject = new GameObject(0);
    tempObject->modelMatrix = backgroundBox->modelMatrix;
    tempObject->position = backgroundBox->position;
    tempObject->rotation = backgroundBox->rotation;
    tempObject->scale = backgroundBox->scale;

    glm::mat4 inverseModelMatrix = glm::inverse(tempObject->modelMatrix);
    glm::vec4 pointA = glm::vec4(5);
    
    for (auto& o : objects) {
        glm::vec4 pointB = o->modelMatrix * (inverseModelMatrix * pointA);
        if (pointA == pointB) {
            cout << "TESTING" << endl;
        }
        else {
            cout << "NOT TESTING" << endl;
        }



        if (o != backgroundBox) {
            o->position = glm::vec3(o->position.x+deltaTime, o->position.y, o->position.z);
            o->getComponent<MoveToComponent>()->target = o->position;
        }
        o->update(deltaTime);
    }

    
}

void draw()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    glEnable(GL_DEPTH_TEST);
    //for outlines only
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (auto& o : objects) {
        if (o == backgroundBox) {
            tigl::shader->enableColor(false);
            tigl::shader->enableTexture(true);
        }
        else {
            tigl::shader->enableColor(true);
            tigl::shader->enableTexture(false);
        }
        o->draw();

    }       

    glDisable(GL_DEPTH_TEST);
  
}

