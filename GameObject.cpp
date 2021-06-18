#include "GameObject.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>


// Constructor
GameObject::GameObject(int id) : id(id)
{
}

// Destructor
GameObject::~GameObject()
{
}

// Add component to component list
void GameObject::addComponent(Component * component)
{
	component->setGameObject(this);
	components.push_back(component);

	// If component hasnt been drawn, drawn it
	if (!drawComponent)
		drawComponent = dynamic_cast<DrawComponent*>(component);
}

// Getter to retreive components
std::list<Component*> GameObject::getComponents()
{
	return components;
}

// Overloading
bool operator==(const GameObject object1, const GameObject object2) {
	return true;
}

// Draw function for the gameobject
void GameObject::draw(const glm::mat4 &parentMatrix)
{
	// If there is no drawcomponent, exit the function
	if (!drawComponent)

		return;

	// Translate, rotate, and scale with vectors
	modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	// Add the matrix to the shader, so the shader can process the changes
	tigl::shader->setModelMatrix(modelMatrix);
	// Draw the component
	drawComponent->draw();
}

// Update method for the gameobject
void GameObject::update(float elapsedTime)
{
	for (auto &c : components)
		c->update(elapsedTime);
}