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
#include <fluxions_simple_renderer.hpp>

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

		GLES30StateSnapshot();
		~GLES30StateSnapshot();

		void save();
		void restore();
	};

	class SimpleGLES30Renderer
	{
	public:
		SimpleGLES30Renderer(const std::string& name);
		//SimpleGLES30Renderer(const std::string& name, const SimpleRenderConfig& rc);
		~SimpleGLES30Renderer();

		void setSceneGraph(SimpleSceneGraph& ssg_);
		void buildBuffers();
		void render();
		void renderMesh(SimpleGeometryMesh& mesh, const Matrix4f& worldMatrix);

		// ApplyRenderConfig() returns false if there was a unrecoverable error
		bool applyRenderConfig();
		bool saveGLState();
		bool restoreGLState();
		void setRenderConfig(const SimpleRenderConfig& newRenderConfig);
		SimpleRenderConfig& getRenderConfig() { return renderConfig; }

	private:
		std::string name_;
		GLES30StateSnapshot gles30StateSnapshot;

		SimpleSceneGraph emptySSG;
		SimpleSceneGraph* pssg = nullptr;
		SimpleRenderConfig renderConfig{ "gles30renderer" };

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

		void renderSingleImage();
		void renderCubeImages();
		void applyGlobalSettingsToCurrentProgram();
		void applySpheresToCurrentProgram();
		void render(SimpleProgram& program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix, Matrix4f& cameraMatrix);
		void applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		void disableCurrentTextures();
		GLuint getTexUnit() { return textureUnits.Create(); }
		void freeTexUnit(GLuint id) { textureUnits.Delete(id); }

		void initSkyBox();
		void renderSkyBox();
	};
} // namespace Fluxions

#endif
