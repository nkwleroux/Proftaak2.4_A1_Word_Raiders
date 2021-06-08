#include "WallComponent.h"

WallComponent::WallComponent(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4,
	glm::vec2 texturePos1, glm::vec2 texturePos2, glm::vec2 texturePos3, glm::vec2 texturePos4, 
	glm::vec3 normal = glm::vec3(0))
{
	glm::vec3 p(0, 0, 0);

	verts.push_back(Vertex::PTN(p + pos1, texturePos1, normal));
	verts.push_back(Vertex::PTN(p + pos2, texturePos2, normal));
	verts.push_back(Vertex::PTN(p + pos3, texturePos3, normal));
	verts.push_back(Vertex::PTN(p + pos4, texturePos4, normal));

	vbo = tigl::createVbo(verts);
}

WallComponent::~WallComponent()
{
}

void WallComponent::draw()
{
	tigl::drawVertices(GL_QUADS, vbo);
}
