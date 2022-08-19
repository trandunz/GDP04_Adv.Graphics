#pragma once
#include<time.h>
#include <cstdlib>
#include <string>
#define PI 3.14159265358979323846264338327950288

typedef std::basic_ofstream<unsigned char, std::char_traits<unsigned char> > uofstream;

class Noise
{
public:
	static float Random(int _x, int _y);

	static float Smooth(float _x, float _y);

	static double CosineInterpolate(double _y1, double _y2, double _mu);

	static float SmoothInterpolate(float _x, float _y);

	static float TotalNoisePerPoint(int _x, int _y, int _octaves = 4, float _zoom = 64, float _persistance = 0.5f);

	static void CreateNoiseJPG(std::string _fileName, unsigned _width, unsigned _height);

	static void CreateNoiseRAW(std::string _fileName, unsigned _width, unsigned _height);
};

