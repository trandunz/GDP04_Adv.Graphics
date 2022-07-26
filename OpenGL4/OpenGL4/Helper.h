// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Helper.h 
// Description : Helper Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <functional>

using KEYMAP = std::map<int, bool>;

#define Forward glm::vec3{0.0f,0.0f,-1.0f}
#define Right glm::vec3{1.0f,0.0f,0.0f}
#define Up glm::vec3{0.0f,1.0f,0.0f}
#define NUM_BONES_PER_VERTEX 16

/// <summary>
/// Ray struct for physics raycasting
/// </summary>
struct Ray
{
	glm::vec3 origin{};
	glm::vec3 direction{};
	float distance{};
};

/// <summary>
/// ShaderProgramLocation struct that encapuslates the file paths of a programs shaders.
/// </summary>
struct ShaderProgramLocation
{
    std::string vertShader = "";
	std::string geoShader = "";
	std::string tcShader = "";
	std::string teShader = "";
	std::string fragShader = "";
	std::string computeShader = "";
};

/// <summary>
/// UniformLocation struct that encapsulates the uniforms program and location.
/// </summary>
struct UniformLocation
{
    GLuint program;
    const char* location;
};

/// <summary>
/// Vertex struct that encapsulates Positional data, textureCoordinates and Normals
/// </summary>
struct Vertex
{
	glm::vec3 position{0,0,0};
	glm::vec2 texCoords{0,0};
	glm::vec3 normals{ 0,0,0 };
};

/// <summary>
/// Transform struct that encapsulates positional data such as translation, rotation and scale.
/// </summary>
struct Transform
{
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 rotation_axis = { 0,0,0 };
    glm::vec3 translation = {0,0,0};
    glm::vec3 scale = {1,1,1};
    GLfloat rotation_value = 0.0f;
};

/// <summary>
/// Texture Struct that contains the ID of the texture, 
/// Its dimentions and its filePath.
/// </summary>
struct Texture
{
	GLuint ID = 0;
	glm::vec2 Dimensions{ 0 };
	std::string FilePath = "";
};

/// <summary>
/// Font Character Struct that contains the characters texture id, size, bearing and advance
/// </summary>
struct FontChar
{
	GLuint textureID = 0; // Texture ID
	glm::ivec2 size{ 0,0 }; // Size of 'glyph'
	glm::ivec2 bearing{ 0,0 }; // Offset of 'glyph' from baseline
	GLuint advance = 0; // Distance To Next Character
};

/// <summary>
/// TEXTALIGNMENT enum to specify what sort of alignment TextLabels Should Use When Rendering
/// </summary>
enum class TEXTALIGNMENT
{
	LEFT,
	MIDDLE,
	RIGHT
};

/// <summary>
/// Updates the provided transform's model matrix with 
/// its positional, rotational and scaling values.
/// </summary>
/// <param name="_transform"></param>
/// <returns></returns>
inline glm::mat4& UpdateModelValueOfTransform(Transform& _transform)
{
	_transform.transform = glm::mat4(1);
	_transform.transform = glm::translate(_transform.transform, _transform.translation);
	if (_transform.rotation_axis.x > 0 ||
		_transform.rotation_axis.y > 0 ||
		_transform.rotation_axis.z > 0)
	{
		_transform.transform = glm::rotate(_transform.transform, _transform.rotation_value, _transform.rotation_axis);
	}
	_transform.transform = glm::scale(_transform.transform, _transform.scale);
	return _transform.transform;
}

/// <summary>
/// returns the value of pi at specified angle from sin(_angle * _xScale) / _yScale) + _offset
/// </summary>
/// <param name="_angle"></param>
/// <param name="_yScale"></param>
/// <param name="_xScale"></param>
/// <param name="_offset"></param>
/// <returns></returns>
inline float ClampedSin(float _angle, float _yScale, float _xScale, float _offset)
{
	return (sinf(_angle * _xScale) / _yScale) + _offset;
}

/// <summary>
/// Returns the Magnitude of the specified vector3
/// </summary>
/// <param name="_vector"></param>
/// <returns></returns>
inline float Magnitude(glm::vec3 _vector)
{
	return sqrtf((_vector.x * _vector.x) + (_vector.y * _vector.y) + (_vector.z * _vector.z));
}

/// <summary>
/// Return the distance between two vectors
/// </summary>
/// <param name="_v1"></param>
/// <param name="_v2"></param>
/// <returns></returns>
inline float Distance(glm::vec3 _v1, glm::vec3 _v2)
{
	return glm::abs(Magnitude(_v2 - _v1));
}


/// <summary>
/// Prints the given string withought a heap allocation to the console output stream.
/// </summary>
/// <param name="_string"></param>
inline void Print(std::string_view _string, bool _newLine = true)
{
	std::cout << _string;
	if (_newLine)
		std::cout << std::endl;
}

/// <summary>
/// Prints the Given Vector with format Vector X:~ | Y:~ | Z:~ 
/// </summary>
/// <param name="_vector"></param>
inline void Print(glm::vec3 _vector, bool _newLine = true)
{
	std::cout << "Vector x:" << std::to_string(_vector.x) << "| Y: " << std::to_string(_vector.y) << "| Z:" << std::to_string(_vector.z);
	if (_newLine)
		std::cout << std::endl;
}

/// <summary>
/// Prints the Given Vector with format Vector X:~ | Y:~ | Z:~ | W:~
/// </summary>
/// <param name="_vector"></param>
inline void Print(glm::vec4 _vector, bool _newLine = true)
{
	std::cout << "Vector x:" << std::to_string(_vector.x) << "| Y: " << std::to_string(_vector.y) << "| Z:" << std::to_string(_vector.z) << "| W:" << std::to_string(_vector.w);
	if (_newLine)
		std::cout << std::endl;
}

/// <summary>
/// Prints the given integer to the console output stream.
/// </summary>
/// <param name="_int"></param>
inline void Print(int&& _int, bool _newLine = true)
{
	std::cout << std::to_string(_int);
	if (_newLine)
		std::cout << std::endl;
}

/// <summary>
/// Prints the given float to the console output Stream.
/// </summary>
/// <param name="_float"></param>
inline void Print(float&& _float, bool _newLine = true)
{
	std::cout << std::to_string(_float);
	if (_newLine)
		std::cout << std::endl;
}

/// <summary>
/// Converts assimp matrix to glm matrix
/// </summary>
/// <param name="_matrix"></param>
/// <returns></returns>
inline glm::mat4 ConvertMatrixToGLMFormat(aiMatrix4x4 _matrix)
{
	glm::mat4 outputMatrix{};
	outputMatrix[0][0] = _matrix.a1; 
	outputMatrix[0][1] = _matrix.a2;
	outputMatrix[0][2] = _matrix.a3; 
	outputMatrix[0][3] = _matrix.a4;
	outputMatrix[1][0] = _matrix.b1; 
	outputMatrix[1][1] = _matrix.b2;
	outputMatrix[1][2] = _matrix.b3; 
	outputMatrix[1][3] = _matrix.b4;
	outputMatrix[2][0] = _matrix.c1; 
	outputMatrix[2][1] = _matrix.c2;
	outputMatrix[2][2] = _matrix.c3; 
	outputMatrix[2][3] = _matrix.c4;
	outputMatrix[3][0] = _matrix.d1; 
	outputMatrix[3][1] = _matrix.d2;
	outputMatrix[3][2] = _matrix.d3; 
	outputMatrix[3][3] = _matrix.d4;
	return outputMatrix;
}

/// <summary>
/// Converts assimp vector to glm vector
/// </summary>
/// <param name="_vector"></param>
/// <returns></returns>
inline glm::vec3 ConvertVecToGLMFormat(aiVector3D _vector)
{
	return {_vector.x, _vector.y,_vector.z};
}

/// <summary>
/// Converts assimp quaternion to glm quaternion
/// </summary>
/// <param name="_quat"></param>
/// <returns></returns>
inline glm::quat ConvertQuatToGLMFormat(aiQuaternion _quat)
{
	return { _quat.x, _quat.y,_quat.z, _quat.w };
}

/// <summary>
/// Returns a ranom float between 0 and 1
/// </summary>
/// <returns></returns>
inline float RandomFloat() 
{
	float r = (float)rand() / (float)RAND_MAX;
	return r;
}
