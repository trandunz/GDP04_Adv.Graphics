#include "Noise.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STBI/stb_image_write.h>
#include <fstream>
#include <vector>

int Noise::Seed = time(NULL);

float Noise::Random(int _x, int _y)
{
	double value;
	int noise = _x + _y * Seed;
	noise = (noise << 13) ^ noise;
	int t = (noise * (noise * noise * 15731 + 789221) + 1376312589) & 0x7fffffff;
	value = 1.0 - double(t) * 0.931322574615478515625e-9;
	return value;
}

float Noise::Smooth(float _x, float _y)
{
	float corners = (Random(_x - 1, _y - 1) + Random(_x + 1, _y - 1) + Random(_x - 1, _y + 1) + Random(_x + 1, _y + 1)) / 16;
	float sides = (Random(_x - 1, _y) + Random(_x + 1, _y) + Random(_x, _y - 1) + Random(_x, _y + 1)) / 8;
	float center = Random(_x, _y) / 4;
	return corners + sides + center;
}

double Noise::CosineInterpolate(double _y1, double _y2, double _mu)
{
	double mu2;
	mu2 = (1 - cos(_mu * PI)) / 2;
	return (_y1 * (1 - mu2) + _y2 * mu2);
}

double Noise::LinearInterpolate(double _y1, double _y2, double _mu)
{
	return (_y1 * (1 - _mu) + _y2 * _mu);
}

float Noise::SmoothInterpolate(float _x, float _y)
{
	float fractX = _x - int(_x);
	float fractY = _y - int(_y);

	float v1 = Smooth(int(_x), int(_y));
	float v2 = Smooth(int(_x) + 1, int(_y));
	float v3 = Smooth(int(_x), int(_y) + 1);
	float v4 = Smooth(int(_x) + 1, int(_y) + 1);

	float i1 = CosineInterpolate(v1, v2, fractX);
	float i2 = CosineInterpolate(v3, v4, fractX);

	float final = CosineInterpolate(i1, i2, fractY);

	return final;
}

float Noise::TotalNoisePerPoint(int _x, int _y, int _octaves, float _zoom, float _persistance)
{
	float total = 0.0f;
	for (int i = 0; i < _octaves - 1; i++)
	{
		float frequency = pow(2, i) / _zoom;
		float amplitude = pow(_persistance, i);

		total += SmoothInterpolate(_x * frequency, _y * frequency) * amplitude;
	}

	return (total);
}

void Noise::CreateNoiseJPG(std::string _fileName, unsigned _width, unsigned _height)
{
	uint8_t* pixels = new uint8_t[_width * _height];

	int index = 0;
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			pixels[index++] = uint8_t(128 * TotalNoisePerPoint(i, j) + 128);
		}
	}

	stbi_write_jpg(("Resources/Textures/Heightmaps/" + _fileName + ".jpg").c_str(), _width, _height, 1, pixels, 100);

	delete[] pixels;
	pixels = nullptr;
}

void Noise::CreateNoisePNG(std::string _fileName, unsigned _width, unsigned _height)
{
	uint8_t* pixels = new uint8_t[_width * _height];

	int index = 0;
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			pixels[index++] = uint8_t(128 * TotalNoisePerPoint(i, j) + 128);
		}
	}

	stbi_write_png(("Resources/Textures/Heightmaps/" + _fileName + ".png").c_str(), _width, _height, 1, pixels, _width);

	delete[] pixels;
	pixels = nullptr;
}

void Noise::CreateNoiseRAW(std::string _fileName, unsigned _width, unsigned _height)
{
	uint8_t* pixels = new uint8_t[_width * _height];

	int index = 0;
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			pixels[index++] = (uint8_t)(128 * TotalNoisePerPoint(i, j) + 128);
		}
	}

	std::ofstream binaryFile("Resources/Textures/Heightmaps/" + _fileName + ".RAW", std::ios_base::binary);
	if (binaryFile)
	{
		binaryFile.write((char*)&pixels[0], (std::streamsize)(_width * _height));
		binaryFile.close();
	}

	stbi_write_jpg(("Resources/Textures/Heightmaps/" + _fileName + ".jpg").c_str(), _width, _height, 1, pixels, 100);

	delete[] pixels;
	pixels = nullptr;
}
