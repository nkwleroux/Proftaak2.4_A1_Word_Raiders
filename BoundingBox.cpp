#include "BoundingBox.h"
#include "ObjectModelComponent.h"
#include "GameObject.h"

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

bool BoundingBox::collide(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);


	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->max, 1.0f);

	//Chech for collision
	return (realMin.x <= otherRealMax.x && realMax.x >= otherRealMin.x) && 
		(realMin.y <= otherRealMax.y && realMax.y >= otherRealMin.y) && 
		(realMin.z <= otherRealMax.z && realMax.z >= otherRealMin.z);

}