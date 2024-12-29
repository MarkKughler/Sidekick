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
	// Load the targa image data into memory.
	if (!LoadTarga32Bit(filename))
	{
		std::string msg = "Failed to load texture ";
		LOG_ERROR("core::cTexture::Load", msg+filename)
		return false;
	}
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &_texture_id);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _targaData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Release the targa image data
	delete[] _targaData;
	_targaData = 0;

	return true;
}


void core::cTexture::Destroy()
{
	//if (m_textureView) { m_textureView->Release(); m_textureView = nullptr; }
	//if (m_texture) { m_texture->Release();     m_texture = nullptr; }
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
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	_height = (int)targaFileHeader.height;
	_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = _width * _height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	_targaData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (_width * _height * 4) - (_width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for (j = 0; j < _height; j++)
	{
		for (i = 0; i < _width; i++)
		{
			_targaData[index + 0] = targaImage[k + 2];  // Red.
			_targaData[index + 1] = targaImage[k + 1];  // Green.
			_targaData[index + 2] = targaImage[k + 0];  // Blue
			_targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (_width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = 0;

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
