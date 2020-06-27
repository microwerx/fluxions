#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_gpu_texture.hpp>
#include <fluxions_image_loader.hpp>

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
			if (!LoadImage4f(path, image4))
				return false;
		}
		else {
			return false;
		}

		if (t.onloadFlipY) {
			image4.flipY();
		}

		bool isCube = false;
		if (image4.isLikely61CubeMap()) {
			isCube = true;
			image4.convertRectToCubeMapEx(t.onloadSwizzle);
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
			&& target != GL_TEXTURE_RECTANGLE
			&& target != GL_TEXTURE_2D_ARRAY)
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
			FxDeleteTexture(texture_.get());
			texture_.reset();
		}
		RendererObject::kill();
	}

	const char* RendererGpuTexture::type() const {
		return "RendererGpuTexture";
	}

	bool RendererGpuTexture::loadMap(const std::string& path, bool generateMipmaps) {
		mappath = path;
		maploaded = false;
		useMipMaps = generateMipmaps;
		return loadMap();
	}

	bool RendererGpuTexture::loadMap() {
		if (maploaded && !alwaysLoad) return true;
		FilePathInfo fpi(mappath);
		if (fpi.notFound()) {
			HFLOGERROR("map '%s' not found", mappath.c_str());
			return false;
		}
		std::string ext = fpi.extension();
		toupper(ext);
		maploaded = gpuLoadTexture(*this, ext, mappath, useMipMaps);
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
		glBindTexture(target_, 0);
		glActiveTexture(GL_TEXTURE0);
		lastUnitBound_ = -1;
	}

	void RendererGpuTexture::create() {
		if (!texture_) {
			texture_ = std::make_shared<GLuint>(0);
		}
		FxCreateTexture(target_, texture_.get());
		usable_ = false;
		FxDebugBindTexture(target_, *texture_);
		FxDebugBindTexture(target_, 0);
		//HFLOGINFO("Created texture %i", *texture_);
	}

	void RendererGpuTexture::createStorage(GLenum internalformat, GLint width, GLint height, GLint depth, GLenum format, GLenum type) {
		if (!texture_)
			return;
		while (glGetError() != GL_NO_ERROR);
		switch (target_) {
		case GL_TEXTURE_2D:
			usable_ = created_ = true;
			bind(0);
			glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, nullptr);
			unbind();
			break;
		case GL_TEXTURE_CUBE_MAP:
			usable_ = created_ = true;
			bind(0);
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, format, type, nullptr);
			}
			unbind();
			break;
		case GL_TEXTURE_3D:
		case GL_TEXTURE_2D_ARRAY:
			usable_ = created_ = true;
			bind(0);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalformat, width, height, depth, 0, format, type, nullptr);
			unbind();
			break;
		default:
			break;
		}
		while (glGetError() != GL_NO_ERROR) { usable_ = false; }
	}

	void RendererGpuTexture::createTexture2D(GLsizei width, GLsizei height) {
		createStorage(GL_RGB, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		setDefaultParameters(GL_LINEAR, GL_LINEAR, GL_REPEAT);
	}

	void RendererGpuTexture::createTextureShadow2D(GLsizei width, GLsizei height) {
		createStorage(GL_DEPTH_COMPONENT, width, height, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
		setDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTextureCube(GLsizei size) {
		createStorage(GL_RGB, size, size, 1, GL_RGB, GL_UNSIGNED_BYTE);
		setDefaultParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTextureShadowCube(GLsizei size) {
		createStorage(GL_DEPTH_COMPONENT, size, size, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
		setDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTexture2DArray(GLsizei width, GLsizei height, GLsizei depth) {
		createStorage(GL_DEPTH_COMPONENT, width, height, depth, GL_RGB, GL_UNSIGNED_BYTE);
		setDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void RendererGpuTexture::createTexture2DArrayShadow(GLsizei width, GLsizei height, GLsizei depth) {
		createStorage(GL_DEPTH_COMPONENT, width, height, depth, GL_DEPTH_COMPONENT, GL_FLOAT);
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
		GLenum internalformat = format;
		if (type == GL_FLOAT) {
			internalformat = (format == GL_RGB) ? GL_RGB16F : GL_RGBA16F;
		}
		else if (type == GL_UNSIGNED_BYTE) {
			internalformat = (format == GL_RGB) ? GL_SRGB8 : GL_SRGB8_ALPHA8;
		}
		glTexImage2D(target, 0, internalformat, width, height, 0, format, type, data);

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
		FxGenerateMipmap(target_);
		unbind();
	}

	GLuint RendererGpuTexture::getTexture() const {
		if (!texture_)
			return 0;
		return *texture_;
	}
} // namespace Fluxions
