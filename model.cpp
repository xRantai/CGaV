#include "model.h"



Model::Model() {}

Model::Model(std::string plik, const char* texture, BoundTypes boundType) 
	: boundType(boundType) {
	Assimp::Importer importer;
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;
	this->material = Material::emerald;

	const aiScene* scene = importer.ReadFile(plik, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::cout << importer.GetErrorString() << std::endl;

	auto mesh = scene->mMeshes[0];

	for (int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D vertex = mesh->mVertices[i];
		vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	this->vertices = vertices;
	this->indices = indices;
	this->texCoords = texCoords;
	this->normals = normals;
	this->texture = texture;
}
