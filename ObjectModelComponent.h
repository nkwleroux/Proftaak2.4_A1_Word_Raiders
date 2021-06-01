#pragma once
#include "DrawComponent.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "tigl.h"

#include <string>
#include <vector>
#include <list>

class Texture;



class ObjectModelComponent : public DrawComponent
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord; 
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};


	class MaterialInfo
	{
	public:
		std::string name;
		Texture* texture;
		glm::vec3 ka;
		glm::vec3 kd;
		glm::vec3 ks;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};

	//todo remove these when vbo is created
	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	std::vector<tigl::VBO*> vbos;

	void loadMaterialFile(const std::string& fileName, const std::string& dirName);

public:
	void createVBO();
	ObjectModelComponent(const std::string &filename);
	~ObjectModelComponent(void);
	int materialIndex;

	virtual void draw() override;
};

