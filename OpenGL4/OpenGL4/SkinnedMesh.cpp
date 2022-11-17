#include "SkinnedMesh.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

SkinnedMesh::SkinnedMesh(std::string _path)
{
	m_Scene = m_Importer.ReadFile(_path.c_str(), ASSIMP_LOAD_FLAGS);

	if (m_Scene) 
	{

		InitFromScene(m_Scene, _path);
	}
	else 
	{
		printf("Error parsing '%s': '%s'\n", _path.c_str(), m_Importer.GetErrorString());
	}


	std::string fileExt = _path.substr(_path.length() - 3, 3);
	if (fileExt.compare("fbx") != 0) 
	{
		InitMaterials(m_Scene, _path);
	}

	LoadBoneHierarchy();

	if (m_Scene->HasAnimations())
	{
		m_AnimStartFrame = 0;
		m_AnimEndFrame = 1;
		m_AnimFPS = (int)(m_Scene->mAnimations[0]->mChannels[0]->mNumPositionKeys / m_Scene->mAnimations[0]->mDuration);

		m_AnimStartTime = m_AnimStartFrame / float(m_AnimFPS);
		m_AnimEndTime = m_AnimEndFrame / float(m_AnimFPS);
		m_AnimTime = m_AnimStartTime;
		m_AnimSpeed = 0.1f;

		m_AnimTick = 1.0f / float(m_AnimFPS) * m_AnimSpeed;
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

void SkinnedMesh::SetCurrentAnimation(int startFrameNum, int endFramNum, bool _loops)
{
	m_Looping = _loops;
	m_AnimStartFrame = startFrameNum;
	m_AnimEndFrame = endFramNum;
	m_AnimStartTime = startFrameNum / float(m_AnimFPS);
	m_AnimEndTime = endFramNum / float(m_AnimFPS);

	m_AnimTime = m_AnimStartTime;
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

void SkinnedMesh::BoneTransforms(float timeInSeconds, std::vector<Matrix4f>& transforms)
{
	if (m_AnimTime >= m_AnimEndTime && m_Looping) 
	{
		m_AnimTime = m_AnimStartTime;
	}
	else
	{
		m_AnimTime += m_AnimTick;
	}

	Matrix4f Identity;
	Identity.InitIdentity();

	if (m_Scene->HasAnimations())
	{
		ReadNodeHeirarchy(m_AnimTime, m_Scene->mRootNode, Identity);
	}

	transforms.resize(m_NumBones);

	for (GLuint i = 0; i < (unsigned)m_NumBones; i++)
	{
		transforms[i] = m_BoneTransformInfo[i].FinalTransformation;
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

bool SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	GLuint numVertices = 0;
	GLuint numIndices = 0;

	for (size_t i = 0; i < m_Entries.size(); i++) {

		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = numVertices;
		m_Entries[i].BaseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_Entries[i].NumIndices;
	}

	m_Vertices.reserve(numVertices);
	m_Indices.reserve(numIndices);

	for (size_t i = 0; i < m_Entries.size(); i++) 
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, m_Vertices, m_Indices);
	}

	InitalizeBuffers();

	return GLCheckError();
}

void SkinnedMesh::InitMesh(GLuint meshIndex, const aiMesh* paiMesh, std::vector<PerVertexBoneData>& _vertices, std::vector<GLuint>& indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < paiMesh->mNumVertices; i++) 
	{

		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormals = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		PerVertexBoneData vertex{};
		vertex.position = { pPos->x, pPos->y, pPos->z };
		vertex.normals = { pNormals->x, pNormals->y, pNormals->z };
		vertex.texCoords = { pTexCoord->x, pTexCoord->y };
		_vertices.push_back(vertex);
	}

	for (size_t i = 0; i < paiMesh->mNumFaces; i++) 
	{

		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	LoadPerVertexBoneData(meshIndex, paiMesh, _vertices);
}

void SkinnedMesh::LoadPerVertexBoneData(int meshIndex, const aiMesh* pMesh, std::vector<PerVertexBoneData>& bones)
{
	for (int i = 0; i < (int)pMesh->mNumBones; i++)
	{
		int BoneIndex = 0;

		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneNameToIDMap.find(BoneName) == m_BoneNameToIDMap.end()) 
		{
			BoneIndex = m_NumBones;
			m_NumBones++;

			BoneTransformationInfo bi;
			m_BoneTransformInfo.push_back(bi);

			m_BoneTransformInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

			m_BoneNameToIDMap[BoneName] = BoneIndex;
		}
		else 
		{
			BoneIndex = m_BoneNameToIDMap[BoneName];
		}

		for (int j = 0; j < (int)pMesh->mBones[i]->mNumWeights; j++)
		{

			int VertexID = m_Entries[meshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;

			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;

			bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void SkinnedMesh::InitMaterials(const aiScene* pScene, const std::string filename)
{
	for (GLuint i = 0; i < pScene->mNumMaterials; i++) 
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];
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
	glVertexArrayAttribFormat(m_VertexArrayID, 1, 2, GL_FLOAT, GL_FALSE, offsetof(PerVertexBoneData, texCoords));
	// Normals
	glEnableVertexArrayAttrib(m_VertexArrayID, 2);
	glVertexArrayAttribBinding(m_VertexArrayID, 2, 0);
	glVertexArrayAttribFormat(m_VertexArrayID, 2, 3, GL_FLOAT, GL_FALSE, offsetof(PerVertexBoneData, normals));
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

GLuint SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (size_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	return 0;
}

GLuint SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mRotationKeys > 0);

	for (size_t i = 0; pNodeAnim->mNumRotationKeys - 1; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	return 0;
}

GLuint SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (size_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	return 0;
}

void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) 
	{
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	size_t positionIndex = FindPosition(AnimationTime, pNodeAnim);
	size_t NextPositionIndex = (positionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float deltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D delta = end - start;

	out = start + factor * delta;
}

void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) 
	{
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	size_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	size_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;

	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();
}

void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) 
	{
		out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	size_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	size_t NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;

	assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	aiVector3D Delta = End - Start;
	out = Start + Factor * Delta;
}

const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName)
{
	if (m_BoneNameToAiNodeAnimMap.find(NodeName) != m_BoneNameToAiNodeAnimMap.end()) 
	{
		const aiNodeAnim* pNodeAnim = m_BoneNameToAiNodeAnimMap[NodeName];
		return pNodeAnim;
	}

	return NULL;
}

void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_Scene->mAnimations[-1];

	Matrix4f NodeTransformation(pNode->mTransformation);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
	if (pNodeAnim) 
	{

		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);

		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);

		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);

		Matrix4f ScalingM; ScalingM.InitIdentity();
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4f modalAnimParentTransform = ParentTransform * NodeTransformation;

	if (m_BoneNameToIDMap.find(NodeName) != m_BoneNameToIDMap.end()) 
	{

		m_GlobalInverseTransform = m_Scene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();  //scene inverse space
		size_t BoneIndex = m_BoneNameToIDMap[NodeName];

		m_BoneTransformInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * modalAnimParentTransform * m_BoneTransformInfo[BoneIndex].BoneOffset;
	}

	for (size_t i = 0; i < pNode->mNumChildren; i++) {

		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], modalAnimParentTransform);
	}
}
