#include "fluxions_pch.hpp"
#include <fluxions_image_loader.hpp>
#include <fluxions_file_path_info.hpp>

namespace Fluxions {
	bool loadIMG(const std::string& path, Image3f& image) {
		SDL_Surface* imageSurface = IMG_Load(path.c_str());
		if (imageSurface == NULL) {
			HFLOGERROR("IMG_GetError() reports: '%s'", IMG_GetError());
			return false;
		}

		unsigned width = imageSurface->w;
		unsigned height = imageSurface->h;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB
			: imageSurface->format->BitsPerPixel == 32 ? GL_RGBA
			: 0;
		if (format == 0)
			return false;
		image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 1, data);
		SDL_FreeSurface(imageSurface);
		return true;
	}

	bool loadIMG(const std::string& path, Image4f& image) {
		SDL_Surface* imageSurface = IMG_Load(path.c_str());
		if (imageSurface == NULL) {
			HFLOGERROR("IMG_GetError() reports: '%s'", IMG_GetError());
			return false;
		}

		unsigned width = imageSurface->w;
		unsigned height = imageSurface->h;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB
			: imageSurface->format->BitsPerPixel == 32 ? GL_RGBA
			: 0;
		if (format == 0)
			return false;
		image.setImageData(format, GL_UNSIGNED_BYTE, width, height, 1, data);
		SDL_FreeSurface(imageSurface);
		return true;
	}

	bool LoadImage3f(const std::string& path, Image3f& image) {
		Hf::StopWatch stopwatch;
		FilePathInfo fpi(path);
		if (fpi.notFound()) return false;
		std::string ext = fpi.extension();
		tolower(ext);

		if (ext == ".exr" && !image.loadEXR(path)) {
			return false;
		}
		else if (ext == ".pfm" && !image.loadPFM(path)) {
			return false;
		}
		else if (ext == ".ppm" && !image.loadPPM(path)) {
			return false;
		}
		else if (ext == ".png" || ext == ".jpg") {
			if (!loadIMG(path, image))
				return false;
		}

		HFLOGINFO("Image '%s' took %3.2f secs to load", fpi.filenameC(), stopwatch.Stop_s());
		return true;
	}

	bool LoadImage4f(const std::string& path, Image4f& image) {
		Hf::StopWatch stopwatch;
		FilePathInfo fpi(path);
		if (fpi.notFound()) return false;
		std::string ext = fpi.extension();
		tolower(ext);

		if (ext == ".exr" && !image.loadEXR(path)) {
			return false;
		}
		else if (ext == ".pfm" && !image.loadPFM(path)) {
			return false;
		}
		else if (ext == ".ppm" && !image.loadPPM(path)) {
			return false;
		}
		else if (ext == ".png" || ext == ".jpg") {
			if (!loadIMG(path, image))
				return false;
		}

		HFLOGINFO("Image '%s' took %3.2f secs to load", fpi.filenameC(), stopwatch.Stop_s());
		return true;
	}
}
