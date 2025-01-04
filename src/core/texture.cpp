#include "texture.h"
#include "../nublog.h"
#include <stdio.h>
#include "glad/glad_4_6.h"


core::cTexture::cTexture()
{
	_targaData = 0;
	_texture_id = 0;
	_width = 0;
	_height = 0;
}


bool core::cTexture::Load(const char* filename)
{
	if (!LoadTarga32Bit(filename))
	{
		std::string msg = "Failed to load texture ";
		LOG_ERROR("core::cTexture::Load", msg + filename);
		return false;
	}

	glGenTextures(1, &_texture_id);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _targaData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] _targaData;
	_targaData = nullptr;

	return true;
}


void core::cTexture::Destroy()
{
	if (_targaData) { delete[] _targaData;     _targaData = nullptr; }
}

void core::cTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
}

unsigned int core::cTexture::GetTexture()
{
	return _texture_id;
}


bool core::cTexture::LoadTarga32Bit(const char* filename)
{
	int bpp;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;

	if ((fopen_s(&filePtr, filename, "rb") != 0)) return false;
	if ((count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr)) != 1) return false;

	_height = (int)targaFileHeader.height;
	_width = (int)targaFileHeader.width;
	if ((bpp = (int)targaFileHeader.bpp) != 32) return false;

	int imageSize = _width * _height * 4;
	unsigned char* targaImage = new unsigned char[imageSize];
	if ((count = (unsigned int)fread(targaImage, 1, imageSize, filePtr)) != imageSize) return false;

	if (fclose(filePtr) != 0) return false;


	// todo: change varible length array _targaData to std::vector
	_targaData = new unsigned char[imageSize];

	int index = 0;
	int k = (_width * _height * 4) - (_width * 4);
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{   // convert BGRA => RGBA
			_targaData[index + 0] = targaImage[k + 2];  // Red.
			_targaData[index + 1] = targaImage[k + 1];  // Green.
			_targaData[index + 2] = targaImage[k + 0];  // Blue
			_targaData[index + 3] = targaImage[k + 3];  // Alpha

			k += 4;
			index += 4;
		}
		k -= (_width * 8);
	}

	delete[] targaImage;
	targaImage = nullptr;

	return true;
}


int core::cTexture::GetWidth()
{
	return _width;
}


int core::cTexture::GetHeight()
{
	return _height;
}
