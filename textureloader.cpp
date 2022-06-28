#include "textureloader.h"

TextureLoader::TextureLoader() {
	id = 0;
}

GLuint TextureLoader::load(const char* textureFile) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0 + id); // czytanie na id jednostk� teksturuj�c�
	id++;

	//Wczytanie do pami�ci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do kt�rych wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, textureFile);

	//Import do pami�ci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pami�ci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);

	return tex;
}

unsigned int TextureLoader::getCurrentID() {
	return id;
}

void TextureLoader::destroyTextures() {
	for (int i = 0; i < textures.size(); i++) {
		glDeleteTextures(1, &textures[i]);
	}
	id = 0;
}