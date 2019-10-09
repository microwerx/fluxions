#include "pch.hpp"
#include <fluxions_simple_gpu_texture.hpp>

namespace Fluxions
{
	SimpleGpuTexture::SimpleGpuTexture(GLenum target) {
		if (target != GL_TEXTURE_CUBE_MAP && target != GL_TEXTURE_2D)
			throw "Unsupported Texture Target";

		target_ = target;
		try {
			texture_ = std::make_shared<GLuint>(0);
			GLuint texture;
			glGenTextures(1, &texture);
			*texture_ = texture;
			FxDebugBindTexture(target_, *texture_);
			FxDebugBindTexture(target_, 0);
			// Hf::Log.info("%s(): Created texture %d", __FUNCTION__, *texture_);
		}
		catch (...) {
			// Hf::Log.error("%s(): glGenTextures() failed", __FUNCTION__);
		}
	}

	SimpleGpuTexture::~SimpleGpuTexture() {
		if (texture_.use_count() == 1) {
			GLuint texture = *texture_;
			glDeleteTextures(1, &texture);
			// Hf::Log.info("%s(): Deleted texture %d", __FUNCTION__, *texture_);
		}
	}

	void SimpleGpuTexture::Bind(int unit) {
		if (!texture_)
			return;
		Unbind();
		lastUnitBound_ = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
		FxDebugBindTexture(target_, *texture_);
	}

	void SimpleGpuTexture::Unbind() {
		if (!texture_ || lastUnitBound_ < 0)
			return;

		glActiveTexture(GL_TEXTURE0 + lastUnitBound_);
		glBindTexture(target_, *texture_);
		glActiveTexture(GL_TEXTURE0);
		lastUnitBound_ = -1;
	}

	void SimpleGpuTexture::CreateStorage(GLenum internalformat, GLint width, GLint height, GLenum format, GLenum type) {
		if (!texture_)
			return;
		if (target_ == GL_TEXTURE_2D) {
			created_ = true;
			Bind(0);
			glTexImage2D(target_, 0, internalformat, width, height, 0, format, type, nullptr);
			Unbind();
		}
		else if (target_ == GL_TEXTURE_CUBE_MAP) {
			created_ = true;
			Bind(0);
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, width, height, 0, format, type, nullptr);
			}
			Unbind();
		}
	}

	void SimpleGpuTexture::CreateTexture2D(GLsizei width, GLsizei height) {
		CreateStorage(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE);
		SetDefaultParameters(GL_LINEAR, GL_LINEAR, GL_REPEAT);
	}

	void SimpleGpuTexture::CreateTextureShadow2D(GLsizei width, GLsizei height) {
		CreateStorage(GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
		SetDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void SimpleGpuTexture::CreateTextureCube(GLsizei size) {
		CreateStorage(GL_RGB, size, size, GL_RGB, GL_UNSIGNED_BYTE);
		SetDefaultParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}

	void SimpleGpuTexture::CreateTextureShadowCube(GLsizei size) {
		CreateStorage(GL_DEPTH_COMPONENT, size, size, GL_DEPTH_COMPONENT, GL_FLOAT);
		SetDefaultParameters(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}

	void SimpleGpuTexture::SetDefaultParameters(GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
		if (!texture_)
			return;
		Bind(0);
		glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(target_, GL_TEXTURE_WRAP_R, wrapMode);
		Unbind();
	}

	void SimpleGpuTexture::GenerateMipmap() {
		if (!texture_)
			return;
		Bind(0);
		glGenerateMipmap(target_);
		Unbind();
	}

	GLuint SimpleGpuTexture::GetTexture() const {
		if (!texture_)
			return 0;
		return *texture_;
	}
} // namespace Fluxions