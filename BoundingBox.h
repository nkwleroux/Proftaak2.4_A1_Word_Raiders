#pragma once
#include "tigl.h"
#include "Component.h"
#include "DrawComponent.h"

#include <vector>
class BoundingBox : public DrawComponent
{

public:
	glm::vec3 min, max;

	BoundingBox(GameObject* gameObject);
	~BoundingBox();
	void addVector(glm::vec3 vector);

	bool collideWithObject(GameObject* otherObject);
	bool collideWithWall(GameObject* otherObject);

	//virtual void setTexture(Texture* t) override;
	virtual void draw() override;

	// Inherited via DrawComponent
	virtual std::vector<glm::vec3> getVertices() override;

};
