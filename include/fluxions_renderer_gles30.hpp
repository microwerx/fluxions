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
#ifndef FLUXIONS_RENDERER_GLES30_HPP
#define FLUXIONS_RENDERER_GLES30_HPP

#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_geometry_mesh.hpp>
#include <fluxions_renderer_config.hpp>
#include <fluxions_renderer_gles30_snapshot.hpp>
#include <fluxions_shader_program_locations.hpp>

namespace Fluxions
{
	class RendererGLES30 : public RendererObject
	{
	public:
		RendererGLES30();
		~RendererGLES30();

		void init(const std::string& name) override;
		void kill() override;

		void buildBuffers();
		void render();
		void renderMesh(SimpleGeometryMesh& mesh, const Matrix4f& worldMatrix);

		// ApplyRenderConfig() returns false if there was a unrecoverable error
		bool applyRenderConfig();
		bool saveGLState();
		bool restoreGLState();

		void setSceneGraph(SimpleSceneGraph* pSSG_);
		SimpleSceneGraph* getSceneGraph();

		void setRenderConfig(RendererConfig* newRenderConfig);
		RendererConfig* getRenderConfig();

		bool validate() const {
			return (pSSG != nullptr
				&& pRenderConfig != nullptr
				&& pProgram != nullptr);
		}

		GLuint getTexUnit();
		void freeTexUnit(GLuint id);

	private:
		std::string name_;
		RendererGLES30Snapshot gles30StateSnapshot;

		//SimpleSceneGraph emptySSG;
		SimpleSceneGraph* pSSG = nullptr;
		RendererConfig* pRenderConfig = nullptr;
		RendererProgramPtr pProgram = nullptr;

		Matrix4f projectionMatrix;
		Matrix4f cameraMatrix;

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
		void render(RendererProgramPtr program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix, Matrix4f& cameraMatrix);
		void applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		void disableCurrentTextures();

		void initSkyBox();
		void renderSkyBox();
	};
} // namespace Fluxions

#endif
