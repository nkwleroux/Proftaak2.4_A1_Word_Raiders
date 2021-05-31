#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "ObjModel.h"
#include <glm/gtc/matrix_transform.hpp>

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
std::vector<ObjModel*> models;

void init();
void update();
void draw();

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


    //models.push_back(new ObjModel("resources/Diamond_Word_Raiders.obj"));
    models.push_back(new ObjModel("resources/Cube_Word_Raiders.obj"));
    //models.push_back(new ObjModel("resources/scene.obj"));
    //models.push_back(new ObjModel("resources/cube2.obj"));
}

float rotation = 0;

void update()
{
    rotation += 0.01f;
}

void draw()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 500.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0,5.0f,10.0f), glm::vec3(0,0,0), glm::vec3(0,1,0)));
    tigl::shader->setModelMatrix(glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0,1,0)));

    
    //tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);
    tigl::shader->setLightAmbient(0, glm::vec3(0.5f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.5f));
    tigl::shader->setLightPosition(0, glm::vec3(0, 1, 1));
    tigl::shader->setLightDirectional(0, true);

    tigl::shader->enableTexture(true);

    glEnable(GL_DEPTH_TEST);

    for (auto model : models) {
        if (model->hasTexture())
        {
            tigl::shader->enableColor(false);
            tigl::shader->enableTexture(true);
        }
        else {
            tigl::shader->enableColor(true);
            tigl::shader->enableTexture(false);
        }
        model->draw();
    }
}