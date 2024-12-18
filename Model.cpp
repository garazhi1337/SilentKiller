#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/include/stb_image.h"
#include "Model.h"
#include "Camera.h"

Model::Model()
{

}

Model::Model(string path)
{
    this->path = path;
	loadModel(path);
}

Model::~Model()
{
    delete this;
}

void Model::draw(Shader* shader, Camera* playerCamera, float screenWidth, float screenHeight)
{
    for (uint32_t i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader, playerCamera, screenWidth, screenHeight);
    }

}

void Model::loadModel(string path)
{
    if (std::filesystem::exists(path.substr(0, path.find_first_of('.')) + ".bin"))
    {
        //std::cout << "exists" << "\n";
        //десериализация сохраненной в бинарном формате модели
        deserializeModel(path.substr(0, path.find_first_of('.')) + ".bin");
    }
    else
    {
        Assimp::Importer import;
        scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        else
        {
            extractMeshData();
        }
    }

}

void Model::extractMeshData()
{

    std::ofstream out(path.substr(0, path.find_first_of('.')) + ".bin", std::ios::binary);
    size_t meshesSize = scene->mNumMeshes;
    out.write(reinterpret_cast<const char*>(&meshesSize), sizeof(size_t));

	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

        Mesh tmpMesh;
        // извлечение данных из меша
        for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];
            aiVector3D texCoord = mesh->mTextureCoords[0] != nullptr ? mesh->mTextureCoords[0][j] : aiVector3D(0, 0, 0);
            glm::vec3 glmPos = glm::vec3(position.x, position.y, position.z);
            glm::vec3 glmNorm = glm::vec3(normal.x, normal.y, normal.z);
            glm::vec2 glmTex = glm::vec2(texCoord.x, texCoord.y);
            Vertex vert = { glmPos, glmNorm, glmTex };

            tmpMesh.vertices.push_back(vert);
        }

        for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (uint32_t k = 0; k < face.mNumIndices; k++) {
                tmpMesh.indices.push_back(face.mIndices[k]);
            }
        }

        uint32_t textureId;
        glGenTextures(1, &textureId);

        int width, height, channels;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString path; // переменная для хранения пути к текстуре
        aiString rel;
        rel.Set("models/textures/");
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS) {
            rel.Append(path.C_Str());
        }
        std::string str = rel.C_Str();
        std::cout << str << " " << textureId << std::endl;

        string name = "texture_diffuse";
        unsigned char* data = stbi_load(str.c_str(), &width, &height, &channels, 0);

        if (data)
        {

            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
            switch (channels)
            {
                case 3:
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case 4:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture *quq quq*: " << stbi_failure_reason() << std::endl;
        }

        Texture texture = {textureId, name};
        //cout << texture.id << " " << texture.type << endl;
        tmpMesh.textures.push_back(texture);

        Mesh myMesh(tmpMesh.vertices, tmpMesh.indices, tmpMesh.textures);
        
        //запись данных обьекта в бинарный формат
        size_t verticesSize = tmpMesh.vertices.size();
        out.write(reinterpret_cast<const char*>(&verticesSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(tmpMesh.vertices.data()), sizeof(Vertex) * verticesSize);
        size_t indicesSize = tmpMesh.indices.size();
        out.write(reinterpret_cast<const char*>(&indicesSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(tmpMesh.indices.data()), sizeof(uint32_t) * indicesSize);
        size_t nameSize = name.size();
        out.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(name.data()), nameSize);
        
        size_t refSize = str.size();
        out.write(reinterpret_cast<const char*>(&refSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(str.data()), refSize);

        meshes.push_back(myMesh);
	}
    
    out.close();
}

void Model::deserializeModel(std::string path)
{
    std::ifstream in(path, std::ios::binary);

    size_t meshesSize;
    in.read(reinterpret_cast<char*>(&meshesSize), sizeof(size_t));

    for (uint32_t i = 0; i < (uint32_t)meshesSize; i++)
    {

        vector<Vertex> vertices;
        vector<uint32_t> indices;
        vector<Texture> textures;

        size_t verticesSize;
        in.read(reinterpret_cast<char*>(&verticesSize), sizeof(size_t));
        vertices.resize(verticesSize);
        in.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * verticesSize);

        size_t indicesSize;
        in.read(reinterpret_cast<char*>(&indicesSize), sizeof(size_t));
        indices.resize(indicesSize);
        in.read(reinterpret_cast<char*>(indices.data()), sizeof(uint32_t) * indicesSize);

        std::string name;
        size_t nameSize;
        in.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
        name.resize(nameSize);
        in.read(reinterpret_cast<char*>(name.data()), nameSize);

        std::string ref;
        size_t refSize;
        in.read(reinterpret_cast<char*>(&refSize), sizeof(size_t));
        ref.resize(refSize);
        in.read(reinterpret_cast<char*>(ref.data()), refSize);

        uint32_t textureId;
        glGenTextures(1, &textureId);
        int32_t width, height, channels;
        unsigned char* data = stbi_load(ref.c_str(), &width, &height, &channels, 0);

        if (data)
        {

            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            switch (channels)
            {
            case 3:
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                break;
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture *quq quq*: " << stbi_failure_reason() << std::endl;
        }

        Texture texture = {textureId, name};
        textures.push_back(texture);

        Mesh mesh(vertices, indices, textures);

        meshes.push_back(mesh);
    }
    in.close();
}

void Model::serializeModel(std::string path)
{

}
