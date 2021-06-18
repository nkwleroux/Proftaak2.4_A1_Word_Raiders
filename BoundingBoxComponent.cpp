#include "BoundingBoxComponent.h"
#include "ObjectModelComponent.h"
#include "GameObject.h"
#include "CubeComponent.h"
#include "tigl.h"
#include <iostream>

// Constructor of the BoundingBox class
BoundingBoxComponent::BoundingBoxComponent(GameObject* gameObject) {
	//Setting value of min and max boundingbox
	minBoundingBox = glm::vec3(std::numeric_limits<float>::max());
	maxBoundingBox = glm::vec3(std::numeric_limits<float>::min());

	//Creating a drawcomponent
	DrawComponent* modelComponent = gameObject->getComponent<DrawComponent>();

	// Retreive the vertices
	auto vertices = modelComponent->getVertices();


	for (const auto& vertex : vertices)
	{
		// 3 for x , y , z
		for (int i = 0; i < 3; i++)
		{
			minBoundingBox[i] = glm::min(minBoundingBox[i], vertex[i]);
			maxBoundingBox[i] = glm::max(maxBoundingBox[i], vertex[i]);
		}
	}
}

// Destructor of the BoundingBox class
BoundingBoxComponent::~BoundingBoxComponent() {

}

// Method to add vectors to the component
void BoundingBoxComponent::addVector(glm::vec3 vector) {
	minBoundingBox += vector;
	maxBoundingBox += vector;
}

//Collidewithobject method checks if this object collides with another object
bool BoundingBoxComponent::collideWithObject(GameObject* otherObject)
{
	//Retreiving the real minimum and maximum value from the current object
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(minBoundingBox, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(maxBoundingBox, 1.0f);

	//Retreiving the real minimum and maximum value from the other object
	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->minBoundingBox, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->maxBoundingBox, 1.0f);

	//Check for collision
	collisionX = (realMin.x <= otherRealMax.x && realMax.x >= otherRealMin.x);
	collisionY = (realMin.y <= otherRealMax.y && realMax.y >= otherRealMin.y);
	collisionZ = (realMin.z <= otherRealMax.z && realMax.z >= otherRealMin.z);

	//Return if there is collision or not
	 return collisionX && collisionY && collisionZ;
}

bool BoundingBoxComponent::collideWithWall(GameObject* otherObject)
{
	//Retreiving the real minimum and maximum value from the current object
	glm::vec4 realMin = gameObject->modelMatrix * glm::vec4(minBoundingBox, 1.0f);
	glm::vec4 realMax = gameObject->modelMatrix * glm::vec4(maxBoundingBox, 1.0f);

	//Retreiving the real minimum and maximum value from the other object
	glm::vec4 otherRealMin = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->minBoundingBox, 1.0f);
	glm::vec4 otherRealMax = otherObject->modelMatrix * glm::vec4(otherObject->getComponent<BoundingBoxComponent>()->maxBoundingBox, 1.0f);

	//Check for collision , //Return if there is collision or not
	return !((realMin.x <= otherRealMin.x && realMax.x >= otherRealMax.x) &&
		(realMin.y <= otherRealMin.y && realMax.y >= otherRealMax.y) &&
		(realMin.z <= otherRealMin.z && realMax.z >= otherRealMax.z));
}

//Method for drawing the component
void BoundingBoxComponent::draw()
{
	//Enable colors
	tigl::shader->enableColor(true);

	// select a polygon rasterization mode
	// face
	// 	Specifies the polygons that mode applies to.Must be GL_FRONT_AND_BACK for front - and back - facing polygons.
	// mode
	//  Specifies how polygons will be rasterized.Accepted values are GL_POINT, GL_LINE, and GL_FILL.The initial value is GL_FILL for both front - and back - facing polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// get vertices from component
	std::vector<glm::vec3> vec3s = gameObject->getComponent<CubeComponent>()->getVertices();

	// defining variable for storing the vertices
	std::vector<Vertex> vertices;

	for (const auto& vert : vec3s) {
		// add every vec3 to the vertex variable
		vertices.push_back(tigl::Vertex::PC(glm::vec3(vert.x + 1, vert.y + 1, vert.z + 1), glm::vec4(1,0,0,1)));
	}

	// draw from the vertices variable
	tigl::drawVertices(GL_QUADS, vertices);
}

//Getter for the vertices
std::vector<glm::vec3> BoundingBoxComponent::getVertices()
{
	return std::vector<glm::vec3>();
}
