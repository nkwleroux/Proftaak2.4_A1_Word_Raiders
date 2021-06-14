#include "SkyboxComponent.h"

SkyboxComponent::SkyboxComponent(float size, Texture** textures) : textures{ textures }
{
	x = x - size / 2;
	y = y - size / 2;
	z = z - size / 2;

	glm::vec3 p(0, 0, 0);
	glm::vec3 n(0, 0, 0);

	//middle
	std::vector<Vertex> vertexMiddle;
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z), glm::vec2(1, 0),			n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z), glm::vec2(1, 1),	n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z), glm::vec2(0, 1),			n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(0, 0),					n));
	verts.push_back(vertexMiddle);

	//right
	std::vector<Vertex> vertexRight;
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z), glm::vec2(0, 0),				n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z + size), glm::vec2(1, 0),		n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z + size), glm::vec2(1, 1), n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z), glm::vec2(0, 1),		n));
	verts.push_back(vertexRight);

	//left
	std::vector<Vertex> vertexLeft;
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z), glm::vec2(0, 0),		n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z + size), glm::vec2(1, 0), n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y, z + size), glm::vec2(1, 1),		n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(0, 1),				n));
	verts.push_back(vertexLeft);

	//back
	std::vector<Vertex> vertexBack;
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x, y, z + size), glm::vec2(1, 0),				n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z + size), glm::vec2(1, 1),			n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z + size), glm::vec2(0, 1),	n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z + size), glm::vec2(0, 0),			n));
	verts.push_back(vertexBack);

	//bottom
	std::vector<Vertex> vertexBottom;
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(1, 0),					n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x, y, z + size), glm::vec2(1, 1),			n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z + size), glm::vec2(0, 1),	n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x + size, y, z), glm::vec2(0, 0),			n));
	verts.push_back(vertexBottom);

	//top
	std::vector<Vertex> vertexTop;
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z), glm::vec2(1, 0),			n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x + size, y + size, z + size), glm::vec2(1, 1),	n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z + size), glm::vec2(0, 1),			n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x, y + size, z), glm::vec2(0, 0),					n));
	verts.push_back(vertexTop);
	
}

SkyboxComponent::SkyboxComponent(float height, float width, float length, Texture** textures) : textures{ textures }
{
	// Center the skybox
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	glm::vec3 p(0, 0, 0);
	glm::vec3 n(0, 0, 0);

	//middle
	std::vector<Vertex> vertexMiddle;
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z), glm::vec2(1, 0),				n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z), glm::vec2(1, 1),	n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z), glm::vec2(0, 1),			n));
	vertexMiddle.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(0, 0),						n));
	verts.push_back(vertexMiddle);

	//right
	std::vector<Vertex> vertexRight;
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z), glm::vec2(0, 0),						n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z + length), glm::vec2(1, 0),				n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length), glm::vec2(1, 1),	n));
	vertexRight.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z), glm::vec2(0, 1),				n));
	verts.push_back(vertexRight);

	//left
	std::vector<Vertex> vertexLeft;
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z), glm::vec2(0, 0),				n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z + length), glm::vec2(1, 0),		n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y, z + length), glm::vec2(1, 1),				n));
	vertexLeft.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(0, 1),						n));
	verts.push_back(vertexLeft);

	//back
	std::vector<Vertex> vertexBack;
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x, y, z + length), glm::vec2(1, 0),					n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z + length), glm::vec2(1, 1),			n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length), glm::vec2(0, 1), n));
	vertexBack.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z + length), glm::vec2(0, 0),			n));
	verts.push_back(vertexBack);

	//bottom
	std::vector<Vertex> vertexBottom;
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x, y, z), glm::vec2(1, 0),						n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x, y, z + length), glm::vec2(1, 1),			n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z + length), glm::vec2(0, 1),	n));
	vertexBottom.push_back(Vertex::PTN(p + glm::vec3(x + width, y, z), glm::vec2(0, 0),				n));
	verts.push_back(vertexBottom);

	//top
	std::vector<Vertex> vertexTop;
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z), glm::vec2(1, 0),			n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x + width, y + height, z + length), glm::vec2(1, 1),	n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z + length), glm::vec2(0, 1),			n));
	vertexTop.push_back(Vertex::PTN(p + glm::vec3(x, y + height, z), glm::vec2(0, 0),					n));
	verts.push_back(vertexTop);

}

SkyboxComponent::~SkyboxComponent()
{
}

void SkyboxComponent::draw()
{
	glLoadIdentity();
	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < 6; i++)
	{
		textures[i]->bind();
		tigl::drawVertices(GL_QUADS, verts[i]);
	}
	glDisable(GL_TEXTURE_2D);
}

std::vector<glm::vec3> SkyboxComponent::getVertices()
{
	std::vector<glm::vec3> list;
	for (int i = 0; i < 6; i++)
	{
		for (const auto& vert : verts[i]) {
			list.push_back(vert.position);
		}
	}
	return list;
}


