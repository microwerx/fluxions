#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_gles30_snapshot.hpp>

namespace Fluxions {
	//////////////////////////////////////////////////////////////////////
	// RendererGLES30Snapshot ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	RendererGLES30Snapshot::RendererGLES30Snapshot() {}

	RendererGLES30Snapshot::~RendererGLES30Snapshot() {}

	void RendererGLES30Snapshot::save() {
		glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
		if (activeTexture >= GL_TEXTURE0)
			activeTexture -= GL_TEXTURE0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture2D);
		glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &textureCubeMap);
		glActiveTexture(GL_TEXTURE0);
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &renderbuffer);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBuffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertexArray);
		glGetIntegerv(GL_BLEND_SRC_RGB, &blendSrcRgb);
		glGetIntegerv(GL_BLEND_DST_RGB, &blendDstRgb);
		glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
		glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
		glGetIntegerv(GL_BLEND_EQUATION_RGB, &blendEquationRgb);
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &blendEquationAlpha);
		glGetIntegerv(GL_VIEWPORT, viewport.v());
		glGetIntegerv(GL_SCISSOR_BOX, scissorBox.v());
		glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

		glGetIntegerv(GL_STENCIL_FUNC, &stencilFunc);
		glGetIntegerv(GL_STENCIL_REF, &stencilRef);
		glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencilValueMask);
		glGetIntegerv(GL_STENCIL_BACK_FUNC, &stencilBackFunc);
		glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &stencilBackValueMask);
		glGetIntegerv(GL_STENCIL_BACK_REF, &stencilBackRef);

		glGetIntegerv(GL_STENCIL_FAIL, &stencilFail);
		glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &stencilPassDepthFail);
		glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &stencilPassDepthPass);
		glGetIntegerv(GL_STENCIL_BACK_FAIL, &stencilBackFail);
		glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &stencilBackPassDepthFail);
		glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &stencilBackPassDepthPass);

		glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
		glGetFloatv(GL_COLOR_CLEAR_VALUE, colorClearValue.ptr());
		blendEnabled = glIsEnabled(GL_BLEND);
		cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
		depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
		scissorTestEnabled = glIsEnabled(GL_SCISSOR_TEST);
		stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
		framebufferSrgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
	}

	void RendererGLES30Snapshot::restore() {
		glUseProgram(program);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glActiveTexture(GL_TEXTURE0 + activeTexture);
		glBindTexture(GL_TEXTURE_2D, texture2D);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
		glBlendEquationSeparate(blendEquationRgb, blendEquationAlpha);
		glBlendFuncSeparate(blendSrcRgb, blendDstRgb, blendSrcAlpha, blendDstAlpha);
		glDepthFunc(depthFunc);
		glStencilFuncSeparate(GL_FRONT, stencilFunc, stencilRef, stencilValueMask);
		glStencilFuncSeparate(GL_BACK, stencilBackFunc, stencilBackRef, stencilBackValueMask);
		glStencilOpSeparate(GL_FRONT, stencilFail, stencilPassDepthFail, stencilPassDepthPass);
		glStencilOpSeparate(GL_BACK, stencilBackFail, stencilBackPassDepthFail, stencilBackPassDepthPass);
		glCullFace(cullFaceMode);
		glClearColor(colorClearValue.r, colorClearValue.g, colorClearValue.b, colorClearValue.a);
		if (blendEnabled)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		if (cullFaceEnabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		if (depthTestEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		if (scissorTestEnabled)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		if (stencilTestEnabled)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		if (framebufferSrgbEnabled)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);
		glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
		glScissor(scissorBox.x, scissorBox.y, scissorBox.w, scissorBox.h);
	}
}
