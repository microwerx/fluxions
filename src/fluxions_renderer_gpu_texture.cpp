#include "pch.hpp"
#include <hatchetfish_stopwatch.hpp>
#include <fluxions_renderer_gpu_texture.hpp>
#include <fluxions_gte_image.hpp>

namespace Fluxions
{
	// Image loading routines
	void saveCubeMapVariations(const std::string& path, const Image4f& image4) {
		{Image4f tmp;
		image4.convertCubeMapToRect(tmp);
		tmp.savePPM(path + "-cube-converted.ppm"); }
		{Image4f tmp;
		image4.convertCubeMapToCross(tmp, false);
		tmp.savePPM(path + "-vcross-converted.ppm"); }
		{Image4f tmp;
		image4.convertCubeMapToCross(tmp, true);
		tmp.savePPM(path + "-hcross-converted.ppm"); }
	}

	bool loadIMG(const std::string& path, Image4f& image4) {
		SDL_Surface* imageSurface = IMG_Load(path.c_str());
		if (imageSurface == NULL) {
			HFLOGERROR("IMG_GetError() reports: %s", IMG_GetError());
			return false;
		}

		unsigned width = imageSurface->w;
		unsigned height = imageSurface->h;
		void* data = imageSurface->pixels;
		int format = imageSurface->format->BitsPerPixel == 24 ? GL_RGB
			: imageSurface->format->BitsPerPixel == 32 ? GL_RGBA
			: 0;
		//int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_RGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_RGBA8 : 0;
		// int internalformat = imageSurface->format->BitsPerPixel == 24 ? GL_SRGB8 : imageSurface->format->BitsPerPixel == 32 ? GL_SRGB_ALPHA : 0;
		if (format == 0)
			return false;
		image4.setImageData(format, GL_UNSIGNED_BYTE, width, height, 1, data);
		SDL_FreeSurface(imageSurface);
		return true;
	}

	bool gpuLoadTexture(RendererGpuTexture& t, const std::string& ext, const std::string& path, bool genMipMap) {
		Hf::StopWatch stopwatch;
		int width = 0;
		int height = 0;
		int internalformat = GL_RGBA;
		int format = GL_RGB;
		GLenum type = GL_UNSIGNED_BYTE;

		Image4f image4;
		if (ext == ".EXR") {
			if (!image4.loadEXR(path))
				return false;
			type = GL_FLOAT;
		}
		else if (ext == ".PFM") {
			if (!image4.loadPFM(path))
				return false;
			type = GL_FLOAT;
		}
		else if (ext == ".PPM") {
			if (!image4.loadPPM(path))
				return false;
		}
		else if (ext == ".PNG" || ext == ".JPG") {
			if (!loadIMG(path, image4))
				return false;
		}
		else {
			return false;
		}

		bool isCube = false;
		if (image4.isLikely61CubeMap()) {
			isCube = true;
			image4.convertRectToCubeMap();
			saveCubeMapVariations(path, image4);
		}
		else if (image4.isLikelyCross()) {
			isCube = true;
			image4.convertCrossToCubeMap();
			saveCubeMapVariations(path, image4);
		}

		while (glGetError()) HFLOGWARN("GL ERROR!");
		t.bind(0);
		if (isCube) {
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
							 0, GL_RGBA,
							 image4.width(), image4.height(),
							 0, GL_RGBA, GL_FLOAT,
							 image4.getImageData(i));
			}
			while (glGetError()) HFLOGWARN("TexImage2D     GL ERROR!");
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			while (glGetError()) HFLOGWARN("GenerateMipmap GL ERROR!");
		}
		else {
			glTexImage2D(GL_TEXTURE_2D,
						 0, GL_RGBA,
						 image4.width(), image4.height(),
						 0, GL_RGBA, GL_FLOAT,
						 image4.getImageData(0));
			while (glGetError()) HFLOGWARN("TexImage2D     GL ERROR!");
			glGenerateMipmap(GL_TEXTURE_2D);
			while (glGetError()) HFLOGWARN("GenerateMipmap GL ERROR!");
		}
		t.unbind();
		stopwatch.Stop();
		t.buildTime = stopwatch.GetSecondsElapsed();
		HFLOGINFO("Image '%s' took %3.2f secs to load", t.name(), t.buildTime);
		return true;
	}


	// RendererGpuTexture /////////////////////////////////////////////

	RendererGpuTexture::RendererGpuTexture(GLenum target) {
		if (target != GL_TEXTURE_CUBE_MAP && target != GL_TEXTURE_2D)
			throw "Unsupported Texture Target";
		target_ = target;
	}

	RendererGpuTexture::~RendererGpuTexture() {}

	void RendererGpuTexture::init(const std::string& name,
								  RendererObject* pparent) {
		RendererObject::init(name, pparent);
		try {
			texture_ = std::make_shared<GLuint>(0);
			GLuint texture;
			glGenTextures(1, &texture);
			*texture_ = texture;
			FxDebugBindTexture(target_, *texture_);
			FxDebugBindTexture(target_, 0);
			HFLOGINFO("Created texture %d", *texture_);
		}
		catch (...) {
			HFLOGERROR("glGenTextures() failed");
		}
	}

	void RendererGpuTexture::kill() {
		if (texture_.use_count() == 1) {
			GLuint texture = *texture_;
			glDeleteTextures(1, &texture);
			HFLOGINFO("Deleted texture %d", *texture_);
			texture_.reset();
		}
		RendererObject::kill();
	}

	const char* RendererGpuTexture::type() const {
		return "RendererGpuTexture";
	}

	bool RendererGpuTexture::loadMap() {
		FilePathInfo fpi(mappath);
		toupper(fpi.ext);
		return gpuLoadTexture(*this, fpi.ext, mappath, true);
	}

	void RendererGpuTexture::bind(int unit) {
		if (!texture_)
			return;
		unbind();
		lastUnitBound_ = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
		FxDebugBindTexture(target_, *texture_);
	}

	void RendererGpuTexture::unbind() {
		if (!texture_ || lastUnitBound_ < 0)
			return;

		glActiveTexture(GL_TEXTURE0 + lastUnitBound_);
		glBindTexture(target_, *texture_);
		glActiveTexture(GL_TEXTURE0);
		lastUnitBound_ = -1;
	}

	void RendererGpuTexture::createStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type) {
		if (!texture_)
			return;
		if (target_ == GL_TEXTURE_2D) {
			created_ = true;
			bind(0);
			glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, nullptr);
			unbind();
		}
		else if (target_ == GL_TEXTURE_CUBE_MAP) {
			created_ = true;
			bind(0);
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, format, type, nullptr);
			}
			unbind();
		}
	}

	void RendererGpuTexture::createTexture2D(GLsizei width, GLsizei height) {
		createStorage(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE);
		setDefaultParameters(GL_LINEAR, GL_LINEAR, GL_REPEAT);
	}

	void RendererGpuTexture::createTextureShadow2D(GLsizei width, GLsizei height) {
		createStorage(GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
		setDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTextureCube(GLsizei size) {
		createStorage(GL_RGB, size, size, GL_RGB, GL_UNSIGNED_BYTE);
		setDefaultParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTextureShadowCube(GLsizei size) {
		createStorage(GL_DEPTH_COMPONENT, size, size, GL_DEPTH_COMPONENT, GL_FLOAT);
		setDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::setDefaultParameters(GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
		if (!texture_)
			return;
		bind(0);
		glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(target_, GL_TEXTURE_WRAP_R, wrapMode);
		unbind();
	}

	void RendererGpuTexture::generateMipmap() {
		if (!texture_)
			return;
		bind(0);
		glGenerateMipmap(target_);
		unbind();
	}

	GLuint RendererGpuTexture::getTexture() const {
		if (!texture_)
			return 0;
		return *texture_;
	}
} // namespace Fluxions