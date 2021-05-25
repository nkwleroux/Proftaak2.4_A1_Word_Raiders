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

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using namespace cv;
using namespace std;

GLFWwindow* window;

void init();
void update();
void draw();

glm::vec3 position = glm::vec3(0, 0, 0);
glm::vec2 rotation = glm::vec2(0, 0);
Texture* backgroundTexture;
//tigl::VBO* kubusVbo;
//double t;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::mat4 getMatrix();
void move(float angle, float fac);

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

void init()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    //to hide mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    backgroundTexture = new Texture("rainbow.jpg");

 /*   glm::vec4 color(0, 1.0f, 0, 1.0f);

    std::vector<tigl::Vertex> vertices;
    vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, -1, -1), color, glm::vec3(0, 0, -1)));
    vertices.push_back(tigl::Vertex::PCN(glm::vec3(-1, 1, -1), color, glm::vec3(0, 0, -1)));
    vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, 1, -1), color, glm::vec3(0, 0, -1)));
    vertices.push_back(tigl::Vertex::PCN(glm::vec3(1, -1, -1), color, glm::vec3(0, 0, -1)));

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

    t = 0; */
}

void update()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    static double lastX = x;
    static double lastY = y;

    rotation.x -= (float)(lastY - y) / 100.0f;
    rotation.y -= (float)(lastX - x) / 100.0f;

    lastX = x;
    lastY = y;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        move(0, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        move(180, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        move(90, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        move(-90, 0.05f);
    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    //    t++;
    //}

}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);

    backgroundTexture->bind();

    int size = 50;

    glEnable(GL_DEPTH_TEST);
    
    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(-size, size, -size), glm::vec2(0, 1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(size, size, -size), glm::vec2(1, 1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(size, -size, -size), glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(-size, -size, -size), glm::vec2(0, 0)));
    tigl::end();

    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(-size, size, size), glm::vec2(0, 1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(size, size, size), glm::vec2(1, 1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(size, -size, size), glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(-size, -size, size), glm::vec2(0, 0)));
    tigl::end();

    glDisable(GL_DEPTH_TEST);


    ////tigl::shader->enableLighting(true);
    ////tigl::shader->setLightCount(1);
    ////tigl::shader->setLightDirectional(0, true);
    ////tigl::shader->setLightPosition(0, glm::vec3(1, 1, 1));
    ////tigl::shader->setLightAmbient(0, glm::vec3(0.2f, 0.2f, 0.2f));
    ////tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));
    ////tigl::shader->setLightSpecular(0, glm::vec3(0.1f, 0.1f, 0.1f));
    //tigl::shader->setProjectionMatrix(glm::ortho(0.0f, 1400.0f, 0.0f, 800.0f, -100.0f, 100.0f));
    //glm::mat4 modelMatrix(1.0f);
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(200, 600 - 200 * t, 0));
    ////modelMatrix = glm::rotate(modelMatrix, t.f, glm::vec3(1, 0, 0));
    ////modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(150, 150, 150));
    //tigl::shader->setModelMatrix(modelMatrix);

    //tigl::shader->enableTexture(true);
    //tigl::shader->enableColor(true);
    //backgroundTexture->bind();

    /*tigl::drawVertices(GL_QUADS, kubusVbo);*/
    //tigl::shader->enableLighting(false);
    
}

glm::mat4 getMatrix()
{
    glm::mat4 ret(1.0f);
    ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
    ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
    ret = glm::translate(ret, position);
    return ret;
}

void move(float angle, float fac)
{
    position.x += (float)cos(rotation.y + glm::radians(angle)) * fac;
    position.z += (float)sin(rotation.y + glm::radians(angle)) * fac;
}