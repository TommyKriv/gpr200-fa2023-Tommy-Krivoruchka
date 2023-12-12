#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>

//Credit to LearnOpenGl for the guide. 

namespace patchwork 
{

	class Model 
    {
    public:
        std::vector<Texture> textures_loaded;
        Model(char* path);
        void Draw(ew::Shader& shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };

}