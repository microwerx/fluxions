#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions {
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
		GLenum type = GL_UNSIGNED_BYTE;

		Image4f image4;
		if (ext == ".EXR") {
			if (!image4.loadEXR(path)) {
				return false;
			}
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
			//saveCubeMapVariations(path, image4);
		}
		else if (image4.isLikelyCross()) {
			isCube = true;
			image4.convertCrossToCubeMap();
			//saveCubeMapVariations(path, image4);
		}

		t.useMipMaps = genMipMap;
		if (isCube) {
			t.setTextureCube(image4);
		}
		else {
			t.setTexture2D(image4);
		}
		stopwatch.Stop();
		t.buildTime = stopwatch.GetSecondsElapsed();
		HFLOGINFO("Image '%s' took %3.2f secs to load", t.name(), t.buildTime);
		return true;
	}


	// RendererGpuTexture /////////////////////////////////////////////

	RendererGpuTexture::RendererGpuTexture(GLenum target) {
		if (target != GL_TEXTURE_CUBE_MAP
			&& target != GL_TEXTURE_2D
			&& target != GL_TEXTURE_RECTANGLE)
			throw "Unsupported Texture Target";
		target_ = target;
	}

	RendererGpuTexture::~RendererGpuTexture() {}

	void RendererGpuTexture::init(const std::string& name,
								  RendererObject* pparent) {
		RendererObject::init(name, pparent);
		usable_ = false;
		texture_ = std::make_shared<GLuint>(0);
	}

	void RendererGpuTexture::kill() {
		if (texture_.use_count() == 1) {
			GLuint texture = *texture_;
			if (texture != 0) {
				glDeleteTextures(1, &texture);
				//HFLOGINFO("Deleted texture %d", *texture_);
			}
			texture_.reset();
		}
		RendererObject::kill();
	}

	const char* RendererGpuTexture::type() const {
		return "RendererGpuTexture";
	}

	bool RendererGpuTexture::loadMap(const std::string& path, bool generateMipMaps) {
		mappath = path;
		maploaded = false;
		return loadMap();
	}

	bool RendererGpuTexture::loadMap() {
		if (maploaded && !alwaysLoad) return true;
		FilePathInfo fpi(mappath);
		toupper(fpi.ext);
		maploaded = gpuLoadTexture(*this, fpi.ext, mappath, true);
		return maploaded;
	}

	void RendererGpuTexture::bind(int whichunit) {
		if (!texture_)
			return;
		unbind();
		lastUnitBound_ = whichunit;
		glActiveTexture(GL_TEXTURE0 + whichunit);
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

	void RendererGpuTexture::create() {
		if (!texture_) {
			texture_ = std::make_shared<GLuint>(0);
		}
		GLuint texture = *texture_;
		if (texture != 0) {
			//HFLOGINFO("Deleted texture %i", texture);
			glDeleteTextures(1, &texture);
		}
		glGenTextures(1, &texture);
		*texture_ = texture;
		usable_ = false;
		FxDebugBindTexture(target_, *texture_);
		FxDebugBindTexture(target_, 0);
		//HFLOGINFO("Created texture %i", *texture_);
	}

	void RendererGpuTexture::createStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type) {
		if (!texture_)
			return;
		while (glGetError() != GL_NO_ERROR);
		if (target_ == GL_TEXTURE_2D) {
			usable_ = created_ = true;
			bind(0);
			glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, nullptr);
			unbind();
		}
		else if (target_ == GL_TEXTURE_CUBE_MAP) {
			usable_ = created_ = true;
			bind(0);
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, format, type, nullptr);
			}
			unbind();
		}
		while (glGetError() != GL_NO_ERROR) { usable_ = false; }
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

	void RendererGpuTexture::setTextureCube(const Image3f& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGB, GL_FLOAT, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTextureCube(const Image3ub& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGB, GL_UNSIGNED_BYTE, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTextureCube(const Image3us& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGB, GL_UNSIGNED_SHORT, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTextureCube(const Image4f& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_FLOAT, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTextureCube(const Image4ub& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_UNSIGNED_BYTE, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTextureCube(const Image4us& image) {
		if (!image.IsCubeMap()) { usable_ = false; return; }
		create();
		bind(0);
		for (int i = 0; i < 6; i++) {
			_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_UNSIGNED_SHORT, i, image.getImageData(i));
		}
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image3f& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGB, GL_FLOAT, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image3ub& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGB, GL_UNSIGNED_BYTE, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image3us& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_UNSIGNED_SHORT, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image4f& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_FLOAT, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image4ub& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_UNSIGNED_BYTE, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::setTexture2D(const Image4us& image, unsigned arrayElement) {
		create();
		bind(0);
		_setTexture2D((int)image.width(), (int)image.height(), GL_RGBA, GL_UNSIGNED_SHORT, arrayElement, image.getImageData(arrayElement));
		generateMipmap();
	}

	void RendererGpuTexture::_setTexture2D(int width, int height, unsigned format, unsigned type, unsigned arrayElement, const void* data) {
		while (glGetError() != GL_NO_ERROR);
		usable_ = true;

		unsigned target = target_;
		if (target_ == GL_TEXTURE_CUBE_MAP) {
			target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + arrayElement;
		}
		glTexImage2D(target, 0, format, width, height, 0, format, type, data);

		while (glGetError() != GL_NO_ERROR) {
			usable_ = false;
		}
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
		if (!texture_ || !usable_ || !useMipMaps)
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
