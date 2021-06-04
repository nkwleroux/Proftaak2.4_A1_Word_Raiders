#include "BoundingBox.h"
#include "ObjectModelComponent.h"
#include "GameObject.h"
#include "CubeComponent.h"
#include "tigl.h"
#include <iostream>

BoundingBox::BoundingBox(GameObject* gameObject) {
	min = glm::vec3(std::numeric_limits<float>::max());
	max = glm::vec3(std::numeric_limits<float>::min());

	DrawComponent* modelComponent = gameObject->getComponent<DrawComponent>();

	auto vertices = modelComponent->getVertices();
	for (const auto& vertex : vertices)
	{
		for (int i = 0; i < 3; i++)
		{
			min[i] = glm::min(min[i], vertex[i]);
			max[i] = glm::max(max[i], vertex[i]);
		}
	}
}
BoundingBox::~BoundingBox() {

}
void BoundingBox::addVector(glm::vec3 vector) {
	min += vector;
	max += vector;
}

bool BoundingBox::collideWithObject(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);

	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->max, 1.0f);

	//Check for collision
	return (realMin.x <= otherRealMax.x && realMax.x >= otherRealMin.x) &&
		(realMin.y <= otherRealMax.y && realMax.y >= otherRealMin.y) &&
		(realMin.z <= otherRealMax.z && realMax.z >= otherRealMin.z);

}

bool BoundingBox::collideWithWall(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);

	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->max, 1.0f);

	//Check for collision
	return !((realMin.x <= otherRealMin.x && realMax.x >= otherRealMax.x) &&
		(realMin.y <= otherRealMin.y && realMax.y >= otherRealMax.y) &&
		(realMin.z <= otherRealMin.z && realMax.z >= otherRealMax.z));
	/*(otherRealMin.x <= realMin.x && otherRealMax.x >= realMin.x) &&
		(otherRealMin.y <= realMin.y && otherRealMax.y >= realMin.y) &&
		(otherRealMin.z <= realMin.z && otherRealMax.z >= realMin.z);*/
}

void BoundingBox::draw()
{
	tigl::shader->enableColor(true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::vector<glm::vec3> vec3s = gameObject->getComponent<CubeComponent>()->getVertices();

	std::vector<Vertex> verts;

	for (const auto& vert : vec3s) {
		verts.push_back(tigl::Vertex::PC(glm::vec3(vert.x + 1, vert.y + 1, vert.z + 1), glm::vec4(1,0,0,1)));
	}

	tigl::drawVertices(GL_QUADS, verts);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

std::vector<glm::vec3> BoundingBox::getVertices()
{
	return std::vector<glm::vec3>();
}
