#include "SkinnedMesh.h"
#include "TextureLoader.h"

SkinnedMesh::SkinnedMesh(std::string _path)
{
    LoadModel(_path);
}

SkinnedMesh::~SkinnedMesh()
{
    for (auto& mesh : m_Meshes)
    {
        if (mesh)
            delete mesh;
        mesh = nullptr;
    }
    m_Meshes.clear();
}

void SkinnedMesh::Draw()
{
    for (auto& mesh : m_Meshes)
    {
        mesh->Draw();
    }
}

int& SkinnedMesh::GetBoneCounter()
{
    return m_BoneCounter;
}

std::map<std::string, BoneInfo>& SkinnedMesh::GetBoneInfoMap()
{
    return m_BoneInfoMap;
}

void SkinnedMesh::LoadModel(std::string _path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(_path, 
        aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_Directory = _path;

    ProcessNode(scene->mRootNode, scene);
}

void SkinnedMesh::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* SkinnedMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<Texture> textures;

    for (unsigned i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        if (mesh->HasPositions())
        {
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
        }

        if (mesh->HasNormals())
        {
            vertex.normals.x = -mesh->mNormals[i].x;
            vertex.normals.y = -mesh->mNormals[i].y;
            vertex.normals.z = -mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        if (mesh->HasTangentsAndBitangents())
        {
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
            vertex.Bitangent.x = mesh->mBitangents[i].x;
            vertex.Bitangent.y = mesh->mBitangents[i].y;
            vertex.Bitangent.z = mesh->mBitangents[i].z;
        }
    
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    if (mesh->HasBones())
        ExtractBoneWeightForVertices(vertices, mesh, scene);

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture> SkinnedMesh::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture = TextureLoader::LoadTexture(str.C_Str());
        textures.push_back(texture);
    }
    return textures;
}

void SkinnedMesh::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}

void SkinnedMesh::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        int boneIndex = 0;
        std::string boneName = mesh->mBones[i]->mName.data;

        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            boneIndex = m_BoneCounter++;

            BoneInfo newBoneInfo;
            newBoneInfo.id = boneIndex;
            newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = newBoneInfo;
        }
        else
        {
            boneIndex = m_BoneInfoMap[boneName].id;
        }

        aiVertexWeight* weights = mesh->mBones[i]->mWeights;
        int numWeights = mesh->mBones[i]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            SetVertexBoneData(vertices[vertexId], boneIndex, weight);
        }
    }
}

void SkinnedMesh::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
    {
        if (vertex.m_Weights[i] == 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
        }
    }
}
