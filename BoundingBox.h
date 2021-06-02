#pragma once
#include "tigl.h"
#include "Component.h"

#include <vector>
class BoundingBox : public Component
{
public:
	glm::vec3 min, max;

	BoundingBox();
	~BoundingBox();
	void addVector(glm::vec3 vector);


	bool collide(GameObject* otherObject);
};
