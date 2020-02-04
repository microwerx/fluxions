#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_sampler.hpp>

namespace Fluxions {
	RendererSampler::RendererSampler() {
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

	RendererSampler::~RendererSampler() {
		kill();
	}

	void RendererSampler::init(const std::string& newname, RendererObject* pparent) {
		RendererObject::init(newname, pparent);
		FxCreateSampler(&id);
	}

	void RendererSampler::kill() {
		FxDeleteSampler(&id);
		RendererObject::kill();
	}

	const char* RendererSampler::type() const noexcept {
		return "RendererSampler";
	}

	void RendererSampler::bind(GLuint unit) {
		if (id == 0 || unit == 0 || (int)unit > g_MaxCombinedTextureUnits)
			return;
		FxBindSampler(unit, id);
		lastBoundUnit = unit;
	}

	void RendererSampler::unbind() {
		if (lastBoundUnit > 0)
			glBindSampler(lastBoundUnit, 0);
		lastBoundUnit = 0;
	}

	void RendererSampler::setWrapS(GLint param) {
		wrapS = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, param);
	}

	void RendererSampler::setWrapT(GLint param) {
		wrapT = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, param);
	}

	void RendererSampler::setWrapR(GLint param) {
		wrapR = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, param);
	}

	void RendererSampler::setWrapST(GLint S, GLint T) {
		wrapS = S;
		wrapT = T;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, S);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
	}

	void RendererSampler::setWrapTR(GLint T, GLint R) {
		wrapT = T;
		wrapR = R;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, R);
	}

	void RendererSampler::setWrapSTR(GLint S, GLint T, GLint R) {
		wrapS = S;
		wrapT = T;
		wrapR = R;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, S);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, R);
	}

	void RendererSampler::setMinFilter(GLint param) {
		minFilter = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, minFilter);
	}

	void RendererSampler::setMagFilter(GLint param) {
		magFilter = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, param);
	}

	void RendererSampler::setMinMagFilters(GLint newMinFilter, GLint newMagFilter) {
		setMinFilter(newMinFilter);
		setMagFilter(newMagFilter);
	}

	void RendererSampler::setCompareFunction(GLint func) {
		compareFunc = func;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_FUNC, func);
	}

	void RendererSampler::setCompareMode(GLint mode) {
		compareMode = mode;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, mode);
	}

	void RendererSampler::setMinLOD(GLfloat lod) {
		minLOD = lod;
		if (id != 0)
			glSamplerParameterf(id, GL_TEXTURE_MIN_LOD, lod);
	}

	void RendererSampler::setMaxLOD(GLfloat lod) {
		maxLOD = lod;
		if (id != 0)
			glSamplerParameterf(id, GL_TEXTURE_MAX_LOD, lod);
	}

	void RendererSampler::applySettings(GLuint unit, GLuint texture, GLenum target) {
		if (!FxBindTexture(unit, target, texture))
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
		FxBindTexture(0, target, 0);
	}

	void RendererSampler::use(GLenum unit, GLuint texture, GLenum target) {
		if (!FxBindTexture(unit, target, texture))
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
		FxSetActiveTexture(GL_TEXTURE0);
	}
}
