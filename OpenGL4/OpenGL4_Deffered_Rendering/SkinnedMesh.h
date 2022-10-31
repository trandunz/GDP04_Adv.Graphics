#pragma once
#include "Mesh.h"

#define MAX_BONE_WEIGHTS 4

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class SkinnedMesh
{
public:
    SkinnedMesh(std::string _path);
    ~SkinnedMesh();
    void Draw();

    int& GetBoneCounter();
    std::map<std::string, BoneInfo>& GetBoneInfoMap();
private:
    std::vector<Mesh*> m_Meshes{};
    std::string m_Directory{};
    std::map<std::string, BoneInfo> m_BoneInfoMap{};
    int m_BoneCounter{};

    void LoadModel(std::string _path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
};

