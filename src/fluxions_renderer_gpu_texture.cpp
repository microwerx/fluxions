#include "pch.hpp"
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions
{
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