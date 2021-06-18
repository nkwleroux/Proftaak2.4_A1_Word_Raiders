#pragma once

class GameObject;

class Component
{
protected:
	GameObject* gameObject;
public:
	//Constructor
	Component();

	//Destructor
	~Component();

	//Update function
	virtual void update(float elapsedTime) {};

	//Set game object
	inline void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
};

