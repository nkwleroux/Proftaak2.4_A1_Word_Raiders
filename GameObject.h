#pragma once
#include <list>
#include <glm/glm.hpp>
#include "tigl.h"

class Component;
class DrawComponent;

// Using to make things a bit easier ofr us
using tigl::Vertex;


class GameObject
{
	// Setting drawcomponent
	DrawComponent* drawComponent = nullptr;

	// Creating variable for a list of components
	std::list<Component*> components;
public:
	//Constructor
	GameObject(int id);
	// Destrucotr
	~GameObject();

	// Id of gameobject
	int id = 0;

	//Matrix of gameobject
	glm::mat4 modelMatrix;
	//Position and rotation and scaling vectore for the game object
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1,1,1);

	// Add components to component list
	void addComponent(Component* component);

	// Update function
	void update(float elapsedTime);

	// Draw function to draw the gameobject
	void draw(const glm::mat4& = glm::mat4(1.0f));

	// Getter for components
	std::list<Component*> getComponents();

	// The friend declaration appears in a class body and grants a function or another class access to private and protected members of the class where the friend declaration appears.
	friend bool operator==(const GameObject object1, const GameObject object2);

	// Create template for class
	template<class T>

	// Getter to retreive the components
	T* getComponent()
	{
		for (auto c : components)
		{
			T* t = dynamic_cast<T*>(c);
			if (t)
				return t;
		}
		return nullptr;
	}

	template<class T>

	// (kinda) setter to remove components
	void removeComponent()
	{
		components.remove_if([](Component* c)
		{
			T* t = dynamic_cast<T*>(c);
			return t != nullptr;
		});
	}

};

