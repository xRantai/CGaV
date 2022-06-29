#include "model.h"

glm::mat4 perspective;
glm::mat4 view;
std::vector<GLuint> textures;

Model::Model(std::string plik, unsigned int texID, BoundTypes boundType, glm::vec3 pos, float rotation, glm::vec3 scale)
	: rotation(rotation), scale(scale), texID(texID) {
	br.type = boundType;
	glm::vec3 min((float)(~0));			// initial set to largest integer
	glm::vec3 max(-(float)(~0));		// initial set to smallest integer

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

		for (int j = 0; j < 3; j++) {//calculations for Bounding Region
			if (vertex[j] < min[j]) min[j] = vertex[j];
			if (vertex[j] > max[j]) max[j] = vertex[j];
		}

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

	/*
		Process data for Bounding Region
	*/

	if (boundType == BoundTypes::AABB) {
		br.max = max;
		br.min = min;
	}
	else {
		br.center = BoundingRegion(min, max).calculateCenter();
		float MaxRadiusSquared = 0.0f;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			float RadiusSquared = 0.0f;
			for (int j = 0; j < 3; j++) {
				RadiusSquared += (vertices[i][j] - br.center[j]) * (vertices[i][j] - br.center[j]);
			}
			if (RadiusSquared > MaxRadiusSquared) {
				MaxRadiusSquared = RadiusSquared;
			}
		}
		br.radius = sqrt(MaxRadiusSquared);
	}

	this->vertices = vertices;
	this->indices = indices;
	this->texCoords = texCoords;
	this->normals = normals;
	rb.pos = pos;
}
Model::Model(Model model, glm::vec3 pos, float rotation, glm::vec3 scale, bool hasCollission)
	: vertices(model.vertices), indices(model.indices), texCoords(model.texCoords), normals(model.normals), texID(model.texID), rotation(rotation), scale(scale) {
	rb.pos = pos;
} 

void Model::render(float dt) {
	rb.update(dt);

void Model::render(glm::vec3 cameraPos, float dt) {
	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, rb.pos);
	transformation = glm::scale(transformation, scale);

	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));


	float cameraPosition[]{0,0,0,1};
	cameraPosition[0] = cameraPos.x;
	cameraPosition[1] = cameraPos.y;
	cameraPosition[2] = cameraPos.z;


	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za�aduj do programu cieniuj�cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za�aduj do programu cieniuj�cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za�aduj do programu cieniuj�cego macierz modelu
	glUniform4fv(shader->u("cameraPos"), 1, cameraPosition);

	glEnableVertexAttribArray(shader->a("vertex"));
	glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wsp�rz�dne wierzcho�k�w bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(shader->a("normal"));
	glVertexAttribPointer(shader->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(shader->a("texCoord0"));
	glVertexAttribPointer(shader->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords




	glUniform1i(shader->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[texID - 1]);


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(shader->a("vertex"));
	glDisableVertexAttribArray(shader->a("normal"));
	glDisableVertexAttribArray(shader->a("texCoords0"));
	glDisableVertexAttribArray(shader->a("camearaPos"));
}

void Model::render2(glm::vec3 cameraPos, glm::mat4 transformation) {
	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	transformation = glm::translate(transformation, pos);
	transformation = glm::scale(transformation, scale);
	

	float cameraPosition[]{ 0,0,0,1 };
	cameraPosition[0] = cameraPos.x;
	cameraPosition[1] = cameraPos.y;
	cameraPosition[2] = cameraPos.z;


	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za�aduj do programu cieniuj�cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za�aduj do programu cieniuj�cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za�aduj do programu cieniuj�cego macierz modelu

	glEnableVertexAttribArray(shader->a("vertex"));
	glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wsp�rz�dne wierzcho�k�w bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(shader->a("normal"));
	glVertexAttribPointer(shader->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(shader->a("texCoord0"));
	glVertexAttribPointer(shader->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords


	glEnableVertexAttribArray(shader->a("cameraPos"));
	glVertexAttribPointer(shader->a("cameraPos"), 3, GL_FLOAT, false, 0, cameraPosition);


	glUniform1i(shader->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[texID]);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(shader->a("vertex"));
	glDisableVertexAttribArray(shader->a("normal"));
	glDisableVertexAttribArray(shader->a("texCoords0"));
	glDisableVertexAttribArray(shader->a("camearaPos"));
}