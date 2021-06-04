#pragma once
#include "tigl.h"
#include "Component.h"
#include "DrawComponent.h"

#include <vector>
class BoundingBox : public Component
{

public:
	glm::vec3 min, max;

	BoundingBox(GameObject* gameObject);
	~BoundingBox();
	void addVector(glm::vec3 vector);

	bool collide(GameObject* otherObject);

};
