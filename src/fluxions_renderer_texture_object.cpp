#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_texture_object.hpp>

namespace Fluxions {
	//RendererSampler DefaultSamplerObject;
	//RendererTextureObject DefaultTextureObject;

	std::wstring StringToWString(const std::string& str) {
		std::wstring output;
#ifdef _WIN32
		size_t strSize = strlen(str.c_str()) + 1;
		size_t convertedChars = 0;
		wchar_t* wcstring = new wchar_t[strSize];
		mbstowcs_s(&convertedChars, wcstring, strSize, str.c_str(), _TRUNCATE);
		output = wcstring;
		delete[] wcstring;
#else
		size_t neededSize = mbstowcs(NULL, str.c_str(), 0) + 1;
		if (neededSize > 0) {
			wchar_t* wcstring = new wchar_t[neededSize];
			mbstowcs(wcstring, str.c_str(), neededSize);
			output = wcstring;
			delete[] wcstring;
		}
#endif
		return output;
	}

	// class Fluxions::Texture ///////////////////////////////////////

	RendererTextureObject::RendererTextureObject(GLenum whichTarget) {
		id = 0;
		target = whichTarget;
	}

	RendererTextureObject::~RendererTextureObject() {
		kill();
	}

	void RendererTextureObject::init(const std::string& name, RendererObject* pparent) {
		RendererObject::init(name, pparent);
		name_ = name;
		FxCreateTexture(target, &id);
	}

	void RendererTextureObject::kill() {
		FxDeleteTexture(&id);
		RendererObject::kill();
	}

	const char* RendererTextureObject::type() const noexcept {
		return "RendererTextureObject";
	}

	void RendererTextureObject::bind(GLuint unit, bool applySamplerObjectSettings) {
		if (id == 0 || !FxBindTexture(unit, target, id))
			return;

		lastBoundUnit = unit;

		if (applySamplerObjectSettings != false) {
			//samplerObject.ApplySettings(unit, id, target);
			if (cached.samplerObject) {
				cached.samplerObject->bind(unit);
			}
			didApplySamplerSettings = true;
		}

		FxSetActiveTexture(GL_TEXTURE0);
	}

	void RendererTextureObject::unbind() {
		if (lastBoundUnit > 0) {
			// GLuint sampler = didApplySamplerSettings ? samplerObject.GetId() : 0;
			FxBindTextureAndSampler(lastBoundUnit, target, 0, 0);
			lastBoundUnit = 0;
			didApplySamplerSettings = false;
			FxSetActiveTexture(GL_TEXTURE0);
		}
	}

	void RendererTextureObject::enable(GLuint unit) {
		if (unit < GL_TEXTURE0)
			unit += GL_TEXTURE0;
		FxSetActiveTexture(unit);
		glEnable(target);
		FxSetActiveTexture(GL_TEXTURE0);
	}

	void RendererTextureObject::disable(GLuint unit) {
		if (unit < GL_TEXTURE0)
			unit += GL_TEXTURE0;
		FxSetActiveTexture(unit);
		glDisable(target);
		FxSetActiveTexture(GL_TEXTURE0);
	}

	void RendererTextureObject::generateMipmaps() {
		if (id == 0)
			return;

		bool didBindUnit = false;
		if (lastBoundUnit == 0) {
			didBindUnit = true;
			bind(GL_TEXTURE0, false);
		}
		else {
			FxSetActiveTexture(lastBoundUnit);
		}
		FxGenerateMipmap(target);
		if (didBindUnit) {
			unbind();
		}
		else {
			FxSetActiveTexture(GL_TEXTURE0);
		}
	}

	bool RendererTextureObject::loadTexture1D(const std::string& filename, bool genMipMap) {
		target = GL_TEXTURE_1D;

		SDL_Surface* imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == nullptr)
			return false;

		int width = imageSurface->w;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, 1, 1, data);

		bind(0);
		setTexture1D(format, GL_UNSIGNED_BYTE, width, data, genMipMap);
		FxBindDefaultTextureAndSampler(target);
		SDL_FreeSurface(imageSurface);
		return true;
	}

	bool RendererTextureObject::loadTexture2D(const std::string& filename, bool genMipMap) {
		target = GL_TEXTURE_2D;

		SDL_Surface* imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == NULL) {
			HFLOGERROR("IMG_GetError() reports: %s", IMG_GetError());
			return false;
		}

		int width = imageSurface->w;
		int height = imageSurface->h;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		//int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_RGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA8 : 0;
		// int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_SRGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_SRGB_ALPHA : 0;
		GLenum type = GL_UNSIGNED_BYTE;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 1, data);

		FxCreateTexture(GL_TEXTURE_2D, &id);
		FxBindTexture(0, target, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, data);
		if (genMipMap != false) {
			FxGenerateMipmap(target);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FxBindDefaultTextureAndSampler(target);
		SDL_FreeSurface(imageSurface);
		return true;
	}

	bool RendererTextureObject::loadTexture1DArray(const std::string& filename, bool genMipMap) {
		target = GL_TEXTURE_1D_ARRAY;

		SDL_Surface* imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == nullptr)
			return false;

		int width = imageSurface->w;
		int height = imageSurface->h;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, 1, 1, data);

		bind(0);
		setTexture2D(format, GL_UNSIGNED_BYTE, width, height, data, genMipMap);
		FxBindDefaultTextureAndSampler(target);
		return true;
	}

	bool RendererTextureObject::loadTexture2DArray(const std::string& filename, bool genMipMap) {
		if (filename.empty()) {
			return false;
		}

		target = GL_TEXTURE_2D_ARRAY;
		bind(0);

		if (genMipMap != false) {
			generateMipmaps();
		}

		FxBindDefaultTextureAndSampler(target);
		return true;
	}

	bool RendererTextureObject::loadTexture2DArray(const std::vector<std::string>& filenames, bool genMipMap) {
		target = GL_TEXTURE_2D_ARRAY;

		unsigned char* data = nullptr;
		int count = (int)filenames.size();
		int width = 0;
		int height = 0;
		int bpp = 0;
		int format = 0;
		bool badData = false;

		for (int i = 0; i < count; i++) {
			SDL_Surface* imageSurface = IMG_Load(filenames[i].c_str());
			if (imageSurface == nullptr) {
				badData = true;
				continue;
			}

			int imageFormat = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
			if (imageFormat == 0)
				badData = true;
			if (i == 0) {
				width = imageSurface->w;
				height = imageSurface->h;
				bpp = imageSurface->format->BitsPerPixel / 8;
				format = imageFormat;

				data = new unsigned char[(size_t)(width * height * count * bpp)];
			}
			else {
				if (imageSurface->w != width || imageSurface->h != height || format != imageFormat) {
					badData = true;
					break;
				}
			}
			if (imageFormat != 0) {
				memcpy(data + (size_t)(i * (width * height * bpp)),
					   imageSurface->pixels,
					   (size_t)(width * height * bpp));
			}
			SDL_FreeSurface(imageSurface);
		}

		if (data == nullptr)
			return false;
		if (badData == true) {
			delete[] data;
			return false;
		}

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, count, data);

		bind(0);
		setTexture3D(format, GL_UNSIGNED_BYTE, width, height, count, data, genMipMap);
		FxBindDefaultTextureAndSampler(target);
		return true;
	}

	bool RendererTextureObject::loadTexture3D(const std::string& filename, int count, bool genMipMap) {
		target = GL_TEXTURE_3D;

		SDL_Surface* imageSurface = IMG_Load(filename.c_str());
		if (imageSurface == nullptr)
			return false;

		int width = imageSurface->w;
		int height = imageSurface->h / count;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		if (format == 0)
			return false;

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, count, data);

		bind(0);
		setTexture3D(format, GL_UNSIGNED_BYTE, width, height, count, data, genMipMap);
		FxBindDefaultTextureAndSampler(target);
		return true;
	}

	bool Fluxions::RendererTextureObject::loadTextureCubeMap(
		const std::string& filename_pos_x,
		const std::string& filename_pos_y,
		const std::string& filename_pos_z,
		const std::string& filename_neg_x,
		const std::string& filename_neg_y,
		const std::string& filename_neg_z,
		bool genMipMap) {
		std::vector<std::string> filenames;
		filenames.push_back(filename_pos_x);
		filenames.push_back(filename_pos_y);
		filenames.push_back(filename_pos_z);
		filenames.push_back(filename_neg_x);
		filenames.push_back(filename_neg_y);
		filenames.push_back(filename_neg_z);
		return loadTextureCubeMap(filenames, genMipMap);
	}

	bool RendererTextureObject::loadTextureCubeMap(const std::vector<std::string>& filenames, bool genMipMap) {
		target = GL_TEXTURE_CUBE_MAP;

		SDL_Surface* images[6] = { nullptr };
		int width = 0;
		int height = 0;
		int format = 0;
		int imageFormat;
		void* data[6] = { nullptr };
		bool badData = false;
		for (int i = 0; i < 6; i++) {
			images[i] = IMG_Load(filenames[i].c_str());
			if (!images[i]) {
				for (int j = 0; j < i; j++) {
					SDL_FreeSurface(images[j]);
				}
				return false;
			}
			imageFormat = images[i]->format->BitsPerPixel == 24 ? GL_RGB : images[i]->format->BitsPerPixel == 32 ? GL_RGBA : 0;
			if (imageFormat == 0)
				badData = true;
			data[i] = images[i]->pixels;
			if (i == 0) {
				width = images[i]->w;
				height = images[i]->h;
				format = imageFormat;
			}
			else {
				if (width != images[i]->w || height != images[i]->h || format != imageFormat) {
					badData = true;
				}
			}
		}

		this->image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 6, data);

		bind(0);
		setTextureCubeMap(format, GL_UNSIGNED_BYTE, width, height, data, genMipMap);
		FxBindDefaultTextureAndSampler(target);
		for (int i = 0; i < 6; i++) {
			SDL_FreeSurface(images[i]);
		}
		if (badData == true)
			return false;
		return true;
	}

	void Rotate90Right(unsigned char* pixels, size_t bytesPerPixel, size_t size) {
		unsigned char* src = (unsigned char*)pixels;
		unsigned char* dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++) {
			for (size_t y = 0; y < size; y++) {
				size_t sx = y;
				size_t sy = size - x - 1;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}

	void Rotate90Left(unsigned char* pixels, size_t bytesPerPixel, size_t size) {
		unsigned char* src = (unsigned char*)pixels;
		unsigned char* dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++) {
			for (size_t y = 0; y < size; y++) {
				size_t sx = size - y - 1;
				size_t sy = x;
				size_t dst_offset = bytesPerPixel * (y * size + x);
				size_t src_offset = bytesPerPixel * (sy * size + sx);
				memcpy(dst + dst_offset, src + src_offset, bytesPerPixel);
			}
		}
		memcpy(pixels, dst, bytesPerPixel * size * size);
		delete[] dst;
	}

	void FlipX(unsigned char* pixels, size_t bytesPerPixel, size_t size) {
		unsigned char* src = (unsigned char*)pixels;
		unsigned char* dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++) {
			for (size_t y = 0; y < size; y++) {
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

	void FlipY(unsigned char* pixels, size_t bytesPerPixel, size_t size) {
		unsigned char* src = (unsigned char*)pixels;
		unsigned char* dst = new unsigned char[bytesPerPixel * size * size];

		for (size_t x = 0; x < size; x++) {
			for (size_t y = 0; y < size; y++) {
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

	bool RendererTextureObject::loadTextureCoronaCubeMap(const std::string& filename, bool genMipMap) {
		target = GL_TEXTURE_CUBE_MAP;

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
		unsigned char* data[6];
		bool badData = false;
		SDL_Surface* imageSurface = IMG_Load(filename.c_str());

		if (!imageSurface)
			return false;

		imageFormat = imageSurface->format->BitsPerPixel == 24 ? GL_RGB : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA : 0;
		bytesPerPixel = imageSurface->format->BitsPerPixel == 24 ? 3 : imageSurface->format->BitsPerPixel == 32 ? 4 : 0;
		if (imageFormat == 0)
			badData = true;
		width = imageSurface->w;
		height = imageSurface->h;
		if (height * 6 != width)
			badData = true;
		format = imageFormat;

		if (!badData) {
			for (int i = 0; i < 6; i++) {
				int k = swizzle[i];
				width = height;
				// demultiplex the data
				data[k] = new unsigned char[(size_t)(bytesPerPixel * width * width)];
				unsigned char* dst_pixels = (unsigned char*)data[k];
				unsigned char* src_pixels = (unsigned char*)imageSurface->pixels;
				size_t rowLength = (size_t)(width * bytesPerPixel);
				size_t dst_offset = 0;
				size_t src_offset = i * rowLength;
				for (int y = 0; y < height; y++) {
					memcpy(dst_pixels + dst_offset, src_pixels + src_offset, rowLength);
					dst_offset += rowLength;
					src_offset += (size_t)(6 * width * bytesPerPixel);
				}
			}

			Rotate90Right((unsigned char*)data[2], bytesPerPixel, width);
			Rotate90Left((unsigned char*)data[3], bytesPerPixel, width);
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

			bind(0);
			setTextureCubeMap(format, GL_UNSIGNED_BYTE, width, height, (void**)data, genMipMap);
			FxBindDefaultTextureAndSampler(target);
			std::vector<unsigned char> _data;
			_data.resize((size_t)(width * height * bytesPerPixel * 6));
			for (int i = 0; i < 6; i++) {
				memcpy(&_data[(size_t)(width * height * bytesPerPixel * i)],
					   data[i],
					   (size_t)(width * height * bytesPerPixel));
			}

			this->image.setImageData(bytesPerPixel, GL_UNSIGNED_BYTE, width, height, 6, &_data[0]);

			for (int i = 0; i < 6; i++) {
				if (bytesPerPixel == 3) {
				}
				if (bytesPerPixel == 4) {
				}
				delete[] data[i];
			}
		}

		SDL_FreeSurface(imageSurface);
		if (badData == true)
			return false;
		return true;
	}

	void RendererTextureObject::setTexture1D(GLenum format, GLenum type, int width, void* data, bool genMipMap) {
		glTexImage1D(target, 0, format, width, 0, format, type, data);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTexture2D(GLenum format, GLenum type, int width, int height, void* data, bool genMipMap) {
		glTexImage2D(target, 0, format, width, height, 0, format, type, data);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTexture3D(GLenum format, GLenum type, int width, int height, int depth, void* data, bool genMipMap) {
		glTexImage3D(target, 0, format, width, height, depth, 0, format, type, data);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTexture1DArray(GLenum format, GLenum type, int width, int count, void* data, bool genMipMap) {
		glTexImage2D(target, 0, 0, width, count, 0, format, type, data);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTexture2DArray(GLenum format, GLenum type, int width, int height, int count, void* data, bool genMipMap) {
		glTexImage3D(target, 0, format, width, height, count, 0, format, type, data);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTextureCubeMap(GLenum format, GLenum type, int width, int height, void** data, bool genMipMap) {
		if (data == nullptr) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, (const void*)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, (const void*)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, (const void*)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, (const void*)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, (const void*)0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, (const void*)0);
		}
		else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, data[0]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, data[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, data[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, data[3]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, data[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, data[5]);
		}
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

	void RendererTextureObject::setTextureCubeMap(GLenum format, GLenum type, int width, int height, void* posxData, void* posyData, void* poszData, void* negxData, void* negyData, void* negzData, bool genMipMap) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, type, posxData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, type, posyData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, type, poszData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, type, negxData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, type, negyData);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, type, negzData);
		if (genMipMap != false) {
			generateMipmaps();
		}
	}

} // namespace Fluxions
