#pragma once
#include <xstring>
#include <GL/glew.h>
#include <glm/vec4.hpp>

#include "stb_truetype.h"

class Text
{
public:
	Text(std::string path, float size);
	~Text();
	void init();
	void draw(const std::string& text, float x, float y, glm::vec4 color);
private:
	GLuint texId;
	stbtt_bakedchar cdata[96];
};

