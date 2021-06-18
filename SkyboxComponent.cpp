#include "SkyboxComponent.h"

SkyboxComponent::SkyboxComponent(float size, Texture** textures) : textures{ textures }
{
	// Variables for x y and z axis
	// /2 for middle
	x = x - size / 2;
	y = y - size / 2;
	z = z - size / 2;

	// variables for position and normal
	glm::vec3 position(0, 0, 0);
	glm::vec3 normal(0, 0, 0);

	//middle
	std::vector<Vertex> vertexMiddle;
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z), glm::vec2(1, 0),			normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z), glm::vec2(1, 1),	normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z), glm::vec2(0, 1),			normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(0, 0),					normal));
	vertexList.push_back(vertexMiddle);

	//right
	std::vector<Vertex> vertexRight;
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z), glm::vec2(0, 0),				normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z + size), glm::vec2(1, 0),		normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z + size), glm::vec2(1, 1), normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z), glm::vec2(0, 1),		normal));
	vertexList.push_back(vertexRight);

	//left
	std::vector<Vertex> vertexLeft;
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z), glm::vec2(0, 0),		normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z + size), glm::vec2(1, 0), normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y, z + size), glm::vec2(1, 1),		normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(0, 1),				normal));
	vertexList.push_back(vertexLeft);

	//back
	std::vector<Vertex> vertexBack;
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x, y, z + size), glm::vec2(1, 0),				normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z + size), glm::vec2(1, 1),			normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z + size), glm::vec2(0, 1),	normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z + size), glm::vec2(0, 0),			normal));
	vertexList.push_back(vertexBack);

	//bottom
	std::vector<Vertex> vertexBottom;
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(1, 0),					normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x, y, z + size), glm::vec2(1, 1),			normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z + size), glm::vec2(0, 1),	normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x + size, y, z), glm::vec2(0, 0),			normal));
	vertexList.push_back(vertexBottom);

	//top
	std::vector<Vertex> vertexTop;
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z), glm::vec2(1, 0),			normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x + size, y + size, z + size), glm::vec2(1, 1),	normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z + size), glm::vec2(0, 1),			normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x, y + size, z), glm::vec2(0, 0),					normal));
	vertexList.push_back(vertexTop);

}

SkyboxComponent::SkyboxComponent(float height, float width, float length, Texture** textures) : textures{ textures }
{
	// Center the skybox
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Creating variable for positon and normal
	glm::vec3 position(0, 0, 0);
	glm::vec3 normal(0, 0, 0);

	//middle
	std::vector<Vertex> vertexMiddle;
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z), glm::vec2(1, 0),				normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z), glm::vec2(1, 1),	normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z), glm::vec2(0, 1),			normal));
	vertexMiddle.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(0, 0),						normal));
	vertexList.push_back(vertexMiddle);

	//right
	std::vector<Vertex> vertexRight;
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z), glm::vec2(0, 0),						normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z + length), glm::vec2(1, 0),				normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z + length), glm::vec2(1, 1),	normal));
	vertexRight.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z), glm::vec2(0, 1),				normal));
	vertexList.push_back(vertexRight);

	//left
	std::vector<Vertex> vertexLeft;
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z), glm::vec2(0, 0),				normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z + length), glm::vec2(1, 0),		normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y, z + length), glm::vec2(1, 1),				normal));
	vertexLeft.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(0, 1),						normal));
	vertexList.push_back(vertexLeft);

	//back
	std::vector<Vertex> vertexBack;
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x, y, z + length), glm::vec2(1, 0),					normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z + length), glm::vec2(1, 1),			normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z + length), glm::vec2(0, 1), normal));
	vertexBack.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z + length), glm::vec2(0, 0),			normal));
	vertexList.push_back(vertexBack);

	//bottom
	std::vector<Vertex> vertexBottom;
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x, y, z), glm::vec2(1, 0),						normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x, y, z + length), glm::vec2(1, 1),			normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z + length), glm::vec2(0, 1),	normal));
	vertexBottom.push_back(Vertex::PTN(position + glm::vec3(x + width, y, z), glm::vec2(0, 0),				normal));
	vertexList.push_back(vertexBottom);

	//top
	std::vector<Vertex> vertexTop;
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z), glm::vec2(1, 0),			normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x + width, y + height, z + length), glm::vec2(1, 1),	normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z + length), glm::vec2(0, 1),			normal));
	vertexTop.push_back(Vertex::PTN(position + glm::vec3(x, y + height, z), glm::vec2(0, 0),					normal));
	vertexList.push_back(vertexTop);

}

// Destructor
SkyboxComponent::~SkyboxComponent()
{
}

// Draw function for the skybox
void SkyboxComponent::draw()
{
	// glLoadIdentity — replace the current matrix with the identity matrix
	glLoadIdentity();
	// enable texture and color
	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);
	// enable 2D
	glEnable(GL_TEXTURE_2D);
	// for loop to bind each texture side
	for (int i = 0; i < 6; i++)
	{
		textures[i]->bind();
		tigl::drawVertices(GL_QUADS, vertexList[i]);
	}
	// disable texture 2D
	glDisable(GL_TEXTURE_2D);
}

// Getter for the vertices
std::vector<glm::vec3> SkyboxComponent::getVertices()
{
	// Variable for the verticesList
	std::vector<glm::vec3> verticesList;
	// Add all sides to the list of vertices
	for (int i = 0; i < 6; i++)
	{
		for (const auto& vert : vertexList[i]) {
			verticesList.push_back(vert.position);
		}
	}
	return verticesList;
}


