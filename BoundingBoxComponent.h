#pragma once
//#include "tigl.h"
#include "Component.h"
#include "DrawComponent.h"
#include <vector>

class BoundingBoxComponent : public DrawComponent
{

public:
	// Minimum and maximum lenght of the bounding box component
	glm::vec3 minBoundingBox, maxBoundingBox;

	// Set collision on false, because there is none yet
	bool collisionX = false, collisionY = false, collisionZ = false;

	// Defining the constructor
	BoundingBoxComponent(GameObject* gameObject);

	// Defining the destructor
	~BoundingBoxComponent();

	// Add a vector component
	void addVector(glm::vec3 vector);

	//Boolean for colliding with another object
	bool collideWithObject(GameObject* otherObject);

	//Boolean for colliding with the skybox wall
	bool collideWithWall(GameObject* otherObject);

	//virtual void setTexture(Texture* t) override;
	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};
