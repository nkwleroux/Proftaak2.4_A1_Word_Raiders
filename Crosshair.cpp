#include "Crosshair.h"
#include "tigl.h"
#include <glm/ext/matrix_transform.hpp>
#include "Texture.h"

tigl::VBO *vbo;
glm::vec2 position;

Crosshair::Crosshair(){
	closedHand = new Texture("Images/closedHand.png");
	openHand = new Texture("Images/openHand.png");
	handIsOpen = true;

	std::vector<tigl::Vertex> vertices;
	vertices.push_back(tigl::Vertex::PT(glm::vec3(-0.5,-0.5,0),glm::vec2(0,0)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(-0.5,0.5,0),glm::vec2(0,1)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(0.5,0.5,0),glm::vec2(1,1)));
	vertices.push_back(tigl::Vertex::PT(glm::vec3(0.5,-0.5,0),glm::vec2(1,0)));

	vbo = tigl::createVbo(vertices);
}

void Crosshair::draw() {
	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);

	if (handIsOpen)
	{
		openHand->bind();
	}
	else {
		closedHand->bind();
	}

	glm::mat4 modelMatrix;

	modelMatrix = glm::translate(modelMatrix,glm::vec3(position,0));

	tigl::shader->setModelMatrix(modelMatrix);

	tigl::drawVertices(GL_TRIANGLES, vbo);
}

void Crosshair::setHandStyle(bool isOpen) {
	handIsOpen = isOpen;
}

void Crosshair::update(double crosshairX, double crosshairY) {
	position.x = crosshairX;
	position.y = crosshairY;
}