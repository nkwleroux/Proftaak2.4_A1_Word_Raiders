#include "BoundingBoxComponent.h"
#include "ObjectModelComponent.h"
#include "GameObject.h"
#include "CubeComponent.h"
#include "tigl.h"
#include <iostream>

BoundingBoxComponent::BoundingBoxComponent(GameObject* gameObject) {
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
BoundingBoxComponent::~BoundingBoxComponent() {

}
void BoundingBoxComponent::addVector(glm::vec3 vector) {
	min += vector;
	max += vector;
}

bool BoundingBoxComponent::collideWithObject(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);

	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->max, 1.0f);

	//Check for collision

	collisionX = (realMin.x <= otherRealMax.x && realMax.x >= otherRealMin.x);
	collisionY = (realMin.y <= otherRealMax.y && realMax.y >= otherRealMin.y); 
	collisionZ = (realMin.z <= otherRealMax.z && realMax.z >= otherRealMin.z);
	 return collisionX && collisionY && collisionZ;
}

bool BoundingBoxComponent::collideWithWall(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);

	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->max, 1.0f);

	//Check for collision
	return !((realMin.x <= otherRealMin.x && realMax.x >= otherRealMax.x) &&
		(realMin.y <= otherRealMin.y && realMax.y >= otherRealMax.y) &&
		(realMin.z <= otherRealMin.z && realMax.z >= otherRealMax.z));
	/*(otherRealMin.x <= realMin.x && otherRealMax.x >= realMin.x) &&
		(otherRealMin.y <= realMin.y && otherRealMax.y >= realMin.y) &&
		(otherRealMin.z <= realMin.z && otherRealMax.z >= realMin.z);*/
}

void BoundingBoxComponent::draw()
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

std::vector<glm::vec3> BoundingBoxComponent::getVertices()
{
	return std::vector<glm::vec3>();
}
