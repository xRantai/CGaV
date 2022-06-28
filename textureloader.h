#ifndef Textures_H
#define Textures_H

#include "lodepng.h"
#include <model.h>
#include <GL/glew.h>

static unsigned int id;

class TextureLoader {
public:
	TextureLoader();

	GLuint load(const char* textureFile);

	unsigned int getCurrentID();

	void destroyTextures();
};

#endif