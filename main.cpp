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

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using namespace cv;
using namespace std;

GLFWwindow* window;
FpsCam* camera;

glm::vec3 position = glm::vec3(0, 0, 0);
glm::vec2 rotation = glm::vec2(0, 0);
Texture* textures[5];
double lastX, lastY;
int textureIndex;

void init();
void update();
void draw();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::vector<tigl::Vertex> create_cube(int size, Texture* texture);

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

    textures[0] = new Texture("rainbow.jpg");
    textures[1] = new Texture("container.jpg");

    glfwGetCursorPos(window, &lastX, &lastY);

    camera = new FpsCam(window);
}

void update()
{
    camera->update(window, &lastX, &lastY, &textureIndex);
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

    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    const int size = 5;

    std::vector<tigl::Vertex> vertices = create_cube(size,textures[textureIndex]);

    tigl::drawVertices(GL_QUADS, vertices);

    glDisable(GL_DEPTH_TEST);
    
}

bool CheckCollision(double cameraX, double cameraY, double cameraZ, double wallX, double wallY, double wallZ) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = cameraX + 1 >= wallX &&
        wallX + 1 >= cameraX;
    // collision y-axis?
 /*   bool collisionY = cameraY + 1 >= wallY &&
        wallY + 1 >= cameraY;*/
    bool collisionZ = cameraY + 1 >= wallY &&
        wallY + 1 >= cameraY;
    // collision only if on both axes
    //return collisionX && collisionY;
    return collisionX && collisionZ;
}

std::vector<tigl::Vertex> create_cube(int size, Texture* texture){
    texture->bind();

    std::vector<tigl::Vertex> vertices;

    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, -size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, -size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, -size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, -size), glm::vec2(0, 0)));

    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, size), glm::vec2(0, 0)));

    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, -size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, -size), glm::vec2(0, 0)));

    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, -size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, -size), glm::vec2(0, 0)));

    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, -size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, size, size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, size, -size), glm::vec2(0, 0)));

    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, -size), glm::vec2(0, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, size), glm::vec2(1, 1)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, size), glm::vec2(1, 0)));
    vertices.push_back(tigl::Vertex::PT(glm::vec3(size, -size, -size), glm::vec2(0, 0)));

    return vertices;
}