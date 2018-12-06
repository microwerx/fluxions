// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include "stdafx.h"
#include "fluxions_opengl.hpp"
#include "fluxions_simple_texture.hpp"
#ifdef WIN32
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

namespace Fluxions
{
	using namespace std;


	SamplerObject DefaultSamplerObject;
	SimpleTexture DefaultTextureObject;


	wstring StringToWString(const std::string &str)
	{
		wstring output;
#ifdef WIN32
		size_t strSize = strlen(str.c_str()) + 1;
		size_t convertedChars = 0;
		wchar_t *wcstring = new wchar_t[strSize];
		mbstowcs_s(&convertedChars, wcstring, strSize, str.c_str(), _TRUNCATE);
		output = wcstring;
		delete[] wcstring;
#else
		size_t neededSize = mbstowcs(NULL, str.c_str(), 0) + 1;
		if (neededSize > 0)
		{
			wchar_t *wcstring = new wchar_t[neededSize];
			mbstowcs(wcstring, str.c_str(), neededSize);
			output = wcstring;
			delete [] wcstring;
		}
#endif
		return output;
	}


	SamplerObject::SamplerObject()
	{
		wrapS = GL_REPEAT;
		wrapT = GL_REPEAT;
		wrapR = GL_REPEAT;
		minFilter = GL_NEAREST_MIPMAP_LINEAR;
		magFilter = GL_LINEAR;
		compareFunc = GL_LEQUAL;
		compareMode = GL_NONE;
		minLOD = -1000.0f;
		maxLOD = 1000.0f;
		id = 0;
	}


	SamplerObject::~SamplerObject()
	{
	}


	void SamplerObject::Create()
	{
		if (id != 0)
		{
			Delete();
		}
		glGenSamplers(1, &id);
	}


	void SamplerObject::Delete()
	{
		if (id != 0)
		{
			glDeleteSamplers(1, &id);
			id = 0;
		}
	}


	void SamplerObject::Bind(GLuint unit)
	{
		if (id == 0 || unit == 0 || (int)unit > g_MaxCombinedTextureUnits)
			return;
		glutBindSampler(unit, id);
		lastBoundUnit = unit;
	}


	void SamplerObject::Unbind()
	{
		if (lastBoundUnit > 0)
			glBindSampler(lastBoundUnit, 0);
		lastBoundUnit = 0;
	}


	void SamplerObject::SetWrapS(GLint param)
	{
		wrapS = param;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_S, param);
	}


	void SamplerObject::SetWrapT(GLint param)
	{
		wrapT = param;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_T, param);
	}


	void SamplerObject::SetWrapR(GLint param)
	{
		wrapR = param;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_R, param);
	}


	void SamplerObject::SetWrapST(GLint S, GLint T)
	{
		wrapS = S;
		wrapT = T;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_S, S);
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
	}


	void SamplerObject::SetWrapTR(GLint T, GLint R)
	{
		wrapT = T;
		wrapR = R;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_R, R);
	}


	void SamplerObject::SetWrapSTR(GLint S, GLint T, GLint R)
	{
		wrapS = S;
		wrapT = T;
		wrapR = R;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_S, S);
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_WRAP_R, R);
	}


	void SamplerObject::SetMinFilter(GLint param)
	{
		minFilter = param;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, minFilter);
	}


	void SamplerObject::SetMagFilter(GLint param)
	{
		magFilter = param;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, param);
	}


	void SamplerObject::SetMinMagFilters(GLint newMinFilter, GLint newMagFilter)
	{
		SetMinFilter(newMinFilter);
		SetMagFilter(newMagFilter);
	}


	void SamplerObject::SetCompareFunction(GLint func)
	{
		compareFunc = func;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_COMPARE_FUNC, func);
	}


	void SamplerObject::SetCompareMode(GLint mode)
	{
		compareMode = mode;
		if (id != 0) glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, mode);
	}


	void SamplerObject::SetMinLOD(GLfloat lod)
	{
		minLOD = lod;
		if (id != 0) glSamplerParameterf(id, GL_TEXTURE_MIN_LOD, lod);
	}


	void SamplerObject::SetMaxLOD(GLfloat lod)
	{
		maxLOD = lod;
		if (id != 0) glSamplerParameterf(id, GL_TEXTURE_MAX_LOD, lod);
	}


	void SamplerObject::ApplySettings(GLuint unit, GLuint texture, GLenum target)
	{
		if (!glutBindTexture(unit, target, texture))
			return;
		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameterf(target, GL_TEXTURE_MIN_LOD, minLOD);
		glTexParameterf(target, GL_TEXTURE_MAX_LOD, maxLOD);
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, compareFunc);
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, compareMode);
		glutBindTexture(0, target, 0);
	}


	void SamplerObject::Use(GLenum unit, GLuint texture, GLenum target)
	{
		if (!glutBindTexture(unit, target, texture))
			return;
		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameterf(target, GL_TEXTURE_MIN_LOD, minLOD);
		glTexParameterf(target, GL_TEXTURE_MAX_LOD, maxLOD);
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, compareFunc);
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, compareMode);
		glutSetActiveTexture(GL_TEXTURE0);
	}


	// class Fluxions::Texture ///////////////////////////////////////


	SimpleTexture::SimpleTexture(GLenum whichTarget)
	{
		id = 0;
		target = whichTarget;
	}


	SimpleTexture::~SimpleTexture()
	{
		Delete();
	}


	void SimpleTexture::Create()
	{
		if (id != 0)
			Delete();
		glGenTextures(1, &id);
	}


	void SimpleTexture::Delete()
	{
		if (id != 0)
		{
			glDeleteTextures(1, &id);
		}
		id = 0;
	}


	void SimpleTexture::Bind(GLuint unit, bool applySamplerObjectSettings)
	{
		if (id == 0 || !glutBindTexture(unit, target, id)) return;
		
		lastBoundUnit = unit;

		if (applySamplerObjectSettings != false)
		{
			//samplerObject.ApplySettings(unit, id, target);
			samplerObject.Bind(unit);
			didApplySamplerSettings = true;
		}

		glutSetActiveTexture(GL_TEXTURE0);
	}


	void SimpleTexture::Unbind()
	{
		if (lastBoundUnit > 0)
		{
			GLuint sampler = didApplySamplerSettings ? samplerObject.GetId() : 0;
			glutBindTextureAndSampler(lastBoundUnit, target, 0, 0);
			lastBoundUnit = 0;
			didApplySamplerSettings = false;
			glutSetActiveTexture(GL_TEXTURE0);
		}
	}


	void SimpleTexture::Enable(GLuint unit)
	{
		if (unit < GL_TEXTURE0)
			unit += GL_TEXTURE0;
		glutSetActiveTexture(unit);
		glEnable(target);
		glutSetActiveTexture(GL_TEXTURE0);
	}


	void SimpleTexture::Disable(GLuint unit)
	{
		if (unit < GL_TEXTURE0)
			unit += GL_TEXTURE0;
		glutSetActiveTexture(unit);
		glDisable(target);
		glutSetActiveTexture(GL_TEXTURE0);
	}

	
	void SimpleTexture::GenerateMipmaps()
	{
		if (id == 0) return;

		bool didBindUnit = false;
		if (lastBoundUnit == 0)
		{
			didBindUnit = true;
			Bind(GL_TEXTURE0, false);
		}
		else
		{
			glutSetActiveTexture(lastBoundUnit);
		}
		glGenerateMipmap(target);
		if (didBindUnit)
		{
			Unbind();
		}
		else
		{
			glutSetActiveTexture(GL_TEXTURE0);
		}
	}


	bool SimpleTexture::LoadTexture1D(const std::string &filename, bool genMipMap)
	{
		target = GL_TEXTURE_1D;

		SDL_Surface *imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == nullptr)
			return false;

		int width = imageSurface->w;
		void *data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB :
			imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, 1, 1, data);

		Bind(0);
		SetTexture1D(format, GL_UNSIGNED_BYTE, width, data, genMipMap);
		glutBindDefaultTextureAndSampler(target);
		SDL_FreeSurface(imageSurface);
		return true;
	}


	bool SimpleTexture::LoadTexture2D(const std::string &filename, bool genMipMap)
	{
		target = GL_TEXTURE_2D;		

		SDL_Surface *imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == NULL)
		{
			cout << "IMG_GetError() reports: " << IMG_GetError() << endl;
			return false;
		}

		int width = imageSurface->w;
		int height = imageSurface->h;
		void *data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB :
			imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		//int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_RGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA8 : 0;
		int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_SRGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_SRGB_ALPHA : 0;
		GLenum type = GL_UNSIGNED_BYTE;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 1, data);

		glGenTextures(1, &id);
		glutBindTexture(0, target, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, data);
		if (genMipMap != false)
		{
			glGenerateMipmap(target);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glutBindDefaultTextureAndSampler(target);
		SDL_FreeSurface(imageSurface);
		return true;
	}


	bool SimpleTexture::LoadTexture1DArray(const std::string &filename, bool genMipMap)
	{
		target = GL_TEXTURE_1D_ARRAY;

		SDL_Surface *image = IMG_Load(filename.c_str());
		if (image == nullptr)
			return false;

		int width = image->w;
		int height = image->h;
		void *data = image->pixels;
		int format = image->format->BitsPerPixel == 24 ? GL_RGB :
			image->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, 1, 1, data);

		Bind(0);
		SetTexture2D(format, GL_UNSIGNED_BYTE, width, height, data, genMipMap);
		glutBindDefaultTextureAndSampler(target);
		return true;
	}


	bool SimpleTexture::LoadTexture2DArray(const std::string &filename, bool genMipMap)
	{
		target = GL_TEXTURE_2D_ARRAY;
		Bind(0);

		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
		glutBindDefaultTextureAndSampler(target);
		return true;
	}


	bool SimpleTexture::LoadTexture2DArray(const std::vector<std::string> &filenames, bool genMipMap)
	{
		target = GL_TEXTURE_2D_ARRAY;

		unsigned char *data = nullptr;
		int count = (int)filenames.size();
		int width = 0;
		int height = 0;
		int bpp = 0;
		int format = 0;
		bool badData = false;

		for (int i = 0; i < count; i++)
		{
			SDL_Surface *image = IMG_Load(filenames[i].c_str());
			if (image == nullptr)
			{
				badData = true;
				continue;
			}

			int imageFormat = image->format->BitsPerPixel == 24 ? GL_RGB :
				image->format->BitsPerPixel == 32 ? GL_RGBA : 0;
			if (imageFormat == 0)
				badData = true;
			if (i == 0)
			{
				width = image->w;
				height = image->h;
				bpp = image->format->BitsPerPixel / 8;
				format = imageFormat;

				data = new unsigned char[width * height * count * bpp];
			}
			else
			{
				if (image->w != width || image->h != height || format != imageFormat)
				{
					badData = true;
					break;
				}
			}
			if (imageFormat != 0)
			{
				memcpy(data + i * (width * height * bpp), image->pixels, width * height * bpp);
			}
			SDL_FreeSurface(image);
		}

		if (data == nullptr)
			return false;
		if (badData == true)
		{
			delete [] data;
			return false;
		}

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, count, data);

		Bind(0);
		SetTexture3D(format, GL_UNSIGNED_BYTE, width, height, count, data, genMipMap);
		glutBindDefaultTextureAndSampler(target);
		return true;
	}


	bool SimpleTexture::LoadTexture3D(const std::string &filename, int count, bool genMipMap)
	{
		target = GL_TEXTURE_3D;

		SDL_Surface *image = IMG_Load(filename.c_str());
		if (image == nullptr)
			return false;

		int width = image->w;
		int height = image->h / count;
		void *data = image->pixels;
		int format = image->format->BitsPerPixel == 24 ? GL_RGB :
			image->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, count, data);

		Bind(0);
		SetTexture3D(format, GL_UNSIGNED_BYTE, width, height, count, data, genMipMap);
		glutBindDefaultTextureAndSampler(target);
		return true;
	}


	bool Fluxions::SimpleTexture::LoadTextureCubeMap(
		const std::string & filename_pos_x,
		const std::string & filename_pos_y,
		const std::string & filename_pos_z,
		const std::string & filename_neg_x,
		const std::string & filename_neg_y,
		const std::string & filename_neg_z,
		bool genMipMap)
	{
		vector<string> filenames;
		filenames.push_back(filename_pos_x);
		filenames.push_back(filename_pos_y);
		filenames.push_back(filename_pos_z);
		filenames.push_back(filename_neg_x);
		filenames.push_back(filename_neg_y);
		filenames.push_back(filename_neg_z);
		return LoadTextureCubeMap(filenames, genMipMap);
	}


	bool SimpleTexture::LoadTextureCubeMap(const std::vector<std::string> &filenames, bool genMipMap)
	{
		target = GL_TEXTURE_CUBE_MAP;
		
		SDL_Surface *images[6] = { nullptr };
		int width;
		int height;
		int format;
		int imageFormat;
		void *data[6] = { nullptr };
		bool badData = false;
		for (int i = 0; i < 6; i++)
		{
			images[i] = IMG_Load(filenames[i].c_str());
			if (!images[i]) {
				for (int j = 0; j < i; j++) {
					SDL_FreeSurface(images[j]);
				}
				return false;
			}
			imageFormat = images[i]->format->BitsPerPixel == 24 ? GL_RGB :
				images[i]->format->BitsPerPixel == 32 ? GL_RGBA : 0;
			if (imageFormat == 0)
				badData = true;
			data[i] = images[i]->pixels;
			if (i == 0)
			{
				width = images[i]->w;
				height = images[i]->h;
				format = imageFormat;
			}
			else
			{
				if (width != images[i]->w || height != images[i]->h || format != imageFormat)
				{
					badData = true;
				}
			}
		}

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 6, data);

		Create();
		Bind(0);
		SetTextureCubeMap(format, GL_UNSIGNED_BYTE, width, height, data, genMipMap);
		glutBindDefaultTextureAndSampler(target);
		for (int i = 0; i < 6; i++)
		{
			SDL_FreeSurface(images[i]);
		}
		if (badData == true)
			return false;
		return true;
	}


	void Rotate90Right(unsigned char *pixels, size_t bytesPerPixel, size_t size)
	{
		unsigned char *src = (unsigned char *)pixels;
		unsigned char *dst = new unsigned char[bytesPerPixel * size * size];

		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				int sx = y;
				int sy = (int)size - x - 1;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}


	void Rotate90Left(unsigned char *pixels, size_t bytesPerPixel, size_t size)
	{
		unsigned char *src = (unsigned char *)pixels;
		unsigned char *dst = new unsigned char[bytesPerPixel * size * size];

		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				int sx = (int)size - y - 1;
				int sy = x;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}


	void FlipX(unsigned char *pixels, size_t bytesPerPixel, size_t size)
	{
		unsigned char *src = (unsigned char *)pixels;
		unsigned char *dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++)
		{
			for (size_t y = 0; y < size; y++)
			{
				size_t sx = size - x - 1;
				size_t sy = y;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}


	void FlipY(unsigned char *pixels, size_t bytesPerPixel, size_t size)
	{
		unsigned char *src = (unsigned char *)pixels;
		unsigned char *dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++)
		{
			for (size_t y = 0; y < size; y++)
			{
				size_t sx = x;
				size_t sy = size - y - 1;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}


	bool SimpleTexture::LoadTextureCoronaCubeMap(const string &filename, bool genMipMap)
	{
		target = GL_TEXTURE_CUBE_MAP;

		SDL_Surface *image;
		int width;
		int height;
		int format;
		int imageFormat;
		int bytesPerPixel = 0;
		int swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};
		void *data[6];
		bool badData = false;
		image = IMG_Load(filename.c_str());
		
		if (!image)
			return false;

		imageFormat = image->format->BitsPerPixel == 24 ? GL_RGB : image->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		bytesPerPixel = image->format->BitsPerPixel == 24 ? 3 : image->format->BitsPerPixel == 32 ? 4 : 0;
		if (imageFormat == 0)
			badData = true;
		width = image->w;
		height = image->h;
		if (height * 6 != width)
			badData = true;
		format = imageFormat;

		if (!badData)
		{
			for (int i = 0; i < 6; i++)
			{
				int k = swizzle[i];
				width = height;
				// demultiplex the data
				data[k] = new unsigned char[bytesPerPixel * width * width];
				unsigned char *dst_pixels = (unsigned char *)data[k];
				unsigned char *src_pixels = (unsigned char *)image->pixels;
				size_t rowLength = width * bytesPerPixel;
				size_t dst_offset = 0;
				size_t src_offset = i * rowLength;
				for (int y = 0; y < height; y++)
				{
					memcpy(dst_pixels + dst_offset, src_pixels + src_offset, rowLength);
					dst_offset += rowLength;
					src_offset += 6 * width * bytesPerPixel;
				}
			}


			Rotate90Right((unsigned char *)data[2], bytesPerPixel, width);
			Rotate90Left((unsigned char *)data[3], bytesPerPixel, width);
			//Rotate90Right((unsigned char *)data[3], bytesPerPixel, width);
			//FlipY((unsigned char *)data[3], bytesPerPixel, width);
			
			//FlipX((unsigned char *)data[0], bytesPerPixel, width);
			//FlipX((unsigned char *)data[1], bytesPerPixel, width);

			//// need to rotate the Y images 90 degrees to the right
			//{
			//	unsigned char *src = (unsigned char *)data[2];
			//	unsigned char *dst = new unsigned char[bytesPerPixel * width * width];

			//	for (int X = 0; X < width; X++)
			//	{
			//		for (int y = 0; y < height; y++)
			//		{
			//			int sx = y;
			//			int sy = width - X - 1;
			//			size_t dst_offset = bytesPerPixel * (y * height + X);
			//			size_t src_offset = bytesPerPixel * (sy * height + sx);
			//			memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			//		}
			//	}
			//	memcpy(src, dst, bytesPerPixel * width * width);
			//	delete[] dst;
			//}

			Create();
			Bind(0);
			SetTextureCubeMap(format, GL_UNSIGNED_BYTE, width, height, data, genMipMap);
			glutBindDefaultTextureAndSampler(target);
			vector<unsigned char> _data;
			_data.resize(width*height*bytesPerPixel * 6);
			for (int i = 0; i < 6; i++)
			{
				memcpy(&_data[width*height*bytesPerPixel * i], data[i], width*height*bytesPerPixel);
			}
			
			this->image.setImageData(bytesPerPixel, GL_UNSIGNED_BYTE, width, height, 6, &_data[0]);

			for (int i = 0; i < 6; i++)
			{
				if (bytesPerPixel == 3)
				{

				}
				if (bytesPerPixel == 4)
				{

				}
				delete[] data[i];
			}
		}

		SDL_FreeSurface(image);
		if (badData == true)
			return false;
		return true;
	}


	void SimpleTexture::SetTexture1D(GLenum format, GLenum type, int width, void *data, bool genMipMap)
	{
		glTexImage1D(target, 0, format, width, 0, format, type, data);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTexture2D(GLenum format, GLenum type, int width, int height, void *data, bool genMipMap)
	{
		glTexImage2D(target, 0, format, width, height, 0, format, type, data);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTexture3D(GLenum format, GLenum type, int width, int height, int depth, void *data, bool genMipMap)
	{
		glTexImage3D(target, 0, format, width, height, depth, 0, format, type, data);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTexture1DArray(GLenum format, GLenum type, int width, int count, void *data, bool genMipMap)
	{
		glTexImage2D(target, 0, 0, width, count, 0, format, type, data);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTexture2DArray(GLenum format, GLenum type, int width, int height, int count, void *data, bool genMipMap)
	{
		glTexImage3D(target, 0, format, width, height, count, 0, format, type, data);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTextureCubeMap(GLenum format, GLenum type, int width, int height, void **data, bool genMipMap)
	{
		if (data == nullptr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, (const void *)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, (const void *)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, (const void *)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, (const void *)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, (const void *)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, (const void *)0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, data[0]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, data[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, data[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, data[3]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, data[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, data[5]);
		}
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


	void SimpleTexture::SetTextureCubeMap(GLenum format, GLenum type, int width, int height, void *posxData, void *posyData, void *poszData, void *negxData, void *negyData, void *negzData, bool genMipMap)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, posxData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, posyData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, poszData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, negxData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, negyData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, negzData);
		if (genMipMap != false)
		{
			GenerateMipmaps();
		}
	}


}
