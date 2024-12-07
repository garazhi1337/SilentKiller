#include "Model.h"

Model::Model(string path)
{
	loadModel(path);

}

void Model::Draw(Shader& shader)
{
    for (uint32_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }

}

void Model::loadModel(string path)
{
	Assimp::Importer import;
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
    }
    else
    {
        extractMeshData();
    }
}

void Model::extractMeshData()
{
	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
        Mesh tmpMesh;

        // ���������� ������ �� ����
        for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];
            aiVector3D texCoord = mesh->mTextureCoords[0] != nullptr ? mesh->mTextureCoords[0][j] : aiVector3D(0, 0, 0);
            glm::vec3 glmPos = glm::vec3(position.x, position.y, position.z);
            glm::vec3 glmNorm = glm::vec3(normal.x, normal.y, normal.z);
            glm::vec2 glmTex = glm::vec2(texCoord.x, texCoord.y);
            // ���������� ������ � ���� ��������� myMesh
            tmpMesh.vertices.push_back({glmPos, glmNorm, glmTex});
        }

        //std::cout << mesh->mNumFaces << std::endl;

        // ������������ �������
        for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (uint32_t k = 0; k < face.mNumIndices; k++) {
                tmpMesh.indices.push_back(face.mIndices[k]);
            }
        }

        Mesh myMesh(tmpMesh.vertices, tmpMesh.indices, tmpMesh.textures);
        meshes.push_back(myMesh);
	}
}