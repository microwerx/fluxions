#include "pch.hpp"
#include <fluxions_renderer_framebuffer.hpp>

#define IFTOSTRING(thing, value) \
	if ((thing) == (value))      \
		return std::string(#value);

namespace Fluxions
{
	std::string GetFramebufferStatusAsString(GLenum status) {
		IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
		IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
		IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
		IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
		IFTOSTRING(status, GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
		return std::string();
	}

	RendererFramebuffer::RendererFramebuffer() {}

	RendererFramebuffer::~RendererFramebuffer() {}

	void RendererFramebuffer::init(const std::string& name,
										 RendererObject* pparent) {
		RendererObject::init(name, pparent);
		setDefaultParameters();
	}

	void RendererFramebuffer::kill() {
		deleteBuffers();
		setDefaultParameters();
		RendererObject::kill();
	}

	const char* RendererFramebuffer::type() const {
		return "RendererFramebuffer";
	}

	void RendererFramebuffer::setDefaultParameters() {
		projectionViewMatrix.LoadIdentity();
		width = DefaultWidth;
		height = DefaultHeight;
		samples = DefaultSamples;
		useMultisamples = false;
		internalformat = GL_RGBA8;
		currentCubeFace = 0;
	}

	void RendererFramebuffer::deleteBuffers() {
		if (fbo == 0)
			return;
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			if (it->second.target == GL_RENDERBUFFER) {
				glDeleteRenderbuffers(1, &(it->second.object));
				it->second.object = 0;
			}
			else {
				glDeleteTextures(1, &(it->second.object));
				it->second.object = 0;
			}
		}
		HFLOGDEBUG("Deleting framebuffer %i", fbo);
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
		dirty = false;
	}

	bool RendererFramebuffer::make() {
		bool result = false;

		if (fbo || dirty) {
			deleteBuffers();			
		}

		glGenFramebuffers(1, &fbo);
		HFLOGDEBUG("Creating framebuffer %i", fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			RenderTarget& rt = it->second;
			if (rt.target == GL_RENDERBUFFER) {
				glGenRenderbuffers(1, &rt.object);
				glBindRenderbuffer(rt.target, rt.object);
				if (rt.useMultisamples)
					glRenderbufferStorageMultisample(rt.target, rt.samples, rt.internalformat, rt.width, rt.height);
				else
					glRenderbufferStorage(rt.target, rt.internalformat, rt.width, rt.height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, rt.attachment, rt.target, rt.object);
			}
			else {
				glGenTextures(1, &rt.object);
				FxBindTexture(0, rt.target, rt.object);
				if (rt.target == GL_TEXTURE_CUBE_MAP) {
					GLenum format = GL_RGBA;
					GLenum type = GL_UNSIGNED_BYTE;
					if (rt.attachment == GL_DEPTH_ATTACHMENT) {
						format = GL_DEPTH_COMPONENT;
						switch (rt.internalformat) {
						case GL_DEPTH_COMPONENT32F:
							type = GL_FLOAT;
							break;
						case GL_DEPTH_COMPONENT24:
							type = GL_DEPTH_COMPONENT24;
							break;
						case GL_DEPTH_COMPONENT16:
							type = GL_DEPTH_COMPONENT16;
							break;
						}
					}
					for (int face = 0; face < 6; face++) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, rt.width, rt.height, 0, format, type, nullptr);
					}
				}
				if (rt.target == GL_TEXTURE_2D) {
					if (rt.useMultisamples)
						glTexStorage2DMultisample(rt.target, rt.samples, rt.internalformat, rt.width, rt.height, GL_TRUE);
					else
						glTexStorage2D(rt.target, rt.levels, rt.internalformat, rt.width, rt.height);
				}
				glTexParameterf(rt.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(rt.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(rt.target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameterf(rt.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(rt.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				if (rt.attachment == GL_DEPTH_ATTACHMENT ||
					rt.attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
					glTexParameterf(rt.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameterf(rt.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					//glTexEnvf(rt.target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
					//glTexEnvf(rt.target, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
				}
				FxBindDefaultTextureAndSampler(rt.target);
				if (rt.target == GL_TEXTURE_CUBE_MAP) {
					glFramebufferTexture(GL_FRAMEBUFFER, rt.attachment, rt.object, 0);
				}
				else {
					glFramebufferTexture2D(GL_FRAMEBUFFER, rt.attachment, rt.target, rt.object, 0);
				}
			}
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::string msg = GetFramebufferStatusAsString(status);

		if (status != GL_FRAMEBUFFER_COMPLETE)
			result = false;
		else
			result = true;

		if (!result) {
			Hf::Log.error("%s(): Framebuffer is not complete!", __FUNCTION__);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		dirty = false;
		return result;
	}

	void RendererFramebuffer::useForReading() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	}

	void RendererFramebuffer::useForWriting() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void RendererFramebuffer::use() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	void RendererFramebuffer::restoreGLState() {
		FxSetErrorMessage(__FILE__, __LINE__, "restoreGLState()");
		// GLenum defaultBuffer = GL_BACK;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDrawBuffer(GL_BACK);// glDrawBuffers(1, &defaultBuffer);
		FxSetDefaultErrorMessage();
	}

	void RendererFramebuffer::generateMipmaps() {
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			RenderTarget& rt = it->second;
			if (rt.target == GL_RENDERBUFFER)
				continue;

			FxBindTexture(0, rt.target, rt.object);
			glGenerateMipmap(rt.target);
			FxBindDefaultTextureAndSampler(rt.target);
		}
	}

	void RendererFramebuffer::bindTextures(int unit) {
		lastBoundUnit = unit;
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			RenderTarget& rt = it->second;
			if (rt.target == GL_RENDERBUFFER)
				continue;
			FxBindTexture(unit, rt.target, rt.object);
			unit++;
		}
	}

	void RendererFramebuffer::unbindTextures() {
		int unit = lastBoundUnit;
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			RenderTarget& rt = it->second;
			if (rt.target == GL_RENDERBUFFER)
				continue;
			FxBindTexture(unit, rt.target, 0);
			unit++;
		}
	}

	void RendererFramebuffer::setDimensions(GLsizei newWidth, GLsizei newHeight) {
		width = newWidth;
		height = newHeight;
	}

	void RendererFramebuffer::setMultisamples(GLsizei newSamples, bool newUseMultisamples) {
		samples = newSamples;
		useMultisamples = newUseMultisamples;
	}

	void RendererFramebuffer::setProjectionViewMatrix(const Matrix4f& M) {
		projectionViewMatrix = M;
	}

	void RendererFramebuffer::setAllProjectionViewMatrices(const Matrix4f& M) {
		projectionViewMatrix = M;
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			it->second.projectionViewMatrix = M;
		}
	}

	void RendererFramebuffer::setCurrentCubeFace(GLenum face) {
		if (face < GL_TEXTURE_CUBE_MAP_POSITIVE_X || face > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) {
			return;
		}
		currentCubeFace = face;

		if (fbo != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
				if (it->second.target != GL_TEXTURE_CUBE_MAP)
					continue;

				RenderTarget& rt = it->second;

				rt.currentCubeFace = face;
				glFramebufferTexture2D(GL_FRAMEBUFFER, rt.attachment, rt.currentCubeFace, rt.object, 0);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RendererFramebuffer::addRenderbuffer(GLenum whichAttachment, GLenum whichInternalformat) {
		RenderTarget rt;
		rt.attachment = whichAttachment;
		rt.internalformat = whichInternalformat;
		rt.target = GL_RENDERBUFFER;
		rt.width = width;
		rt.height = height;
		rt.samples = samples;
		rt.useMultisamples = useMultisamples;
		rt.object = 0;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichAttachment, rt));
		dirty = true;
	}

	void RendererFramebuffer::addTexture2D(GLenum attachment, GLenum target, GLenum whichInternalformat, bool generateMipmaps) {
		RenderTarget rt;
		rt.attachment = attachment;
		rt.internalformat = whichInternalformat;
		rt.generateMipmaps = generateMipmaps;
		rt.levels = generateMipmaps ? (int)(log(std::max(width, height)) / log(2.0)) : 1;
		rt.levels = std::max(1, rt.levels);
		rt.object = 0;
		rt.width = width;
		rt.height = height;
		rt.samples = samples;
		rt.currentCubeFace = currentCubeFace;
		rt.useMultisamples = useMultisamples;
		rt.target = target;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichInternalformat, rt));
		dirty = true;
	}

	void RendererFramebuffer::addTextureCubeMap(GLenum attachment, GLenum target, GLenum whichInternalformat, bool generateMipmaps) {
		RenderTarget rt;
		rt.attachment = attachment;
		rt.internalformat = whichInternalformat;
		rt.generateMipmaps = generateMipmaps;
		rt.levels = generateMipmaps ? (int)(log(std::max(width, height)) / log(2.0)) : 1;
		rt.levels = std::max(1, rt.levels);
		rt.object = 0;
		rt.width = width;
		rt.height = height;
		rt.samples = samples;
		rt.currentCubeFace = currentCubeFace;
		rt.useMultisamples = useMultisamples;
		rt.target = target;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichInternalformat, rt));
		dirty = true;
	}

	void RendererFramebuffer::setMapName(const std::string& mapName) {
		if (renderTargets.empty()) return;
		renderTargets.back().second.mapName = mapName;
	}
} // namespace Fluxions