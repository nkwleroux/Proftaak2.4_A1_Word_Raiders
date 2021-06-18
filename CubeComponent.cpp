#include "CubeComponent.h"
#include <iostream>

CubeComponent::CubeComponent(float size)
{
	//Setting size of the cube
	glm::vec3 sizeCube(size);

	//Setting color of the cube
	glm::vec4 colorCube((float)(rand() % 100 + 1) / 100, (float)(rand() % 100 + 1) / 100, (float)(rand() % 100 + 1) / 100, 1);

	glm::vec3 positionCube(0, 0, 0);


	// By adding the below vertices we are creating a cube

	//bottom
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(0, -1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(0, -1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(0, -1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(0, -1, 0)));

	//top
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(0, 1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(0, 1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(0, 1, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(0, 1, 0)));

	//left
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(1, 0, 0)));

	//right
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(-1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(-1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(-1, 0, 0)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(-1, 0, 0)));

	//back
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(0, 0, -1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(0, 0, -1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, -sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(0, 0, -1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, -sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(0, 0, -1)));

	//front
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 0), glm::vec3(0, 0, 1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(-sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 0), glm::vec3(0, 0, 1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, sizeCube.y, sizeCube.z), colorCube, glm::vec2(1, 1), glm::vec3(0, 0, 1)));
	vertexesList.push_back(Vertex::PCTN(positionCube + glm::vec3(sizeCube.x, -sizeCube.y, sizeCube.z), colorCube, glm::vec2(0, 1), glm::vec3(0, 0, 1)));
}


//Destructor
CubeComponent::~CubeComponent()
{
}

// Draw method for the cube
void CubeComponent::draw()
{
	tigl::drawVertices(GL_QUADS, vertexesList);
}

//Get vertices method for cube.
std::vector<glm::vec3> CubeComponent::getVertices()
{
	std::vector<glm::vec3> list;
	for (const auto &vert : vertexesList) {
		list.push_back(vert.position);
	}
	return list;
}
