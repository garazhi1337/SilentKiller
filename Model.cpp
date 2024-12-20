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
        //�������������� ����������� � �������� ������� ������
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
        // ���������� ������ �� ����
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

        std::vector<std::string> names;
        std::vector<std::string> paths;
        bool useSpecular = false;

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        uint32_t diffuseCount = material->GetTextureCount(aiTextureType_DIFFUSE);
        std::cout << diffuseCount << "\n";
        if (diffuseCount > 0)
        {
            for (uint32_t j = 0; j < diffuseCount; j++)
            {
                uint32_t textureId;
                glGenTextures(1, &textureId);

                int width, height, channels;

                aiString path; // ���������� ��� �������� �������������� ���� � ��������
                aiString rel;

                rel.Set("models/textures/");
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS) {
                    rel.Append(path.C_Str());
                }
                std::string str = rel.C_Str();
                paths.push_back(str);
                std::cout << str << " " << textureId << std::endl;

                string name = "texture_diffuse";
                names.push_back(name);
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

                    Texture texture = { textureId, name };
                    tmpMesh.textures.push_back(texture);
                }
                else
                {
                    std::cout << "Failed to load texture *quq quq*: " << stbi_failure_reason() << std::endl;
                }
            }
        }

        uint32_t specularCount = material->GetTextureCount(aiTextureType_SPECULAR);
        std::cout << specularCount << "\n";
        if (specularCount > 0)
        {
            useSpecular = true;
            for (uint32_t j = 0; j < specularCount; j++)
            {
                uint32_t textureId;
                glGenTextures(1, &textureId);

                int width, height, channels;

                aiString path; // ���������� ��� �������� �������������� ���� � ��������
                aiString rel;

                rel.Set("models/textures/");
                if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == aiReturn_SUCCESS) {
                    rel.Append(path.C_Str());
                }
                std::string str = rel.C_Str();
                paths.push_back(str);
                std::cout << str << " " << textureId << std::endl;

                string name = "texture_specular";
                names.push_back(name);
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

                    Texture texture = { textureId, name };
                    tmpMesh.textures.push_back(texture);
                }
                else
                {
                    std::cout << "Failed to load texture *quq quq*: " << stbi_failure_reason() << std::endl;
                }
            }
        }

        tmpMesh.useSpecular = useSpecular;


        Mesh myMesh(tmpMesh.vertices, tmpMesh.indices, tmpMesh.textures, tmpMesh.useSpecular);
        
        //������ ������ ������� � �������� ������

        size_t verticesSize = tmpMesh.vertices.size();
        out.write(reinterpret_cast<const char*>(&verticesSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(tmpMesh.vertices.data()), sizeof(Vertex) * verticesSize);

        size_t indicesSize = tmpMesh.indices.size();
        out.write(reinterpret_cast<const char*>(&indicesSize), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(tmpMesh.indices.data()), sizeof(uint32_t) * indicesSize);

        size_t namesSize = names.size();
        out.write(reinterpret_cast<const char*>(&namesSize), sizeof(size_t));
        for (std::string name : names)
        {
            size_t nameSize = name.size();
            out.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
            out.write(reinterpret_cast<const char*>(name.data()), nameSize);
        }
        
        size_t pathsSize = paths.size();
        out.write(reinterpret_cast<const char*>(&pathsSize), sizeof(size_t));
        for (std::string path : paths)
        {
            size_t pathSize = path.size();
            out.write(reinterpret_cast<const char*>(&pathSize), sizeof(size_t));
            out.write(reinterpret_cast<const char*>(path.data()), pathSize);
        }

        out.write(reinterpret_cast<const char*>(&useSpecular), sizeof(bool));

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
        vector<std::string> names;
        vector<std::string> paths;
        bool useSpecular;

        size_t verticesSize;
        in.read(reinterpret_cast<char*>(&verticesSize), sizeof(size_t));
        vertices.resize(verticesSize);
        in.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * verticesSize);

        size_t indicesSize;
        in.read(reinterpret_cast<char*>(&indicesSize), sizeof(size_t));
        indices.resize(indicesSize);
        in.read(reinterpret_cast<char*>(indices.data()), sizeof(uint32_t) * indicesSize);

        size_t namesSize;
        in.read(reinterpret_cast<char*>(&namesSize), sizeof(size_t));
        names.resize(namesSize);
        ;
        for (std::string& name : names)
        {
            size_t nameSize;
            in.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
            name.resize(nameSize);
            in.read(name.data(), nameSize);
        }

        size_t pathsSize;
        in.read(reinterpret_cast<char*>(&pathsSize), sizeof(size_t));
        paths.resize(pathsSize);
        for (std::string& path : paths)
        {
            size_t pathSize;
            in.read(reinterpret_cast<char*>(&pathSize), sizeof(size_t));
            path.resize(pathSize);
            in.read(path.data(), pathSize);
        }

        in.read(reinterpret_cast<char*>(&useSpecular), sizeof(bool));

        for (uint32_t j = 0; j < paths.size(); j++)
        {
            uint32_t textureId;
            glGenTextures(1, &textureId);
            int32_t width, height, channels;
            unsigned char* data = stbi_load(paths[j].c_str(), &width, &height, &channels, 0);

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

            Texture texture = { textureId, names[j]};
            textures.push_back(texture);
        }

        Mesh mesh(vertices, indices, textures, useSpecular);

        meshes.push_back(mesh);
    }
    in.close();
}

void Model::serializeModel(std::string path)
{

}
