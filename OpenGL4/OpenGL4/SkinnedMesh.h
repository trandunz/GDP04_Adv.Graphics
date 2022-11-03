#pragma once
#include "Mesh.h"
#include "Matrix4f.h"

#define INVALID_MATERIAL 0xFFFFFFFF
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded | aiProcess_GenSmoothNormals )
#define GLCheckError() (glGetError() == GL_NO_ERROR)

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

struct MeshEntry 
{

    GLuint NumIndices;
    GLuint BaseVertex;
    GLuint BaseIndex;
    GLuint MaterialIndex;

    MeshEntry() 
    {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
        MaterialIndex = INVALID_MATERIAL;
    }
};

enum VB_TYPES 
{
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs
};

struct BoneTransformationInfo 
{

    Matrix4f BoneOffset;
    Matrix4f FinalTransformation;

    BoneTransformationInfo() 
    {
        BoneOffset.SetZero();
        FinalTransformation.SetZero();
    }
};

struct PerVertexBoneData 
{
    glm::vec3 position{ 0,0,0 };
    glm::vec2 texCoords{ 0,0 };
    glm::vec3 normals{ 0,0,0 };
    int IDs[NUM_BONES_PER_VERTEX]{};
    float Weights[NUM_BONES_PER_VERTEX]{};

    void Reset() 
    {
        memset(IDs ,0, sizeof(IDs));
        memset(Weights, 0, sizeof(Weights));
    }

    void AddBoneData(int BoneID, float Weight) 
    {
        for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) 
        {
            if (Weights[i] == 0.0f) 
            {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            }
        }
    }
};

class SkinnedMesh
{
public:
    SkinnedMesh(std::string _path);
    ~SkinnedMesh();
    void Draw(GLuint _shaderID);

    void SetCurrentAnimation(int startFrameNum, int endFramNum);

    void BoneTransforms(float timeInSeconds, std::vector<Matrix4f>& transforms);

    GLuint BoneLocation[100]{};
private:
    int m_NumBones{};
    GLuint m_VertexArrayID{};
    GLuint m_VertexBufferID{};
    GLuint m_IndexBufferID{};
    std::vector<MeshEntry> m_Entries{};
    std::vector<GLuint> m_Textures{};
    std::vector< PerVertexBoneData> m_Vertices{};
    std::vector<GLuint> m_Indices{};
    std::map<std::string, aiNodeAnim*> m_BoneNameToAiNodeAnimMap{};
    std::map<std::string, GLuint> m_BoneNameToIDMap{};
    std::vector<BoneTransformationInfo> m_BoneTransformInfo{};
    Matrix4f m_GlobalInverseTransform{};
    const aiScene* m_Scene{nullptr};
    Assimp::Importer m_Importer{};
    GLuint m_NumMaterials{};

    float m_AnimTick{};
    int m_AnimStartFrame{};
    int m_AnimEndFrame{};
    int m_AnimCurrentFrame{};
    float m_AnimSpeed{};
    int m_AnimFPS{};
    float m_AnimStartTime{};
    float m_AnimEndTime{};
    float m_AnimTime{};

    void LoadBoneHierarchy();

    bool InitFromScene(const aiScene* pScene, const std::string Filename);
    void InitMesh(GLuint meshIndex,const aiMesh* paiMesh,std::vector<PerVertexBoneData>& _vertices,std::vector<GLuint>& indices);

    void LoadPerVertexBoneData(int meshIndex, const aiMesh* pMesh, std::vector<PerVertexBoneData>& bones);
    void InitMaterials(const aiScene* pScene, const std::string filename);
    void Clear();

    void InitalizeBuffers();

    GLuint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    GLuint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    GLuint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

    void CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);

    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName);

    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
};

