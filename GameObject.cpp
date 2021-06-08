#include "GameObject.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(int id)
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

bool operator==(const GameObject object1, const GameObject object2) {
	return true;
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