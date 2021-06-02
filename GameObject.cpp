#include "GameObject.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(int id = 0)
{
	id = id;
}


GameObject::~GameObject()
{
}

void GameObject::addComponent(Component * component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (!drawComponent)
		drawComponent = dynamic_cast<DrawComponent*>(component);
}

std::list<Component*> GameObject::getComponents()
{
	return components;
}

BoundingBox* GameObject::createBoundingBox(std::vector<Vertex> verts) {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    for (auto& vert : verts) {
        if (minX > vert.position.x) {
            minX = vert.position.x;
        }
        else if (maxX < vert.position.x) {
            maxX = vert.position.x;
        }
        else if (minY > vert.position.y) {
            minY = vert.position.y;
        }
        else if (maxY < vert.position.y) {
            maxY = vert.position.y;
        }
        else if (minZ > vert.position.z) {
            minZ = vert.position.z;
        }
        else if (maxZ < vert.position.z) {
            maxZ = vert.position.z;
        }
    }

    boundingBox = new BoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
    return boundingBox;
	
}


void GameObject::draw(const glm::mat4 &parentMatrix)
{
	if (!drawComponent)
		return;

	modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);
	
	tigl::shader->setModelMatrix(modelMatrix);
	drawComponent->draw();
}

void GameObject::update(float elapsedTime)
{
	for (auto &c : components)
		c->update(elapsedTime);
}