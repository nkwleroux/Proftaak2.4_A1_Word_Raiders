#include "Crosshair.h"
#include "tigl.h"
#include <glm/ext/matrix_transform.hpp>
#include "Texture.h"
#include <glm\ext\matrix_clip_space.hpp>

tigl::VBO* vbo;
glm::vec2 position;

/// <summary>
/// Constructor for the crosshair
/// </summary>
Crosshair::Crosshair() {
	// Bind the crosshair textures
	closedHand = new Texture("Images/closedHand.png");
	openHand = new Texture("Images/openHand.png");

	// Initialise crosshair as open
	handIsOpen = true;

	// Create the vbo used for drawing
	std::vector<tigl::Vertex> vertices;
	vertices.push_back(tigl::Vertex::PT(glm::vec3(-0.5, -0.5, 0), glm::vec2(0, 0)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(-0.5, 0.5, 0), glm::vec2(0, 1)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(0.5, 0.5, 0), glm::vec2(1, 1)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(0.5, -0.5, 0), glm::vec2(1, 0)));
	vbo = tigl::createVbo(vertices);
}

/// <summary>
/// Mathod to draw the crosshair
/// </summary>
void Crosshair::draw() {
	// Set the projection matrix to be on the drawing plane
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	tigl::shader->setProjectionMatrix(glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f, -100.0f, 100.0f));

	// Set view matrix as standart matrix
	tigl::shader->setViewMatrix(glm::mat4(1.0f));

	// Set the position of the crosshair
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix,glm::vec3(position.x,position.y,0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 1));
	tigl::shader->setModelMatrix(modelMatrix);

	// Enable the shaders
	tigl::shader->enableColor(false);
	tigl::shader->enableLighting(false);
	tigl::shader->enableTexture(true);
	tigl::shader->enableColorMult(false);

	// Enable blend for the opque part of the crosshair
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable depth testing so the part behind it get drawn too
	glDisable(GL_DEPTH_TEST);

	// Bind the correct texture
	if (handIsOpen)
	{
		openHand->bind();
	}
	else {
		closedHand->bind();
	}

	// Draw the vbo
	tigl::drawVertices(GL_QUADS, vbo);
}

/// <summary>
/// Mathod to set the crosshair texture
/// </summary>
/// <param name="isOpen">boolean to set the texture to openhand</param>
void Crosshair::setHandStyle(bool isOpen) {
	handIsOpen = isOpen;
}

/// <summary>
/// Method to set the position of the crosshair
/// </summary>
/// <param name="crosshairX">Value of the x axis</param>
/// <param name="crosshairY">Value of the y axis</param>
void Crosshair::update(double crosshairX, double crosshairY) {
	position.x = crosshairX;
	position.y = crosshairY;
}