#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

using namespace std;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	uint32_t id;
	string type;
};

class Mesh {
public:
	// mesh data
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Texture> textures;
	Mesh(vector<Vertex> vertices, vector<uint32_t> indices, vector<Texture> textures);
	Mesh();
	void Draw(Shader* shader);
	uint32_t VAO, VBO, EBO;
private:
	// render data

	void setupMesh();
};
