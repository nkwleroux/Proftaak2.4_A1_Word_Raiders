#pragma once

#include <list>
#include <glm/glm.hpp>
#include "BoundingBox.h"

using tigl::Vertex;

class Component;
class DrawComponent;


class GameObject
{
	DrawComponent* drawComponent = nullptr;
	
	std::list<Component*> components;

public:
	GameObject(int id);
	~GameObject();

	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1,1,1);
	int id;
	std::vector<Vertex> verts;
	BoundingBox *boundingBox;

	void addComponent(Component* component);
	std::list<Component*> getComponents();
	void update(float elapsedTime);
	void draw(const glm::mat4& = glm::mat4(1.0f));

	template<class T>
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
	void removeComponent()
	{
		components.remove_if([](Component* c) 
		{
			T* t = dynamic_cast<T*>(c);
			return t != nullptr;
		});
	}

};

