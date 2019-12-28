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

#include <fluxions_stdcxx.hpp>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_geometry_mesh.hpp>
#include <fluxions_renderer_config.hpp>
#include <fluxions_renderer_gles30_snapshot.hpp>
#include <fluxions_shader_program_locations.hpp>
#include <fluxions_simple_renderer.hpp>

namespace Fluxions
{
	class RendererContext;

	struct unit_manager {
		static constexpr int MaxUnits = 16;
		int count{ 0 };
		int index{ 0 };
		int targets[MaxUnits]{ 0 };
		int units[MaxUnits]{ 0 };
		unsigned textures[16]{ 0 };
		int ulocs[MaxUnits]{ 0 };

		unit_manager() { clear(); }

		void unit(int u) { units[index] = u; }
		int unit() const { return units[index]; }
		int unit(int i) const { return units[i]; }

		void target(int t) { targets[index] = t; }
		int target() const { return targets[index]; }
		int target(int i) const { return targets[i]; }

		void uniform_location(int loc) { ulocs[index] = loc; }
		int uniform_location() const { return ulocs[index]; }
		int uniform_location(int i) const { return units[i]; }

		void texture(unsigned t) { textures[index] = t; }
		unsigned texture() { return textures[index]; }
		unsigned texture(int i) { return textures[i]; }

		void add() { index = count; count++; }
		void add(int unit_, int target_, unsigned texture_) {
			add();
			unit(unit_);
			target(target_);
			texture(texture_);
		}

		void first() { index = 0; }
		void next() { index++; }
		void clear() { memset((void*)this, 0, sizeof(this)); }
	};

	class RendererGLES30 : public RendererObject {
	public:
		RendererGLES30();
		~RendererGLES30();

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		void invalidate_caches() override;

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

		void setContext(RendererContext* pcontext);

		bool validate() const {
			return (pSSG != nullptr
					&& pContext != nullptr
					&& pRendererConfig != nullptr
					&& pProgram != nullptr);
		}

		int getTexUnit();
		void freeTexUnit(int id);

		std::string renderconfigname;
		std::string renderskyboxname;
		std::string rendercamera;
	private:
		std::string name_;
		RendererGLES30Snapshot gles30StateSnapshot;

		//SimpleSceneGraph emptySSG;
		SimpleSceneGraph* pSSG = nullptr;
		RendererContext* pContext = nullptr;
		RendererConfig* pRendererConfig = nullptr;
		RendererProgram* pProgram = nullptr;
		RendererTextureCube* pSkyboxCube{ nullptr };
		TSimpleResourceManager<int> textureUnits;

		//Matrix4f projectionMatrix;
		//Matrix4f cameraMatrix;

		struct SCENEINFO {
			__ShaderProgramLocations locs;
			std::map<std::string, SimpleMap*> currentTextures;
			SimpleRenderer_GLuint renderer;
			bool areBuffersBuilt = false;
		} scene;

		struct SKYBOXINFO {
			GLuint abo = 0;
			GLuint eabo = 0;
			~SKYBOXINFO() { FxDeleteBuffer(&abo); FxDeleteBuffer(&eabo); }
		} skybox;

		struct VIZINFO {
			bool buffersBuilt = false;
			int kdLoc = -1;
			int worldMatrixLoc = -1;
			SimpleRenderer_GLuint renderer;
		} viz;

		struct POSTINFO {
			bool usable_ = false;
			unsigned abo = 0;
			unsigned program = 0;
			int vloc = -1;
			int tloc = -1;
			~POSTINFO() { FxDeleteBuffer(&abo); }
		} post;

		unit_manager post_units;

		void renderSingleImage();
		void renderCubeImages();
		void applyGlobalSettingsToCurrentProgram();
		void applySpheresToCurrentProgram();
		void render(RendererProgram* program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix, Matrix4f& cameraMatrix);
		void applyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		void _sceneDisableCurrentTextures();

		bool _initSkyBox();
		void _renderSkyBox();
		bool _initPost();
		void _renderPost();
		void _renderSceneGraph();
		bool _initVIZ();
		void _vizBBox(const BoundingBoxf& bbox, Matrix4f& worldMatrix, Color3f color);
		void _vizBall(const Vector3f& center, float radius, Color3f color);
		void _renderVIZ();
	};
} // namespace Fluxions

#endif
