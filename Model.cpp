#include "Model.h"
#include "Libraries/include/stb_image.h"

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
        // »звлечение данных из меша
        for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            aiVector3D normal = mesh->mNormals[j];
            aiVector3D texCoord = mesh->mTextureCoords[0] != nullptr ? mesh->mTextureCoords[0][j] : aiVector3D(0, 0, 0);
            glm::vec3 glmPos = glm::vec3(position.x, position.y, position.z);
            glm::vec3 glmNorm = glm::vec3(normal.x, normal.y, normal.z);
            glm::vec2 glmTex = glm::vec2(texCoord.x, texCoord.y);
            // ƒобавление данных в вашу структуру myMesh
            Vertex vert = { glmPos, glmNorm, glmTex };
            tmpMesh.vertices.push_back(vert);
        }

        //std::cout << mesh->mNumFaces << std::endl;

        // ќбрабатывать индексы
        for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (uint32_t k = 0; k < face.mNumIndices; k++) {
                tmpMesh.indices.push_back(face.mIndices[k]);
            }
        }

        unsigned int textureId;
        glGenTextures(1, &textureId);

        int width, height, channels;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString path; // ѕеременна€ дл€ хранени€ пути к текстуре
        aiString rel;
        rel.Set("models/textures/");
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS) {
            rel.Append(path.C_Str());
        }
        const char* str = rel.C_Str();
        cout << str << endl;

        unsigned char* data = stbi_load(str, &width, &height, &channels, 0);

        if (data != nullptr)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            switch (channels)
            {
                glBindTexture(GL_TEXTURE_2D, textureId);
                case 3:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    //cout << channels << width << height << str << endl;
                    break;
                case 4:

                    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    //cout << channels << width << height << str << endl;
                    break;
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture *quq quq*: " << stbi_failure_reason() << std::endl;
        }

        Texture texture = {textureId, "texture_diffuse"};
        //cout << texture.id << " " << texture.type << endl;
        tmpMesh.textures.push_back(texture);

        Mesh myMesh(tmpMesh.vertices, tmpMesh.indices, tmpMesh.textures);
        meshes.push_back(myMesh);
	}
}

const char* Model::getTexturePath(aiMesh* mesh)
{
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiString path; // ѕеременна€ дл€ хранени€ пути к текстуре
    aiString rel;
    rel.Set("models\\textures\\");
    //std::cout << (material->GetName()).C_Str() << "\n";
    
    // ѕолучаем текстуру типа DIFFUSE (можно вз€ть другие типы)
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS) {
        // записывает в path путь до текстуры (само название текстуры дл€ того чтобы потом его в стбиимаждолере загрузить)
        rel.Append(path.C_Str());
    }
    return rel.C_Str();
}

