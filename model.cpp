#include "model.h"

glm::mat4 perspective;
glm::mat4 view;
std::vector<GLuint> textures;

Model::Model() {}
Model::Model(std::string plik, unsigned int texID)
	: texID(texID) {

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
}
Model::Model(Model model, glm::vec3 pos, float rotation, glm::vec3 scale, bool hasCollission)
	: vertices(model.vertices), indices(model.indices), texCoords(model.texCoords), normals(model.normals), texID(model.texID), rotation(rotation), scale(scale) {
	rb.pos = pos;
}
Model::Model(Model model, glm::vec3 pos, Angles rotationAngle, glm::vec3 scale, bool hasCollission) 
	: vertices(model.vertices), indices(model.indices), texCoords(model.texCoords), normals(model.normals), texID(model.texID), scale(scale) {
	rb.pos = pos;
	switch (rotationAngle) {
	case Angles::d0:
		rotation = 0.0f;
		break;
	case Angles::d90:
		rotation = PI / 2;
		break;
	case Angles::d180:
		rotation = PI;
		break;
	case Angles::d270:
		rotation = -PI / 2;
		break;
	}

	if (!hasCollission) {
		br = BoundingRegion();
	}
	else {
		bool first = true;
		glm::vec3 min, max;

		/*
			Calculations for Bounding Region depending on rotation angle (magical -90 degree bias because yes)
		*/

		for (glm::vec4& vertex : vertices) {
			glm::vec3 temp = scale;

			switch (rotationAngle) {
			case Angles::d0:
				temp.x *= vertex.x; //0
				temp.y *= vertex.y;
				temp.z *= vertex.z;
				break;
			case Angles::d90:
				temp.x *= -vertex.z; // 90
				temp.y *= vertex.y;
				temp.z *= vertex.x;
				break;
			case Angles::d180:
				temp.x *= vertex.x; //0
				temp.y *= vertex.y;
				temp.z *= vertex.z;
				/*temp.x *= -vertex.z;//180
				temp.y *= vertex.y;
				temp.z *= -vertex.x;*/
				break;
			case Angles::d270:
				temp.x *= vertex.z; //-90
				temp.y *= vertex.y;
				temp.z *= -vertex.x;
				break;
			}

			if (first) {
				min = temp;
				max = temp;
				first = false;
			}

			for (int j = 0; j < 3; j++) {
				if (temp[j] < min[j]) min[j] = temp[j];
				if (temp[j] > max[j]) max[j] = temp[j];
			}
		}

		/*
			Process data for Bounding Region (add position and scale bias)
		*/

		br.max = max + pos;
		br.min = min + pos;
	}
}

void Model::render(glm::vec3 cameraPos, glm::vec3 skullPos, float dt) {
	rb.update(dt);

	glm::mat4 transformation = glm::mat4(1.0f);

	transformation = glm::translate(transformation, rb.pos);
	transformation = glm::scale(transformation, scale);

	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));


	float cameraPosition[]{0,0,0,1};
	cameraPosition[0] = cameraPos.x;
	cameraPosition[1] = cameraPos.y;
	cameraPosition[2] = cameraPos.z;

	float skullPosition[]{ 0,0,0,1 };
	skullPosition[0] = skullPos.x;
	skullPosition[1] = skullPos.y;
	skullPosition[2] = skullPos.z;

	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za�aduj do programu cieniuj�cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za�aduj do programu cieniuj�cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za�aduj do programu cieniuj�cego macierz modelu
	glUniform4fv(shader->u("cameraPos"), 1, cameraPosition);
	glUniform4fv(shader->u("skullPos"), 1, skullPosition);

	glEnableVertexAttribArray(shader->a("vertex"));
	glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wsp�rz�dne wierzcho�k�w bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(shader->a("normal"));
	glVertexAttribPointer(shader->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(shader->a("texCoord0"));
	glVertexAttribPointer(shader->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords




	glUniform1i(shader->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[texID]);


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(shader->a("vertex"));
	glDisableVertexAttribArray(shader->a("normal"));
	glDisableVertexAttribArray(shader->a("texCoords0"));
	glDisableVertexAttribArray(shader->a("camearaPos"));
}

void Model::render2(glm::vec3 cameraPos, glm::vec3 skullPos, float dt, glm::mat4 transformation) {
	rb.update(dt);

	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	transformation = glm::translate(transformation, rb.pos);
	transformation = glm::scale(transformation, scale);
	

	float cameraPosition[]{ 0,0,0,1 };
	cameraPosition[0] = cameraPos.x;
	cameraPosition[1] = cameraPos.y;
	cameraPosition[2] = cameraPos.z;

	float skullPosition[]{ 0,0,0,1};
	skullPosition[0] = skullPos.x;
	skullPosition[1] = skullPos.y;
	skullPosition[2] = skullPos.z;


	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za�aduj do programu cieniuj�cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za�aduj do programu cieniuj�cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za�aduj do programu cieniuj�cego macierz modelu
	glUniform4fv(shader->u("cameraPos"), 1, cameraPosition);
	glUniform4fv(shader->u("skullPos"), 1, skullPosition);

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

void Model::render3(glm::vec3 cameraPos, glm::vec3 skullPos, float dt, float rotation2) {
	rb.update(dt);

	glm::mat4 transformation = glm::mat4(1.0f);
	transformation = glm::translate(transformation, rb.pos);
	transformation = glm::scale(transformation, scale);

	transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	transformation = glm::rotate(transformation, rotation2, glm::vec3(1.0f, 0.0f, 0.0f));


	float cameraPosition[]{0,0,0,1};
	cameraPosition[0] = cameraPos.x;
	cameraPosition[1] = cameraPos.y;
	cameraPosition[2] = cameraPos.z;

	float skullPosition[]{ 0,0,0,1 };
	skullPosition[0] = skullPos.x;
	skullPosition[1] = skullPos.y;
	skullPosition[2] = skullPos.z;

	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspective)); //Za�aduj do programu cieniuj�cego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view)); //Za�aduj do programu cieniuj�cego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(transformation)); //Za�aduj do programu cieniuj�cego macierz modelu
	glUniform4fv(shader->u("cameraPos"), 1, cameraPosition);
	glUniform4fv(shader->u("skullPos"), 1, skullPosition);

	glEnableVertexAttribArray(shader->a("vertex"));
	glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data()); //Wsp�rz�dne wierzcho�k�w bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(shader->a("normal"));
	glVertexAttribPointer(shader->a("normal"), 4, GL_FLOAT, false, 0, normals.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords

	glEnableVertexAttribArray(shader->a("texCoord0"));
	glVertexAttribPointer(shader->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords.data()); //Wsp�rz�dne teksturowania bierz z tablicy myCubeTexCoords




	glUniform1i(shader->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[texID]);


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableVertexAttribArray(shader->a("vertex"));
	glDisableVertexAttribArray(shader->a("normal"));
	glDisableVertexAttribArray(shader->a("texCoords0"));
	glDisableVertexAttribArray(shader->a("camearaPos"));
}