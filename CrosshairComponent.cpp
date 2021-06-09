#include "CrosshairComponent.h"


CrosshairComponent::CrosshairComponent(float size)
{
	initTextures();
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


void CrosshairComponent::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);

	textureCrosshair[currentCrosshair]->bind();

	tigl::drawVertices(GL_QUADS, verts);

	glDisable(GL_BLEND);
}

std::vector<glm::vec3> CrosshairComponent::getVertices()
{
	std::vector<glm::vec3> list;
	for (const auto& vert : verts) {
		list.push_back(vert.position);
	}
	return list;
}

void CrosshairComponent::initTextures() {
	textureCrosshair[0] = new Texture("Images/openHand.png");
	textureCrosshair[1] = new Texture("Images/closedHand.png");
}