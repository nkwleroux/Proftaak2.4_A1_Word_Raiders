#include "ObjectModelComponent.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "Texture.h"

using tigl::Vertex;

/**
* Replaces a substring in a string
*/
static std::string replace(std::string str, const std::string &toReplace, const std::string &replacement)
{
	size_t index = 0;
	while (true)
	{
		 index = str.find(toReplace, index);
		 if (index == std::string::npos)
			 break;
		 str.replace(index, toReplace.length(), replacement);
		 ++index;
	}
	return str;
}

/**
* Splits a string into substrings, based on a seperator
* Seperator can be passed in trough the method
*/
static std::vector<std::string> split(std::string str, const std::string &seperator)
{
	// List in where we can store the strings
	std::vector<std::string> stringList;
	// Variable to store the index size
	size_t sizeString;
	while(true)
	{
		sizeString = str.find(seperator);
		if(sizeString == std::string::npos)
			break;
		stringList.push_back(str.substr(0, sizeString));
		str = str.substr(sizeString+1);
	}
	stringList.push_back(str);
	return stringList;
}

/**
* Turns a string to lowercase
*/
static inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}


/**
* Cleans up a line for processing
*/
static inline std::string cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}


void ObjectModelComponent::createVBO(std::vector<glm::vec3>	normals, std::vector<glm::vec2>	texcoords) {
	//foreach group in groups
	for (int groupPos = 0; groupPos < this->groups.size(); groupPos++)
	{
		// variable for the color and list of vertices
		glm::vec4 color = glm::vec4(1.0f);
		std::vector<tigl::Vertex> verticesList;

		//  set material texture, if available
		if (materials.size() > this->groups[groupPos]->materialIndex && this->groups[groupPos]->materialIndex != -1)
		{
			if (materials[(this->groups[groupPos]->materialIndex)]->texture != NULL)
			{
				materialIndex = this->groups[groupPos]->materialIndex;
			}

			color = glm::vec4(1.0f);
		}


		//  foreach face in group
		for (auto face : this->groups[groupPos]->faces) {

			//    foreach vertex in face
			for (auto vertex : face.vertices) {
				//emit vertex
				if (materialIndex!=-1)
				{
					verticesList.push_back(tigl::Vertex::PTN(
						vertices[(vertex.position)],
						texcoords[(vertex.textureCoord)],
						normals[(vertex.normal)]));
				}
				else
				{
					verticesList.push_back(tigl::Vertex::PC(vertices[(vertex.position)], color));
				}
			}
		}

		// add to vertices List
		vbos.push_back(tigl::createVbo(verticesList));
	}

}

/**
* Loads an object model
*/
ObjectModelComponent::ObjectModelComponent(const std::string &fileName)
{
	// variable for normals and coordinates of the text
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;

	// start index at -1
	materialIndex = -1;

	// Print what file we are trying to load
	std::cout << "Loading " << fileName << std::endl;
	// Setting directory to a local variable
	std::string dirName = fileName;
	//Translating the dir name to something we can use
	if(dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	//Translating the dir name to something we can use
	if(dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	//Translating the dir name to something we can use
	if(fileName == dirName)
		dirName = "";

	// Load the file
	std::ifstream pFile(fileName.c_str());

	// If we can't load the file, print thi
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	// Create a new objgroup
	ObjGroup* currentGroup = new ObjGroup();
	// Set index at -1
	currentGroup->materialIndex = -1;

	// Reading trough the file, till the end
	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if(line == "" || line[0] == '#') //skip empty or commented line
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		// Adding to the vertices list on certain conditions
		if(params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));
		else if(params[0] == "f")
		{
			for(size_t ii = 4; ii <= params.size(); ii++)
			{
				Face face;

				for(size_t i = ii-3; i < ii; i++)	//magische forlus om van quads triangles te maken ;)
				{
					Vertex vertex;
					std::vector<std::string> indices = split(params[i == (ii-3) ? 1 : i], "/");
					if (indices.size() >= 1)	//We have a positon
						vertex.position = atoi(indices[0].c_str()) - 1;
					if(indices.size() == 2)		//Only texture
						vertex.textureCoord = atoi(indices[1].c_str())-1;
					if(indices.size() == 3)		//v/t/n or v//n
					{
						if( indices[1] != "")
							vertex.textureCoord = atoi(indices[1].c_str())-1;
						vertex.normal = atoi(indices[2].c_str())-1;
					}
					// Add to the vertex list
					face.vertices.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}
		}
		else if(params[0] == "s")
		{
			//Smoothing groups
		}
        else if(params[0] == "mtllib")
        {
            loadMaterialFile(dirName + "/" + params[1], dirName);
        }
		else if(params[0] == "usemtl")
		{
			if(currentGroup->faces.size() != 0)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->materialIndex = -1;

			for(size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if(info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if(currentGroup->materialIndex == -1)
				std::cout<<"Could not find material name "<<params[1]<<std::endl;
		}
	}
	// Add groups to the current group
	groups.push_back(currentGroup);

	// Create the VBO
	createVBO(normals,texcoords);
}

ObjectModelComponent::ObjectModelComponent(const std::string& fileName, Texture* texture)
{
	// List of normals
	std::vector<glm::vec3>	normals;
	// List of texture coordinates
	std::vector<glm::vec2>	texcoords;

	// Get current material
	MaterialInfo* currentMaterial = new MaterialInfo();
	currentMaterial->name = "Material";
	currentMaterial->texture = texture;
	materials.push_back(currentMaterial);

	// Setting starting index to -1
	materialIndex = -1;

	//Translating the dir name to something we can use
	std::string dirName = fileName;
	if (dirName.rfind("/") != std::string::npos)
	//Translating the dir name to something we can use
		dirName = dirName.substr(0, dirName.rfind("/"));
	if (dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	//Translating the dir name to something we can use
	if (fileName == dirName)
		dirName = "";

	// Open the file
	std::ifstream pFile(fileName.c_str());

	// Print something if file couldn't be opened
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}


	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->materialIndex = -1;

	// Reading trough the file till the end
	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if (line == "" || line[0] == '#') //skip empty or commented line
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if (params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if (params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if (params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));
		else if (params[0] == "f")
		{
			for (size_t ii = 4; ii <= params.size(); ii++)
			{
				Face face;

				for (size_t i = ii - 3; i < ii; i++)	//magische forlus om van quads triangles te maken ;)
				{
					Vertex vertex;
					std::vector<std::string> indices = split(params[i == (ii - 3) ? 1 : i], "/");
					if (indices.size() >= 1)	//er is een positie
						vertex.position = atoi(indices[0].c_str()) - 1;
					if (indices.size() == 2)		//alleen texture
						vertex.textureCoord = atoi(indices[1].c_str()) - 1;
					if (indices.size() == 3)		//v/t/n of v//n
					{
						if (indices[1] != "")
							vertex.textureCoord = atoi(indices[1].c_str()) - 1;
						vertex.normal = atoi(indices[2].c_str()) - 1;
					}
					face.vertices.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}
		}
		else if (params[0] == "s")
		{
			//smoothing groups
		}
		else if (params[0] == "usemtl")
		{
			if (currentGroup->faces.size() != 0)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->materialIndex = -1;

			for (size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if (info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if (currentGroup->materialIndex == -1)
				std::cout << "Could not find material name " << params[1] << std::endl;
		}
	}
	groups.push_back(currentGroup);

	createVBO(normals,texcoords);
}


// Destructor
ObjectModelComponent::~ObjectModelComponent(void)
{
}

//Draw function for the ObjectModelComponent
void ObjectModelComponent::draw()
{

	for (auto vbo : vbos) {
		// Enable color and textures
		tigl::shader->enableColor(false);
		tigl::shader->enableTexture(true);

		// Bind the texture
		materials[materialIndex]->texture->bind();

		// Last we draw it!
		tigl::drawVertices(GL_TRIANGLES, vbo);
	}
}

// Method for loadign the material file, you can input a filename and directory name
void ObjectModelComponent::loadMaterialFile( const std::string &fileName, const std::string &dirName )
{
	// Print to shnow what file we are loading
	std::cout << "Loading " << fileName << std::endl;
	// Open the file
	std::ifstream pFile(fileName.c_str());
	// If file couldn't be opened, print something
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	// Creating currentmaterial
	MaterialInfo* currentMaterial = NULL;

	// Reading trough the file line by line
	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if(line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if(params[0] == "newmtl")
		{
			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
			materials.push_back(currentMaterial);
		}
		else if(params[0] == "map_kd")
		{
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);

			if (tex!=".")
			{
				currentMaterial->texture = new Texture(dirName + "/" + tex);
			}

		}
		else if (params[0] == "kd")
		{
			currentMaterial->kd = glm::vec3(atoi(params[1].c_str()), atoi(params[2].c_str()), atoi(params[3].c_str()));
		}
		else if (params[0] == "ka")
		{
			currentMaterial->ka = glm::vec3(atoi(params[1].c_str()), atoi(params[2].c_str()), atoi(params[3].c_str()));
		}
		else if (params[0] == "ks")
		{
			currentMaterial->ks = glm::vec3(atoi(params[1].c_str()), atoi(params[2].c_str()), atoi(params[3].c_str()));
		}
		else if (
			params[0] == "illum" ||
			params[0] == "map_bump" ||
			params[0] == "map_ke" ||
			params[0] == "map_ka" ||
			params[0] == "map_d" ||
			params[0] == "d" ||
			params[0] == "ke" ||
			params[0] == "ns" ||
			params[0] == "ni" ||
			params[0] == "td" ||
			params[0] == "tf" ||
			params[0] == "tr" ||
			false)
		{
			//these values are usually not used for rendering at this time, so ignore them
		}
		else
			std::cout<<"Didn't parse "<<params[0]<<" in material file"<<std::endl;
	}
}

// Getter for the vertices
std::vector<glm::vec3> ObjectModelComponent::getVertices() {
	return vertices;
}


