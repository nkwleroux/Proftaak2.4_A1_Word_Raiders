#include "CrosshairComponent.h"

CrosshairComponent::CrosshairComponent(float size)
{
	glm::vec3 s(size);
	glm::vec3 p(0, 0, 0);

	verts.push_back(tigl::Vertex::PT(glm::vec3(-size, -size, -size), glm::vec2(0, 1)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(-size, size, -size), glm::vec2(1, 1)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(size, size, -size), glm::vec2(1, 0)));
	verts.push_back(tigl::Vertex::PT(glm::vec3(size, -size, -size), glm::vec2(0, 0)));
}


CrosshairComponent::~CrosshairComponent()
{
}

//void CrosshairComponent::setTexture(Texture* t)
//{
//	texture = t;
//	texture->bind();
//}

void CrosshairComponent::draw()
{
	tigl::drawVertices(GL_QUADS, verts);
}
