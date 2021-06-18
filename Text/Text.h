#pragma once
#include <xstring>
#include <GL/glew.h>
#include <glm/vec4.hpp>
#include "stb_truetype.h"

class Text
{
public:
	// Constructor
	Text(std::string path, float size);
	// Destructor
	~Text();
	// Init function for the text class
	void init();
	// Draw function so we can draw the text
	void draw(const std::string& text, float x, float y, glm::vec4 color);
private:
	// Text ID
	GLuint texId;
	// Char data
	stbtt_bakedchar cdata[96];
};

