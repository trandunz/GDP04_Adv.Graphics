// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SkinnedMesh.h 
// Description : SkinnedMesh Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Mesh.h"
#include "Matrix4f.h"

#define INVALID_MATERIAL 0xFFFFFFFF
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded)
#define GLCheckError() (glGetError() == GL_NO_ERROR)

/// <summary>
/// Struct for holding the bone info
/// </summary>
struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

/// <summary>
/// Struct for holding a mesh entry
/// </summary>
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

/// <summary>
/// Enum for holding buffer layout positons
/// </summary>
enum VB_TYPES 
{
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs
};

/// <summary>
/// Struct for holding bone transformation info
/// </summary>
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

/// <summary>
/// Struct for holding per vertex bone data
/// </summary>
struct PerVertexBoneData 
{
    glm::vec3 Position{ 0,0,0 };
    glm::vec2 TexCoords{ 0,0 };
    glm::vec3 Normals{ 0,0,0 };
    int IDs[NUM_BONES_PER_VERTEX]{};
    float Weights[NUM_BONES_PER_VERTEX]{};

    void Reset() 
    {
        memset(IDs ,0, sizeof(IDs));
        memset(Weights, 0, sizeof(Weights));
    }

    void AddBoneData(int _boneID, float _weight) 
    {
        for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) 
        {
            if (Weights[i] == 0.0f) 
            {
                IDs[i] = _boneID;
                Weights[i] = _weight;
                return;
            }
        }
    }
};

class SkinnedMesh
{
public:
    /// <summary>
    /// Skinned mesh constructr 
    /// </summary>
    /// <param name="_path"></param>
    SkinnedMesh(std::string _path);
    /// <summary>
    /// Skinned mesh destructor 
    /// </summary>
    ~SkinnedMesh();

    /// <summary>
    /// Draws the skinned mesh
    /// </summary>
    /// <param name="_shaderID"></param>
    void Draw(GLuint _shaderID);

    /// <summary>
    /// Sets the current animation based on start and end frame
    /// </summary>
    /// <param name="startFrameNum"></param>
    /// <param name="endFramNum"></param>
    /// <param name="_loops"></param>
    void SetCurrentAnimation(int _startFrameNum, int _endFramNum, bool _loops = true);
    /// <summary>
    /// Sets the speed of the animation
    /// </summary>
    /// <param name="_newSpeed"></param>
    void SetAnimSpeed(float _newSpeed);

    /// <summary>
    /// Returns the bone transforms
    /// </summary>
    /// <param name="timeInSeconds"></param>
    /// <param name="transforms"></param>
    void BoneTransforms(std::vector<Matrix4f>& _transforms);

    /// <summary>
    /// Returns the start frame
    /// </summary>
    /// <returns></returns>
    int GetStartFrame();
    /// <summary>
    /// Returns the end frame
    /// </summary>
    /// <returns></returns>
    int GetEndFrame();

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
    float m_AnimFPS{};
    float m_AnimStartTime{};
    float m_AnimEndTime{};
    float m_AnimTime{};
    bool m_Looping{ true };

    /// <summary>
    /// Loads the bone hierarchy
    /// </summary>
    void LoadBoneHierarchy();

    /// <summary>
    /// Initializes the mesh from the scene object
    /// </summary>
    /// <param name="pScene"></param>
    /// <param name="Filename"></param>
    /// <returns></returns>
    bool InitFromScene(const aiScene* _pScene);

    /// <summary>
    /// Initializes the mesh from vertices and indices
    /// </summary>
    /// <param name="meshIndex"></param>
    /// <param name="paiMesh"></param>
    /// <param name="_vertices"></param>
    /// <param name="indices"></param>
    void InitMesh(GLuint _meshIndex,const aiMesh* _paiMesh,std::vector<PerVertexBoneData>& _vertices,std::vector<GLuint>& _indices);

    /// <summary>
    /// Loads the per vertex bone data
    /// </summary>
    /// <param name="meshIndex"></param>
    /// <param name="pMesh"></param>
    /// <param name="bones"></param>
    void LoadPerVertexBoneData(int _meshIndex, const aiMesh* _pMesh, std::vector<PerVertexBoneData>& _bones);
    /// <summary>
    /// Initializes any materials stored in the scene
    /// </summary>
    /// <param name="pScene"></param>
    /// <param name="filename"></param>
    void InitMaterials(const aiScene* _pScene);
    /// <summary>
    /// Cleans up all the buffers
    /// </summary>
    void Clear();

    /// <summary>
    /// Initializes all the buffers
    /// </summary>
    void InitalizeBuffers();

    /// <summary>
    /// Finds the position at the specified animation time
    /// </summary>
    /// <param name="AnimationTime"></param>
    /// <param name="pNodeAnim"></param>
    /// <returns></returns>
    GLuint FindPosition(float _animationTime, const aiNodeAnim* _pNodeAnim);
    /// <summary>
    /// Find the rotation at the specified animation time
    /// </summary>
    /// <param name="_animationTime"></param>
    /// <param name="_pNodeAnim"></param>
    /// <returns></returns>
    GLuint FindRotation(float _animationTime, const aiNodeAnim* _pNodeAnim);
    /// <summary>
    /// Find the scaling at the specified animation time
    /// </summary>
    /// <param name="_animationTime"></param>
    /// <param name="_pNodeAnim"></param>
    /// <returns></returns>
    GLuint FindScaling(float _animationTime, const aiNodeAnim* _pNodeAnim);

    /// <summary>
    /// Interpolates the positon at the current animation time
    /// </summary>
    /// <param name="_out"></param>
    /// <param name="_animationTime"></param>
    /// <param name="_pNodeAnim"></param>
    void CalcInterpolatedPosition(aiVector3D& _out, float _animationTime, const aiNodeAnim* _pNodeAnim);
    /// <summary>
    /// Interpolates the rotation at the current animation time
    /// </summary>
    /// <param name="_out"></param>
    /// <param name="_animationTime"></param>
    /// <param name="_pNodeAnim"></param>
    void CalcInterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _pNodeAnim);
    /// <summary>
    /// Interpolates the scale at the current animation time
    /// </summary>
    /// <param name="_out"></param>
    /// <param name="_animationTime"></param>
    /// <param name="_pNodeAnim"></param>
    void CalcInterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _pNodeAnim);

    /// <summary>
    /// Finds the animaton at the specified node
    /// </summary>
    /// <param name="_pAnimation"></param>
    /// <param name="_nodeName"></param>
    /// <returns></returns>
    const aiNodeAnim* FindNodeAnim(const aiAnimation* _pAnimation, std::string _nodeName);

    /// <summary>
    /// Reads the node heirarchy
    /// </summary>
    /// <param name="_animationTime"></param>
    /// <param name="_pNode"></param>
    /// <param name="_parentTransform"></param>
    void ReadNodeHeirarchy(float _animationTime, const aiNode* _pNode, const Matrix4f& _parentTransform);
};

