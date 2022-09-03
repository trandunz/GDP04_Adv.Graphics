// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Noise.h 
// Description : Noise Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz
// Credit To : Ken Perlin

#pragma once
#include <time.h>
#include <cstdlib>
#include <string>
#define PI 3.14159265358979323846264338327950288

class Noise
{
public:
	/// <summary>
	/// Sets the seed of the noise. If nothing is given, seed will be time(NULL)
	/// </summary>
	/// <param name="_newSeed"></param>
	static void SeedRand(int _newSeed = (int)time(NULL));
	/// <summary>
	/// Returns a random noise value for the position _x, _y
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	static float Random(int _x, int _y);

	/// <summary>
	/// Smoothens the value by averaging the corners, sides and center
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	static float Smooth(float _x, float _y);

	/// <summary>
	/// Interpolates between the values _y1 and _y2 by ratio _mu using Cosine
	/// </summary>
	/// <param name="_y1"></param>
	/// <param name="_y2"></param>
	/// <param name="_mu"></param>
	/// <returns></returns>
	static double CosineInterpolate(double _y1, double _y2, double _mu);

	/// <summary>
	/// Linearly Interpolates between the values _y1 and _y2 by ratio _mu
	/// </summary>
	/// <param name="_y1"></param>
	/// <param name="_y2"></param>
	/// <param name="_mu"></param>
	/// <returns></returns>
	static double LinearInterpolate(double _y1, double _y2, double _mu);

	/// <summary>
	/// Interpolates and smooths the value at position __x, _y
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	static float SmoothInterpolate(float _x, float _y);

	/// <summary>
	/// Returns the final noise value at _x, _y based on the number of octives, the zoom and the persistence
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <param name="_octaves"></param>
	/// <param name="_zoom"></param>
	/// <param name="_persistance"></param>
	/// <returns></returns>
	static float TotalNoisePerPoint(int _x, int _y, int _octaves = 4, float _zoom = 64, float _persistance = 0.5f);

	/// <summary>
	/// Creates a JPG image of noise with the specified width and height
	/// </summary>
	/// <param name="_fileName"></param>
	/// <param name="_width"></param>
	/// <param name="_height"></param>
	static void CreateNoiseJPG(std::string _fileName, unsigned _width, unsigned _height);

	/// <summary>
	/// Creates a PNG image of noise with the specified width and height
	/// </summary>
	/// <param name="_fileName"></param>
	/// <param name="_width"></param>
	/// <param name="_height"></param>
	static void CreateNoisePNG(std::string _fileName, unsigned _width, unsigned _height);

	/// <summary>
	/// Creates a RAW file and a JPG of noise with the specified width and height
	/// </summary>
	/// <param name="_fileName"></param>
	/// <param name="_width"></param>
	/// <param name="_height"></param>
	static void CreateNoiseRAW(std::string _fileName, unsigned _width, unsigned _height);

private:
	static int Seed;
};

