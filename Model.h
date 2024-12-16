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
	Model();
	Model(string path);
	~Model();
	void draw(Shader* shader, Camera* playerCamera, float screenWidth, float screenHeight);
	vector<Mesh> meshes;
private:
	const aiScene* scene;
	void loadModel(string path);
	void extractMeshData();
	Model* deserializeModel();
	void serializeModel();
	string path;
};