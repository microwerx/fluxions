#include "pch.hpp"
#include <fluxions_renderer_sampler_object.hpp>

namespace Fluxions {
	RendererSamplerObject::RendererSamplerObject() {
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

	RendererSamplerObject::~RendererSamplerObject() {
		kill();
	}

	void RendererSamplerObject::init(const std::string& newname, RendererObject* pparent) {
		RendererObject::init(newname, pparent);
		kill();
		glGenSamplers(1, &id);
		HFLOGINFO("Creating sampler '%s' = %i", name(), id);
	}

	void RendererSamplerObject::kill() {
		if (id != 0) {
			HFLOGINFO("Deleting sampler '%s' = %i", name(), id);
			glDeleteSamplers(1, &id);
			id = 0;
		}
		RendererObject::kill();
	}

	void RendererSamplerObject::bind(GLuint unit) {
		if (id == 0 || unit == 0 || (int)unit > g_MaxCombinedTextureUnits)
			return;
		FxBindSampler(unit, id);
		lastBoundUnit = unit;
	}

	void RendererSamplerObject::unbind() {
		if (lastBoundUnit > 0)
			glBindSampler(lastBoundUnit, 0);
		lastBoundUnit = 0;
	}

	void RendererSamplerObject::setWrapS(GLint param) {
		wrapS = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, param);
	}

	void RendererSamplerObject::setWrapT(GLint param) {
		wrapT = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, param);
	}

	void RendererSamplerObject::setWrapR(GLint param) {
		wrapR = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, param);
	}

	void RendererSamplerObject::setWrapST(GLint S, GLint T) {
		wrapS = S;
		wrapT = T;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_S, S);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
	}

	void RendererSamplerObject::setWrapTR(GLint T, GLint R) {
		wrapT = T;
		wrapR = R;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_T, T);
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_WRAP_R, R);
	}

	void RendererSamplerObject::setWrapSTR(GLint S, GLint T, GLint R) {
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

	void RendererSamplerObject::setMinFilter(GLint param) {
		minFilter = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, minFilter);
	}

	void RendererSamplerObject::setMagFilter(GLint param) {
		magFilter = param;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, param);
	}

	void RendererSamplerObject::setMinMagFilters(GLint newMinFilter, GLint newMagFilter) {
		setMinFilter(newMinFilter);
		setMagFilter(newMagFilter);
	}

	void RendererSamplerObject::setCompareFunction(GLint func) {
		compareFunc = func;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_FUNC, func);
	}

	void RendererSamplerObject::setCompareMode(GLint mode) {
		compareMode = mode;
		if (id != 0)
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, mode);
	}

	void RendererSamplerObject::setMinLOD(GLfloat lod) {
		minLOD = lod;
		if (id != 0)
			glSamplerParameterf(id, GL_TEXTURE_MIN_LOD, lod);
	}

	void RendererSamplerObject::setMaxLOD(GLfloat lod) {
		maxLOD = lod;
		if (id != 0)
			glSamplerParameterf(id, GL_TEXTURE_MAX_LOD, lod);
	}

	void RendererSamplerObject::applySettings(GLuint unit, GLuint texture, GLenum target) {
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

	void RendererSamplerObject::use(GLenum unit, GLuint texture, GLenum target) {
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