#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include <JLE/shader.h>

#include <string>
#include <vector>

using namespace std;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 color;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};


class Mesh {
public:
	// mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = {};

		setupMesh();
	}
	Mesh(glm::vec4 color) {
		vector<Vertex> vertices;
		for (float x = -0.5f; x <= 0.5f; x += 1.0f)
			for (float y = -0.5f; y <= 0.5f; y += 1.0f)
				for (float z = -0.5f; z <= 0.5f; z += 1.0f)
					vertices.push_back(Vertex{ glm::vec3(x, y, z), glm::vec3(x, y, z), glm::vec2(0.0f), color });

		vector<unsigned int> indices{
			//front
			0, 1, 2,
			1, 2, 3,
			//back
			4, 5, 6,
			5, 6, 7,
			//L
			0, 1, 4,
			1, 4, 5,
			//R
			2, 3, 6,
			3, 6, 7,
			//top
			1, 3, 5,
			3, 5, 7,
			//bottom
			0, 2, 4,
			2, 4, 6 };

		this->vertices = vertices;
		this->indices = indices;
		this->textures = {};

		setupMesh();
	}
	void Draw(Shader& shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);// activate the appropriate texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			shader.setInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		//draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	// render data
	unsigned int VAO, VBO, EBO;

	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		//vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		//vertex color values
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		glBindVertexArray(0);


	}
};
#endif