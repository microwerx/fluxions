// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#ifndef FLUXIONS_SIMPLE_GLES30_RENDERER
#define FLUXIONS_SIMPLE_GLES30_RENDERER

#include <fluxions_simple_geometry_mesh.hpp>
#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{
	class GLES30StateSnapshot
	{
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

		GLES30StateSnapshot() {}

		~GLES30StateSnapshot() {}

		void Save() {
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
		}

		void Restore() {
			glUseProgram(program);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
			glActiveTexture(GL_TEXTURE0 + activeTexture);
			glBindTexture(GL_TEXTURE_2D, texture2D);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
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
			glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
			glScissor(scissorBox.x, scissorBox.y, scissorBox.w, scissorBox.h);
		}
	};

	class SimpleGLES30Renderer
	{
	public:
		SimpleGLES30Renderer();
		SimpleGLES30Renderer(const SimpleRenderConfiguration& rc);
		~SimpleGLES30Renderer();

		void SetSceneGraph(SimpleSceneGraph& ssg_);
		void BuildBuffers();
		void Render();
		void RenderMesh(SimpleGeometryMesh& mesh, const Matrix4f& worldMatrix);

		// ApplyRenderConfig() returns false if there was a unrecoverable error
		bool ApplyRenderConfig();
		bool SaveGLState();
		bool RestoreGLState();
		void SetRenderConfig(const SimpleRenderConfiguration& newRenderConfig);
		SimpleRenderConfiguration& GetRenderConfig() { return renderConfig; }

	private:
		GLES30StateSnapshot gles30StateSnapshot;

		SimpleSceneGraph emptySSG;
		SimpleSceneGraph& ssg = emptySSG;
		SimpleRenderConfiguration renderConfig;

		Matrix4f projectionMatrix;
		Matrix4f cameraMatrix;
		SimpleProgramPtr program = nullptr;

		__ShaderProgramLocations locs;
		std::map<std::string, SimpleMap*> currentTextures;
		TSimpleResourceManager<GLuint> textureUnits;
		SimpleRenderer_GLuint renderer;
		bool areBuffersBuilt = false;
		// for skybox
		GLuint abo = 0;
		GLuint eabo = 0;

		void RenderSingleImage();
		void RenderCubeImages();
		void ApplyGlobalSettingsToCurrentProgram();
		void ApplySpheresToCurrentProgram();
		void Render(SimpleProgram& program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix, Matrix4f& cameraMatrix);
		void ApplyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		void DisableCurrentTextures();
		GLuint GetTexUnit() { return textureUnits.Create(); }
		void FreeTexUnit(GLuint id) { textureUnits.Delete(id); }

		void InitSkyBox();
		void RenderSkyBox();
	};
} // namespace Fluxions

#endif
