#include "model.h"

glm::mat4 perspective;
glm::mat4 view;

Model::Model(std::string plik, const char* textureFile, glm::vec3 pos, float rotation, glm::vec3 scale)
	: pos(pos), rotation(rotation), scale(scale) {
	Assimp::Importer importer;
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;

	const aiScene* scene = importer.ReadFile(plik, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::cout << importer.GetErrorString() << std::endl;

	auto mesh = scene->mMeshes[0];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	this->vertices = vertices;
	this->indices = indices;
	this->texCoords = texCoords;
	this->normals = normals;
	texture = readTexture(textureFile);
}
Model::Model(Model model, glm::vec3 pos, float rotation, glm::vec3 scale)
	: vertices(model.vertices), indices(model.indices), texCoords(model.texCoords), normals(model.normals), texture(model.texture), pos(pos), rotation(rotation), scale(scale) {} 
Model::~Model() {
	glDeleteTextures(1, &texture);
}

GLuint Model::readTexture(const char* textureFile) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, textureFile);

	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void Model::render() {
	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, pos);
	transformation = glm::scale(transformation, scale);
	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));

	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za³aduj do programu cieniuj¹cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za³aduj do programu cieniuj¹cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za³aduj do programu cieniuj¹cego macierz modelu

	glEnableVertexAttribArray(shader->a("aPos"));
	glVertexAttribPointer(shader->a("aPos"), 4, GL_FLOAT, false, 0, vertices.data()); //Wspó³rzêdne wierzcho³ków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(shader->a("aNormal"));
	glVertexAttribPointer(shader->a("aNormal"), 4, GL_FLOAT, false, 0, normals.data()); //Wspó³rzêdne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(shader->a("aTexCoord"));
	glVertexAttribPointer(shader->a("aTexCoord"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wspó³rzêdne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shader->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(shader->a("aPos"));
	glDisableVertexAttribArray(shader->a("aNormal"));
	glDisableVertexAttribArray(shader->a("aTexCoord"));
}
