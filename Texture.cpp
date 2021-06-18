#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// texture class
Texture::Texture(const std::string & fileName)
{
	// glGenTextures — generate texture names
	glGenTextures(1, &id);
	// glBindTexture — bind a named texture to a texturing target
	glBindTexture(GL_TEXTURE_2D, id);
	// define variables for width height and comp
	int width, height, comp;
	stbi_set_flip_vertically_on_load(true);
	// load the texture
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &comp, 4);
	// if no data is found
	if (!data)
	{
		printf("%s -> %s\n", fileName.c_str(), stbi_failure_reason());
		exit(0);
	}
	// if data is found, texture is loaded
	printf("Loaded texture %s\n", fileName.c_str());
	// glTexImage2D — specify a two-dimensional texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexParameter, glTextureParameter — set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
}

// bind texture
void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}


// unbind texture
void Texture::unBind() {
	glDeleteTextures(1, &id);
}
