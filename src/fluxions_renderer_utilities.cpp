#include "fluxions_renderer_pch.hpp"
#include <hatchetfish.hpp>
#include <hatchetfish_stopwatch.hpp>
#include <fluxions_renderer_utilities.hpp>

namespace Fluxions {
	bool RenderImage(RendererContext& rendererContext,
					 SimpleSceneGraph& ssg,
					 const std::string& rendererName,
					 const std::string& rendererConfigName) {
		Hf::StopWatch stopwatch;
		RendererGLES30* renderer = rendererContext.getRenderer(rendererName);
		if (!renderer) {
			HFLOGERROR("Renderer '%s' not found", rendererName.c_str());
			return false;
		}
		RendererConfig* rendererConfig = rendererContext.getRendererConfig(rendererConfigName);
		if (!rendererConfig) {
			HFLOGERROR("Renderer Config '%s' not found", rendererConfigName.c_str());
			return false;
		}
		renderer->setSceneGraph(&ssg);
		renderer->setRenderConfig(rendererConfig);
		if (!renderer->validate())
			return false;
		renderer->render();
		rendererConfig->metrics_total_ms = stopwatch.Stop_msf();
		return true;
	}

	bool RenderCube(RendererContext& rendererContext,
					SimpleSceneGraph& ssg,
					const std::string& rendererName,
					const std::string& rendererConfigName) {
		RendererGLES30* renderer = rendererContext.getRenderer(rendererName);
		if (!renderer) {
			HFLOGERROR("Renderer '%s' not found", rendererName.c_str());
			return false;
		}
		RendererConfig* rendererConfig = rendererContext.getRendererConfig(rendererConfigName);
		if (!rendererConfig) {
			HFLOGERROR("Renderer Config '%s' not found", rendererConfigName.c_str());
			return false;
		}
		renderer->setSceneGraph(&ssg);
		renderer->setRenderConfig(rendererConfig);
		if (!renderer->validate())
			return false;
		renderer->render();
		return true;
	}

	/*
	void RendererUtilities::RenderCubeShadowMap(const SimpleSceneGraph& ssg, RendererTextureCube& scs, const RendererConfig& rc) {
		FxSetErrorMessage("fluxions_simple_scene_graph.cpp", __LINE__, __FUNCTION__);

		if (!scs.dirty)
			return;

		double t0 = HFLOG_SECS_ELAPSED();

		scs.createTextureShadowCube(rc.viewportRect.w);

		GLuint cubeShadowFbo = 0;
		FxCreateFramebuffer(&cubeShadowFbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, scs.texture.GetTexture(), 0);
		if (rc.fbo_gen_color) {
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rc.fbo_color_map, 0);
		}
		else {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::string msg = FxGetFramebufferStatusAsString(status);
		bool fboComplete;

		if (status != GL_FRAMEBUFFER_COMPLETE)
			fboComplete = false;
		else
			fboComplete = true;

		if (!fboComplete) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			HFLOGERROR("Framebuffer not complete!");
			scs.buildTime = -1.0;
			scs.dirty = false;
			return;
		}

		RenderImage(rendererContext, ssg, "gles30", "default");
		//ssg.AdvancedRenderZOnly(rc);

		// remove texture from fbo
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		if (rc.fbo_gen_color) {
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		FxDeleteFramebuffer(&cubeShadowFbo);

		double dt = HFLOG_SECS_ELAPSED() - t0;
		scs.buildTime = dt * 1000.0;
		scs.dirty = false;
	}
	*/

	void ConvertBuffer(GLenum srccomponents, GLenum srctype, const void* srcdata, GLenum dstcomponents, GLenum dsttype, void* dstdata, GLsizei w, GLsizei h, GLsizei layers) {
		float* srcfpixels = nullptr;
		unsigned char* srcubpixels = nullptr;

		float* dstfpixels = nullptr;
		unsigned char* dstubpixels = nullptr;

		if (srctype == GL_UNSIGNED_BYTE) {
			srcubpixels = (unsigned char*)srcdata;
		}
		else if (srctype == GL_FLOAT) {
			srcfpixels = (float*)srcdata;
		}
		else {
			HFLOGWARN("Unsupported source format");
		}

		if (dsttype == GL_UNSIGNED_BYTE) {
			dstubpixels = (unsigned char*)dstdata;
		}
		else if (dsttype == GL_FLOAT) {
			dstfpixels = (float*)dstdata;
		}
		else {
			HFLOGWARN("Unsupported destination format");
		}

		unsigned size = w * h * layers;
		for (unsigned addr = 0; addr < size; addr++) {
			for (unsigned i = 0; i < dstcomponents; i++) {
				if (i >= (unsigned)srccomponents) {
					if (dstubpixels) {
						*dstubpixels++ = 0;
					}
					else if (dstfpixels) {
						*dstfpixels++ = 0.0f;
					}
				}
				else {
					if (dstubpixels && srcubpixels) {
						*dstubpixels++ = *srcubpixels++;
					}
					else if (dstubpixels && srcfpixels) {
						float v = *srcfpixels++;
						*dstubpixels++ = (unsigned char)Fluxions::clamp((int)(v * 255.99f), 0, 255);
					}
					else if (dstfpixels && srcubpixels) {
						float v = *srcubpixels++;
						*dstfpixels++ = Fluxions::clamp(v / 255.0f, 0.0f, 1.0f);
					}
					else if (dstfpixels && srcfpixels) {
						*dstfpixels++ = *srcfpixels++;
					}
				}
			}
		}
	}

	bool SaveTextureMap(GLenum target, GLuint id, const std::string& path) {
		glActiveTexture(GL_TEXTURE0);
		FxDebugBindTexture(target, id);
		int w = 0;
		int h = 0;
		int internalformat = 0;
		int layers = 1;
		GLenum type = GL_UNSIGNED_BYTE;
		int redSize = 0;
		int greenSize = 0;
		int blueSize = 0;
		int alphaSize = 0;
		int depthSize = 0;
		int redType = 0;
		int greenType = 0;
		int blueType = 0;
		int alphaType = 0;
		int depthType = 0;
		int components = 0;

		if (target == GL_TEXTURE_CUBE_MAP) {
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_HEIGHT, &h);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_RED_SIZE, &redSize);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_RED_TYPE, &redType);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_GREEN_SIZE, &greenSize);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_GREEN_TYPE, &greenType);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_BLUE_SIZE, &blueSize);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_BLUE_TYPE, &blueType);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_ALPHA_SIZE, &alphaSize);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_ALPHA_TYPE, &alphaType);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_DEPTH_SIZE, &depthSize);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_DEPTH_TYPE, &depthType);
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_TEXTURE_COMPONENTS, &components);
			layers = 6;
		}
		else if (target == GL_TEXTURE_2D) {
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_RED_SIZE, &redSize);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_RED_TYPE, &redType);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_GREEN_SIZE, &greenSize);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_GREEN_TYPE, &greenType);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_BLUE_SIZE, &blueSize);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_BLUE_TYPE, &blueType);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_ALPHA_SIZE, &alphaSize);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_ALPHA_TYPE, &alphaType);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_DEPTH_SIZE, &depthSize);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_DEPTH_TYPE, &depthType);
			glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPONENTS, &components);
		}
		else {
			return false;
		}

		int stride = w * h;
		int bytesPerPixel = 1;
		if (internalformat == GL_RGB) {
			bytesPerPixel = 3;
			type = GL_UNSIGNED_BYTE;
		}
		else if (internalformat == GL_RGBA) {
			bytesPerPixel = 4;
			type = GL_UNSIGNED_BYTE;
		}
		else if (internalformat == GL_RGBA8) {
			bytesPerPixel = 4;
			internalformat = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
		}
		else if (internalformat == GL_DEPTH_COMPONENT32F) {
			bytesPerPixel = 4;
			internalformat = GL_DEPTH_COMPONENT;
			type = GL_FLOAT;
		}
		else if (internalformat == GL_DEPTH_COMPONENT) {
			bytesPerPixel = 3;
			type = GL_UNSIGNED_BYTE;
		}
		else {
			HFLOGWARN("unknown internal format %04x / %d / %s",
					  internalformat,
					  internalformat,
					  Fluxions::glNameTranslator.getString(internalformat));
			HFLOGWARN("unknown type %d/%s/%s/%s",
					  depthSize,
					  Fluxions::glNameTranslator.getString(depthType),
					  Fluxions::glNameTranslator.getString(type),
					  Fluxions::glNameTranslator.getString(components));
			return false;
		}

		stride *= bytesPerPixel;

		std::vector<unsigned char> pixels;
		pixels.resize((size_t)(layers * stride));
		GLenum error = glGetError();
		Fluxions::Image4f i4f;
		Fluxions::Image4ub i4ub;

		if (target == GL_TEXTURE_CUBE_MAP) {
			i4f.resize(w, h, 6);
			for (int face = 0; face < 6; face++) {
				glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, internalformat, type, &pixels[face * stride]);
				error = glGetError();
				if (error != 0)
					break;
			}
			if (!error) {
				int srccomponents = 0;
				if (internalformat == GL_DEPTH_COMPONENT)
					srccomponents = 1;
				else if (internalformat == GL_RGB)
					srccomponents = 3;
				else if (internalformat == GL_RGBA)
					srccomponents = 4;

				i4f.resize(w, h, 6);
				ConvertBuffer(srccomponents, type, &pixels[0], 4, GL_FLOAT, &i4f.pixels[0], w, h, 6);
				i4f.convertCubeMapToRect();
				i4f.savePPMi(path, 255.99f, 0, 255);
			}
		}
		else if (target == GL_TEXTURE_2D) {
			glGetTexImage(GL_TEXTURE_2D, 0, internalformat, type, &pixels[0]);
			error = glGetError();
			if (!error) {
				int srccomponents = 0;
				if (internalformat == GL_DEPTH_COMPONENT)
					srccomponents = 1;
				else if (internalformat == GL_RGB)
					srccomponents = 3;
				else if (internalformat == GL_RGBA)
					srccomponents = 4;

				i4f.resize(w, h, 1);
				ConvertBuffer(srccomponents, type, &pixels[0], 4, GL_FLOAT, &i4f.pixels[0], w, h, 1);
				i4f.savePPMi(path, 255.99f, 0, 255);
			}
		}

		if (error != 0) {
			HFLOGWARN("glError() -> %s [%s/%s/%s] (R: %d/%s, G: %d/%s, B: %d/%s, A: %d/%s, Z: %d/%s)",
					  Fluxions::glNameTranslator.getString(error),
					  Fluxions::glNameTranslator.getString(internalformat),
					  Fluxions::glNameTranslator.getString(type),
					  Fluxions::glNameTranslator.getString(components),
					  redSize, Fluxions::glNameTranslator.getString(redType),
					  greenSize, Fluxions::glNameTranslator.getString(greenType),
					  blueSize, Fluxions::glNameTranslator.getString(blueType),
					  alphaSize, Fluxions::glNameTranslator.getString(alphaType),
					  depthSize, Fluxions::glNameTranslator.getString(depthType));
		}

		// bool iszero = true;
		// for (auto& p: pixels) {
		// 	if (p != '\0') {
		// 		iszero = false;
		// 		break;
		// 	}
		// }

		FxDebugBindTexture(target, 0);
		return true;
	}
}
