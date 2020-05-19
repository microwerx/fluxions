#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_framebuffer.hpp>
#include <fluxions_renderer_gpu_texture.hpp>

namespace Fluxions {
	RendererFramebuffer::RendererFramebuffer() {}

	RendererFramebuffer::~RendererFramebuffer() {
		for (auto& [k, rt] : renderTargets) {
			delete rt.pGpuTexture;
		}
	}

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

	const char* RendererFramebuffer::status() const {
		return FxGetFramebufferStatusAsString(fbo_status);
	}

	bool RendererFramebuffer::usable() const {
		return fbo_status == GL_FRAMEBUFFER_COMPLETE;
	}

	void RendererFramebuffer::_setFormats(RenderTarget& rt) {
		switch (rt.internalformat) {
		case GL_DEPTH24_STENCIL8:
			rt.format = GL_DEPTH_COMPONENT;
			rt.type = GL_FLOAT;
			break;
		case GL_DEPTH32F_STENCIL8:
			rt.format = GL_DEPTH_COMPONENT;
			rt.type = GL_FLOAT;
			break;
		case GL_DEPTH_COMPONENT16:
			rt.format = GL_DEPTH_COMPONENT;
			rt.type = GL_FLOAT;
			break;
		case GL_DEPTH_COMPONENT24:
			rt.format = GL_DEPTH_COMPONENT;
			rt.type = GL_FLOAT;
			break;
		case GL_DEPTH_COMPONENT32F:
			rt.format = GL_DEPTH_COMPONENT;
			rt.type = GL_FLOAT;
			break;
		case GL_RGB8:
			rt.format = GL_RGB;
			rt.type = GL_UNSIGNED_BYTE;
			break;
		case GL_RGBA8:
			rt.format = GL_RGBA;
			rt.type = GL_UNSIGNED_BYTE;
			break;
		case GL_RGB16F:
			rt.format = GL_RGB;
			rt.type = GL_FLOAT;
			break;
		case GL_RGBA16F:
			rt.format = GL_RGBA;
			rt.type = GL_FLOAT;
			break;
		case GL_RGB32F:
			rt.format = GL_RGB;
			rt.type = GL_FLOAT;
			break;
		case GL_RGBA32F:
			rt.format = GL_RGBA;
			rt.type = GL_FLOAT;
			break;
		default:
			rt.internalformat = GL_RGBA8;
			rt.format = GL_BGRA;
			rt.type = GL_UNSIGNED_BYTE;
		}
	}

	void RendererFramebuffer::setDefaultParameters() {
		projectionViewMatrix.LoadIdentity();
		width_ = DefaultWidth;
		height_ = DefaultHeight;
		samples = DefaultSamples;
		useMultisamples = false;
		internalformat = GL_RGBA8;
		currentLayer = 0;
	}

	void RendererFramebuffer::deleteBuffers() {
		if (fbo == 0)
			return;
		for (auto& [type, rt] : renderTargets) {
			if (type == GL_RENDERBUFFER) {
				FxDeleteRenderbuffer(&rt.object);
			}
		}
		HFLOGDEBUG("Deleting framebuffer %i", fbo);
		FxDeleteFramebuffer(&fbo);
		dirty = false;
	}

	bool RendererFramebuffer::make() {
		if (fbo || dirty) {
			deleteBuffers();
		}

		FxCreateFramebuffer(&fbo);

		for (auto& [type, rt] : renderTargets) {
			if (rt.target == GL_RENDERBUFFER) {
				FxCreateRenderbuffer(&rt.object);
				if (rt.useMultisamples)
					glRenderbufferStorageMultisample(rt.target, rt.samples, rt.internalformat, width_, height_);
				else
					glRenderbufferStorage(rt.target, rt.internalformat, width_, height_);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, rt.attachment, rt.target, rt.object);
			}
			else {
				rt.pGpuTexture->create();
				rt.pGpuTexture->createStorage(rt.internalformat, width_, height_, rt.layers, rt.format, rt.type);
				rt.pGpuTexture->setDefaultParameters();
				rt.object = rt.pGpuTexture->getTexture();
				bool hadErrors = FxCheckLogErrors();
				switch (rt.target) {
				case GL_TEXTURE_2D_ARRAY:
					glFramebufferTextureLayer(GL_FRAMEBUFFER, rt.attachment, rt.object, 0, rt.currentLayer);
					break;
				case GL_TEXTURE_CUBE_MAP:
					glFramebufferTexture2D(GL_FRAMEBUFFER, rt.attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X + rt.currentLayer, rt.object, 0);
					break;
				case GL_TEXTURE_2D:
				default:
					glFramebufferTexture2D(GL_FRAMEBUFFER, rt.attachment, rt.target, rt.object, 0);
					break;
				}
				hadErrors = FxCheckLogErrors();
			}
		}

		fbo_status = FxCheckFramebufferStatus();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		dirty = false;
		return usable();
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

	void RendererFramebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RendererFramebuffer::generateMipmaps() {
		for (auto it = renderTargets.begin(); it != renderTargets.end(); it++) {
			RenderTarget& rt = it->second;
			if (rt.target == GL_RENDERBUFFER)
				continue;

			FxBindTexture(0, rt.target, rt.object);
			FxGenerateMipmap(rt.target);
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
		width_ = newWidth;
		height_ = newHeight;
		if (renderTargets.empty()) return;
		dirty = true;
	}

	void RendererFramebuffer::setLayers(GLsizei newLayers) {
		layers_ = newLayers;
		if (renderTargets.empty()) return;
		dirty = true;
	}

	void RendererFramebuffer::setMapName(const std::string& mapName) {
		if (renderTargets.empty()) return;
		renderTargets.back().second.mapName = mapName;
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

	void RendererFramebuffer::setCurrentLayer(GLint face) {
		if (face < 0 || face > 16) throw std::out_of_range("face should be in the range 0 to 15");
		currentLayer = face;

		if (fbo != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			for (auto& [k, rt] : renderTargets) {
				switch (rt.target) {
				case GL_TEXTURE_2D_ARRAY:
					rt.currentLayer = face;
					glFramebufferTextureLayer(GL_FRAMEBUFFER, rt.attachment, rt.object, 0, rt.currentLayer);
					break;
				case GL_TEXTURE_CUBE_MAP:
					rt.currentLayer = face;
					glFramebufferTexture2D(GL_FRAMEBUFFER, rt.attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X + rt.currentLayer, rt.object, 0);
					break;
				default:
					break;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//FxCheckLogErrors();
		}
	}

	void RendererFramebuffer::addRenderbuffer(GLenum whichAttachment, GLenum whichInternalformat) {
		RenderTarget rt;
		rt.attachment = whichAttachment;
		rt.internalformat = whichInternalformat;
		_setFormats(rt);
		rt.target = GL_RENDERBUFFER;
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
		rt.pGpuTexture = new RendererGpuTexture(target);
		rt.pGpuTexture->init("fbo", this);
		rt.internalformat = whichInternalformat;
		_setFormats(rt);
		rt.generateMipmaps = generateMipmaps;
		rt.levels = generateMipmaps ? (int)(log(std::max(width_, height_)) / log(2.0)) : 1;
		rt.levels = std::max(1, rt.levels);
		rt.object = 0;
		rt.samples = samples;
		rt.currentLayer = currentLayer;
		rt.useMultisamples = useMultisamples;
		rt.target = target;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichInternalformat, rt));
		dirty = true;
	}

	void RendererFramebuffer::addTextureCubeMap(GLenum attachment, GLenum target, GLenum whichInternalformat, bool generateMipmaps) {
		RenderTarget rt;
		rt.attachment = attachment;
		rt.pGpuTexture = new RendererGpuTexture(target);
		rt.internalformat = whichInternalformat;
		_setFormats(rt);
		rt.generateMipmaps = generateMipmaps;
		rt.levels = generateMipmaps ? (int)(log(std::max(width_, height_)) / log(2.0)) : 1;
		rt.levels = std::max(1, rt.levels);
		rt.object = 0;
		rt.samples = samples;
		rt.currentLayer = currentLayer;
		rt.useMultisamples = useMultisamples;
		rt.target = target;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichInternalformat, rt));
		dirty = true;
	}


	void RendererFramebuffer::addTexture2DArrays(GLenum attachment, GLenum target, GLenum whichInternalformat, bool generateMipmaps) {
		RenderTarget rt;
		rt.attachment = attachment;
		rt.pGpuTexture = new RendererGpuTexture(target);
		rt.internalformat = whichInternalformat;
		_setFormats(rt);
		rt.generateMipmaps = generateMipmaps;
		rt.levels = generateMipmaps ? (int)(log(std::max(width_, height_)) / log(2.0)) : 1;
		rt.levels = std::max(1, rt.levels);
		rt.object = 0;
		rt.samples = samples;
		rt.layers = layers_;
		rt.currentLayer = currentLayer;
		rt.useMultisamples = useMultisamples;
		rt.target = target;
		rt.projectionViewMatrix = projectionViewMatrix;
		renderTargets.push_back(std::pair<GLenum, RenderTarget>(whichInternalformat, rt));
		dirty = true;
	}
} // namespace Fluxions
