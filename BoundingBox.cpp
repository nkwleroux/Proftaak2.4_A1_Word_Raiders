#include "BoundingBox.h"
#include "ObjectModelComponent.h"
#include "GameObject.h"

BoundingBox::BoundingBox() {
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
	//minX += vector.x;
	//maxX += vector.x;
	//minY += vector.y;
	//maxY += vector.y;
	//minZ += vector.z;
	//maxZ += vector.z;
}

bool BoundingBox::collide(GameObject* otherObject)
{
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(min, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(max, 1.0f);


	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->min, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBox>()->max, 1.0f);

	//Chech for collision
	return (realMin.x <= otherRealMax.x && realMax.x >= otherRealMin.x) && (realMin.y <= ;

	a.minX <= b.maxX && a.maxX >= b.minX) && (a.minY <= b.maxY && a.maxY >= b.minY) && (a.minZ <= b.maxZ && a.maxZ >= b.minZ

}
