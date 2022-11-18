// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SkinnedMesh.cpp 
// Description : SkinnedMesh Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "SkinnedMesh.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

SkinnedMesh::SkinnedMesh(std::string _path)
{
	m_Scene = m_Importer.ReadFile(_path.c_str(), ASSIMP_LOAD_FLAGS);

	if (m_Scene) 
	{

		InitFromScene(m_Scene);
	}
	else 
	{
		printf("Error parsing '%s': '%s'\n", _path.c_str(), m_Importer.GetErrorString());
	}


	std::string fileExt = _path.substr(_path.length() - 3, 3);
	if (fileExt.compare("fbx") != 0) 
	{
		InitMaterials(m_Scene);
	}

	LoadBoneHierarchy();

	if (m_Scene->HasAnimations())
	{
		m_AnimStartFrame = 0;
		m_AnimEndFrame = 1;
		m_AnimFPS = (float)(m_Scene->mAnimations[0]->mChannels[0]->mNumPositionKeys / m_Scene->mAnimations[0]->mDuration);

		m_AnimStartTime = m_AnimStartFrame / m_AnimFPS;
		m_AnimEndTime = m_AnimEndFrame / m_AnimFPS;
		m_AnimTime = m_AnimStartTime;
		m_AnimSpeed = 0.1f;

		m_AnimTick = 1.0f / float(m_AnimFPS);
	}
}

SkinnedMesh::~SkinnedMesh()
{
	Clear();

	if (m_Scene)
		delete m_Scene;
	m_Scene = nullptr;
}

void SkinnedMesh::Draw(GLuint _shaderID)
{
	glBindVertexArray(m_VertexArrayID);

	for (GLuint i = 0; i < m_Entries.size(); i++) 
	{
		GLuint MaterialIndex = m_Entries[i].MaterialIndex;
		if (m_Textures[MaterialIndex])
		{
			ShaderLoader::SetUniform1i(std::move(_shaderID), "TextureCount", 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex]);
			ShaderLoader::SetUniform1i(std::move(_shaderID), "Texture0", 0);
		}

		glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	glBindVertexArray(0);
}

void SkinnedMesh::SetCurrentAnimation(int _startFrameNum, int _endFramNum, bool _loops)
{
	m_Looping = _loops;
	m_AnimStartFrame = _startFrameNum;
	m_AnimEndFrame = _endFramNum;
	m_AnimStartTime = _startFrameNum / float(m_AnimFPS);
	m_AnimEndTime = _endFramNum / float(m_AnimFPS);

	m_AnimTime = m_AnimStartTime;
}

void SkinnedMesh::SetAnimSpeed(float _newSpeed)
{
	m_AnimSpeed = _newSpeed;
}

void SkinnedMesh::LoadBoneHierarchy()
{
	if (m_Scene->HasAnimations())
	{
		const aiAnimation* pAnimation = m_Scene->mAnimations[0];

		for (size_t i = 0; i < pAnimation->mNumChannels; i++)
		{

			aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
			std::string nodeName = pNodeAnim->mNodeName.data;

			if (m_BoneNameToAiNodeAnimMap.find(nodeName) == m_BoneNameToAiNodeAnimMap.end())
			{
				m_BoneNameToAiNodeAnimMap[nodeName] = pNodeAnim;
			}
		}
	}
}

void SkinnedMesh::BoneTransforms(std::vector<Matrix4f>& _transforms)
{

	if (m_AnimTime >= m_AnimEndTime && m_Looping) 
	{
		m_AnimTime = m_AnimStartTime;
	}
	else
	{
		m_AnimTime += m_AnimTick * m_AnimSpeed;
	}

	Matrix4f Identity;
	Identity.InitIdentity();

	if (m_Scene->HasAnimations())
	{
		ReadNodeHeirarchy(m_AnimTime, m_Scene->mRootNode, Identity);
	}

	_transforms.resize(m_NumBones);

	for (GLuint i = 0; i < (unsigned)m_NumBones; i++)
	{
		_transforms[i] = m_BoneTransformInfo[i].FinalTransformation;
	}
}

int SkinnedMesh::GetStartFrame()
{
	return m_AnimStartFrame;
}

int SkinnedMesh::GetEndFrame()
{
	return m_AnimEndFrame;
}

bool SkinnedMesh::InitFromScene(const aiScene* _pScene)
{
	m_Entries.resize(_pScene->mNumMeshes);
	m_Textures.resize(_pScene->mNumMaterials);

	GLuint numVertices = 0;
	GLuint numIndices = 0;

	for (size_t i = 0; i < m_Entries.size(); i++) {

		m_Entries[i].MaterialIndex = _pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = _pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = numVertices;
		m_Entries[i].BaseIndex = numIndices;

		numVertices += _pScene->mMeshes[i]->mNumVertices;
		numIndices += m_Entries[i].NumIndices;
	}

	m_Vertices.reserve(numVertices);
	m_Indices.reserve(numIndices);

	for (size_t i = 0; i < m_Entries.size(); i++) 
	{
		const aiMesh* paiMesh = _pScene->mMeshes[i];
		InitMesh(i, paiMesh, m_Vertices, m_Indices);
	}

	InitalizeBuffers();

	return GLCheckError();
}

void SkinnedMesh::InitMesh(GLuint _meshIndex, const aiMesh* _paiMesh, std::vector<PerVertexBoneData>& _vertices, std::vector<GLuint>& _indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < _paiMesh->mNumVertices; i++)
	{

		const aiVector3D* pPos = &(_paiMesh->mVertices[i]);
		const aiVector3D* pNormals = &(_paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = _paiMesh->HasTextureCoords(0) ? &(_paiMesh->mTextureCoords[0][i]) : &Zero3D;

		PerVertexBoneData vertex{};
		vertex.Position = { pPos->x, pPos->y, pPos->z };
		vertex.Normals = { pNormals->x, pNormals->y, pNormals->z };
		vertex.TexCoords = { pTexCoord->x, pTexCoord->y };
		_vertices.push_back(vertex);
	}

	for (size_t i = 0; i < _paiMesh->mNumFaces; i++)
	{

		const aiFace& face = _paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		_indices.push_back(face.mIndices[0]);
		_indices.push_back(face.mIndices[1]);
		_indices.push_back(face.mIndices[2]);
	}

	LoadPerVertexBoneData(_meshIndex, _paiMesh, _vertices);
}

void SkinnedMesh::LoadPerVertexBoneData(int _meshIndex, const aiMesh* _pMesh, std::vector<PerVertexBoneData>& _bones)
{
	for (int i = 0; i < (int)_pMesh->mNumBones; i++)
	{
		int BoneIndex = 0;

		std::string BoneName(_pMesh->mBones[i]->mName.data);

		if (m_BoneNameToIDMap.find(BoneName) == m_BoneNameToIDMap.end()) 
		{
			BoneIndex = m_NumBones;
			m_NumBones++;

			BoneTransformationInfo bi;
			m_BoneTransformInfo.push_back(bi);

			m_BoneTransformInfo[BoneIndex].BoneOffset = _pMesh->mBones[i]->mOffsetMatrix;

			m_BoneNameToIDMap[BoneName] = BoneIndex;
		}
		else 
		{
			BoneIndex = m_BoneNameToIDMap[BoneName];
		}

		for (int j = 0; j < (int)_pMesh->mBones[i]->mNumWeights; j++)
		{

			int VertexID = m_Entries[_meshIndex].BaseVertex + _pMesh->mBones[i]->mWeights[j].mVertexId;

			float Weight = _pMesh->mBones[i]->mWeights[j].mWeight;

			_bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void SkinnedMesh::InitMaterials(const aiScene* _pScene)
{
	for (GLuint i = 0; i < _pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = _pScene->mMaterials[i];
		m_Textures[i] = NULL;
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		{
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
			{
				m_Textures[i] = TextureLoader::LoadTexture(Path.C_Str()).ID;
			}
		}
	}
}

void SkinnedMesh::Clear()
{
	if (m_VertexArrayID != 0)
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
		m_VertexArrayID = 0;
	}
	if (m_VertexBufferID != 0)
	{
		glDeleteBuffers(1, &m_VertexBufferID);
		m_VertexBufferID = 0;
	}
	if (m_IndexBufferID != 0)
	{
		glDeleteBuffers(1, &m_IndexBufferID);
		m_IndexBufferID = 0;
	}
}

void SkinnedMesh::InitalizeBuffers()
{
	// Vertex Array
	glCreateVertexArrays(1, &m_VertexArrayID);

	// Vertex Buffer
	glCreateBuffers(1, &m_VertexBufferID);
	glNamedBufferData(m_VertexBufferID, m_Vertices.size() * sizeof(PerVertexBoneData), &m_Vertices[0], GL_STATIC_DRAW);

	// Layouts
	// Position
	glEnableVertexArrayAttrib(m_VertexArrayID, 0);
	glVertexArrayAttribBinding(m_VertexArrayID, 0, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	// TexCoords
	glEnableVertexArrayAttrib(m_VertexArrayID, 1);
	glVertexArrayAttribBinding(m_VertexArrayID, 1, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 1, 2, GL_FLOAT, GL_FALSE, offsetof(PerVertexBoneData, TexCoords));
	// Normals
	glEnableVertexArrayAttrib(m_VertexArrayID, 2);
	glVertexArrayAttribBinding(m_VertexArrayID, 2, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 2, 3, GL_FLOAT, GL_FALSE, offsetof(PerVertexBoneData, Normals));
	// BoneID
	glEnableVertexArrayAttrib(m_VertexArrayID, 3);
	glVertexArrayAttribBinding(m_VertexArrayID, 3, 0);
	glVertexArrayAttribIFormat(m_VertexArrayID, 3, 4, GL_INT, offsetof(PerVertexBoneData, IDs));
	// BoneWeights
	glEnableVertexArrayAttrib(m_VertexArrayID, 4);
	glVertexArrayAttribBinding(m_VertexArrayID, 4, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 4, 4, GL_FLOAT, GL_FALSE, offsetof(PerVertexBoneData, Weights));


	// Attach Vertex Buffer To vertex array
	glVertexArrayVertexBuffer(m_VertexArrayID, 0, m_VertexBufferID, 0, sizeof(PerVertexBoneData));

	// Index Buffer
	glCreateBuffers(1, &m_IndexBufferID);
	glNamedBufferData(m_IndexBufferID, m_Indices.size() * sizeof(unsigned), &m_Indices[0], GL_STATIC_DRAW);

	// Attach index buffer to vertex array
	glVertexArrayElementBuffer(m_VertexArrayID, m_IndexBufferID);
}

GLuint SkinnedMesh::FindPosition(float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	for (size_t i = 0; i < _pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (_animationTime < (float)_pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

GLuint SkinnedMesh::FindRotation(float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	assert(_pNodeAnim->mRotationKeys > 0);

	for (size_t i = 0; _pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (_animationTime < (float)_pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

GLuint SkinnedMesh::FindScaling(float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	assert(_pNodeAnim->mNumScalingKeys > 0);

	for (size_t i = 0; i < _pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (_animationTime < (float)_pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& _out, float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	if (_pNodeAnim->mNumPositionKeys == 1)
	{
		_out = _pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	size_t positionIndex = FindPosition(_animationTime, _pNodeAnim);
	size_t NextPositionIndex = (positionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float deltaTime = (float)(_pNodeAnim->mPositionKeys[NextPositionIndex].mTime - _pNodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (_animationTime - (float)_pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = _pNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = _pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D delta = end - start;

	_out = start + factor * delta;
}

void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	if (_pNodeAnim->mNumRotationKeys == 1)
	{
		_out = _pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	size_t RotationIndex = FindRotation(_animationTime, _pNodeAnim);
	size_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < _pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(_pNodeAnim->mRotationKeys[NextRotationIndex].mTime - _pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (_animationTime - (float)_pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;

	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& StartRotationQ = _pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = _pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	aiQuaternion::Interpolate(_out, StartRotationQ, EndRotationQ, Factor);
	_out = _out.Normalize();
}

void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _pNodeAnim)
{
	if (_pNodeAnim->mNumScalingKeys == 1) 
	{
		_out = _pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	size_t ScalingIndex = FindScaling(_animationTime, _pNodeAnim);
	size_t NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < _pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(_pNodeAnim->mScalingKeys[NextScalingIndex].mTime - _pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (_animationTime - (float)_pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;

	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& Start = _pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = _pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	aiVector3D Delta = End - Start;
	_out = Start + Factor * Delta;
}

const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* _pAnimation, std::string _nodeName)
{
	if (m_BoneNameToAiNodeAnimMap.find(_nodeName) != m_BoneNameToAiNodeAnimMap.end()) 
	{
		const aiNodeAnim* pNodeAnim = m_BoneNameToAiNodeAnimMap[_nodeName];
		return pNodeAnim;
	}

	return NULL;
}

void SkinnedMesh::ReadNodeHeirarchy(float _animationTime, const aiNode* _pNode, const Matrix4f& _parentTransform)
{
	std::string NodeName(_pNode->mName.data);

	const aiAnimation* pAnimation = m_Scene->mAnimations[-1];

	Matrix4f NodeTransformation(_pNode->mTransformation);
	const aiNodeAnim* _pNodeAnim = FindNodeAnim(pAnimation, NodeName);
	if (_pNodeAnim) 
	{
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, _animationTime, _pNodeAnim);

		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, _animationTime, _pNodeAnim);

		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, _animationTime, _pNodeAnim);

		Matrix4f ScalingM; ScalingM.InitIdentity();
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4f modalAnim_parentTransform = _parentTransform * NodeTransformation;

	if (m_BoneNameToIDMap.find(NodeName) != m_BoneNameToIDMap.end()) 
	{

		m_GlobalInverseTransform = m_Scene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();  //scene inverse space
		size_t BoneIndex = m_BoneNameToIDMap[NodeName];

		m_BoneTransformInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * modalAnim_parentTransform * m_BoneTransformInfo[BoneIndex].BoneOffset;
	}

	for (size_t i = 0; i < _pNode->mNumChildren; i++) {

		ReadNodeHeirarchy(_animationTime, _pNode->mChildren[i], modalAnim_parentTransform);
	}
}
