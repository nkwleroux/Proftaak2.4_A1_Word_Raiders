#pragma once

#include <gl/glew.h>
#include <string>

class Texture
{
	GLuint id;
public:
	// Constructor
	Texture(const std::string &fileName);
	// Method for binding texture
	void bind();
	// Method for unbinding the texture
	void unBind();
};