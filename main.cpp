#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
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

//delete after
void display_image() {

    //Create image - Row, Column, 8bit [signed = -127 to 127, unsigned = 0 - 255] C = Num channels, BGR values.
    Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

    //Draw a circle in the image - input, center point, size of circle, BGR calue, thickness of border brush/circle (or put FILLED)
    circle(img, Point(256, 256), 155, Scalar(0, 0, 255), FILLED);

    //Draw a rectangle in the image - input, rect(x, y, width, height), BGR values, thickness of border brush/circle (or put FILLED)
    //Can also make from points, replaces rect with 2 points (point 1 = top left, point 2 = bottom right)
    rectangle(img, Point(130, 226), Point(382, 286), Scalar(0, 0, 0), FILLED);
    
    imshow("Image", img); 
    waitKey(1);
}

int main(void)
{
    //delete after
    display_image();

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

}


void update()
{

}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}