#include "PlayerComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

//Constructor
PlayerComponent::PlayerComponent()
{
}

// Destructor
PlayerComponent::~PlayerComponent()
{
}

void PlayerComponent::update(float elapsedTime)
{
	// If W is pressed, we move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gameObject->position.z -= elapsedTime * speed;
	// If S is pressed, move backwards
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gameObject->position.z += elapsedTime * speed;
	// If A is pressed, we move to the left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gameObject->position.x -= elapsedTime * speed;
	// If D is pressed, we move to the right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gameObject->position.x += elapsedTime * speed;

}
