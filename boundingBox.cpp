#include "BoundingBox.h"

std::vector<Vertex> BoundingBox::makeBoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
	glm::vec4 color((float)(rand() % 100 + 1) / 100, (float)(rand() % 100 + 1) / 100, (float)(rand() % 100 + 1) / 100, 1);
	std::vector<Vertex> verts;
	//bottom
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMin), color, glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMin), color, glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMax), color, glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMax), color, glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMin), color, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMin), color, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMax), color, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMax), color, glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMin), color, glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMin), color, glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMax), color, glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMax), color, glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMin), color, glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMin), color, glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMax), color, glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMax), color, glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMin), color, glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMin), color, glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMin), color, glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMin), color, glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMin, zMax), color, glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMin, yMax, zMax), color, glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMax, zMax), color, glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	verts.push_back(Vertex::PCTN(glm::vec3(xMax, yMin, zMax), color, glm::vec2(0, 1), glm::vec3(0, 0, 1)));

	tigl::drawVertices(GL_QUADS, verts);
}