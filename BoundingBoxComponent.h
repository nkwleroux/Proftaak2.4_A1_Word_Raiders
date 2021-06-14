#pragma once
#include "tigl.h"
#include "Component.h"
#include "DrawComponent.h"

#include <vector>
class BoundingBoxComponent : public DrawComponent
{

public:
	glm::vec3 min, max;

	bool collisionX = false, collisionY = false, collisionZ = false;

	BoundingBoxComponent(GameObject* gameObject);
	~BoundingBoxComponent();
	void addVector(glm::vec3 vector);

	bool collideWithObject(GameObject* otherObject);
	bool collideWithWall(GameObject* otherObject);

	//virtual void setTexture(Texture* t) override;
	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};
