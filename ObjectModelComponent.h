#pragma once
#include "DrawComponent.h"
#include <glm/glm.hpp>
#include "tigl.h"
#include <string>
#include <vector>
#include <list>
#include "Texture.h"

// We want to use a texture and map it to the component
class Texture;

class ObjectModelComponent : public DrawComponent
{
private:
	class Vertex
	{
	public:
		// Position of component
		int position;
		// Normal of the component
		int normal;
		// coordinates of the texture
		int textureCoord;
	};

	class Face
	{
	public:
		// List of vertices
		std::list<Vertex> vertices;
	};

	// Class that contains info about the materials
	class MaterialInfo
	{
	public:
		std::string name;
		Texture* texture;
		// Ka: specifies ambient color, to account for light that is scattered about the entire scene
		// using values between 0 and 1 for the RGB components.
		glm::vec3 ka;
		//Kd: specifies diffuse color, which typically contributes most of the color to an object
		glm::vec3 kd;
		// Ks: specifies specular color, the color seen where the surface is shiny and mirror-like [
		glm::vec3 ks;
	};

	// Group of objects
	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};
	// Needed for bounding box
	std::vector<glm::vec3>	vertices;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	std::vector<tigl::VBO*> vbos;

	// Method to load the material file
	void loadMaterialFile(const std::string& fileName, const std::string& dirName);
	// Method to create VBO
	void createVBO(std::vector<glm::vec3>	normals, std::vector<glm::vec2>	texcoords);

public:
	// Constructor in which you should input the filename.
	ObjectModelComponent(const std::string& filename);
	// Constructor in which you should input the filename and texture
	ObjectModelComponent(const std::string &filename, Texture* texture);
	// Destructor
	~ObjectModelComponent(void);

	// Int to keep track of the material index
	int materialIndex;

	// Draw function of the component
	virtual void draw() override;

	// Getter for the vertices
	std::vector<glm::vec3> getVertices() override;
};

