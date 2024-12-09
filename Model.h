#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model
{
public:
	Model(string path, GLFWwindow* window);
	void Draw(Shader* shader);
	vector<Mesh> meshes;
private:
	// model data

	string directory;
	const aiScene* scene;
	void loadModel(string path);
	void extractMeshData();
	const char* getTexturePath(aiMesh* mesh);
};