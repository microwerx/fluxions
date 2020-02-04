#ifndef FLUXIONS_RENDERER_GLES30_SNAPSHOT_HPP
#define FLUXIONS_RENDERER_GLES30_SNAPSHOT_HPP

#include <fluxions_opengl.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_rect.hpp>

namespace Fluxions {
	class RendererGLES30Snapshot {
	public:
		GLint framebuffer = 0;
		GLint renderbuffer = 0;
		GLint activeTexture = 0;
		GLint program = 0;
		GLint texture2D = 0;
		GLint textureCubeMap = 0;
		GLint arrayBuffer = 0;
		GLint elementArrayBuffer = 0;
		GLint vertexArray = 0;
		GLint blendSrcRgb = 0;
		GLint blendDstRgb = 0;
		GLint blendSrcAlpha = 0;
		GLint blendDstAlpha = 0;
		GLint blendEquationRgb = 0;
		GLint blendEquationAlpha = 0;
		GLint depthFunc = 0;
		GLint stencilFunc = 0;
		GLint stencilValueMask = 0;
		GLint stencilRef = 0;
		GLint stencilBackFunc = 0;
		GLint stencilBackValueMask = 0;
		GLint stencilBackRef = 0;
		GLint stencilFail = 0;
		GLint stencilPassDepthPass = 0;
		GLint stencilPassDepthFail = 0;
		GLint stencilBackFail = 0;
		GLint stencilBackPassDepthPass = 0;
		GLint stencilBackPassDepthFail = 0;
		GLint cullFaceMode = 0;
		Color4f colorClearValue;
		Recti viewport;
		Recti scissorBox;
		GLboolean blendEnabled = GL_FALSE;
		GLboolean cullFaceEnabled = GL_FALSE;
		GLboolean depthTestEnabled = GL_FALSE;
		GLboolean scissorTestEnabled = GL_FALSE;
		GLboolean stencilTestEnabled = GL_FALSE;
		GLboolean framebufferSrgbEnabled = GL_FALSE;

		RendererGLES30Snapshot();
		~RendererGLES30Snapshot();

		void save();
		void restore();
	};
}

#endif
