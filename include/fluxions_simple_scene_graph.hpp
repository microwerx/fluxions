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
#ifndef FLUXIONS_SIMPLE_SCENE_GRAPH_HPP
#define FLUXIONS_SIMPLE_SCENE_GRAPH_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_file_path_info.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_obj_static_model.hpp>
#include <fluxions_pbsky.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_simple_materials.hpp>
#include <fluxions_simple_geometry_group.hpp>
#include <fluxions_simple_camera.hpp>
#include <fluxions_simple_environment.hpp>
#include <fluxions_simple_sphere.hpp>
#include <fluxions_simple_point_light.hpp>
#include <fluxions_scene_graph_reader.hpp>

namespace Fluxions
{
	constexpr int MaxSphlLights = 16;
	constexpr int MaxSphlDegree = 9;
	constexpr int DefaultSphlDegree = 2;
	constexpr int SphlSunIndex = MaxSphlLights;

	struct SceneGraphReader;
	struct SceneGraphWriter;
	struct ISimpleRendererPlugin;

	class SimpleSceneGraph {
	public:
		std::string name;
		std::vector<std::string> sceneFileLines;
		std::vector<std::string> pathsToTry;
		Matrix4f currentTransform;

		std::vector<std::string> confFiles;
		SimpleCamera camera;
		SimpleEnvironment environment;

		TResourceManager<std::string> shaderMaps;
		TResourceManager<SimpleSphere> spheres;
		TResourceManager<SimpleGeometryGroup> geometry;
		// TODO: Change OBJStaticModel to SimpleGeometryMesh
		TResourceManager<OBJStaticModel> geometryObjects;
		std::vector<SimplePointLight> pointLights;
		std::vector<SimpleSceneGraphNode*> nodes;

		SimpleMaterialSystem materials;
		//mutable SimpleRenderer_GLuint renderer;
		//__ShaderProgramLocations locs;
		std::map<std::string, SimpleMap*> currentTextures;

		ISimpleRendererPlugin* userdata = nullptr;

		//bool ReadMtlLibFile(const std::string &filename);
		//bool ReadConfFile(const std::string &filename);
		bool ReadObjFile(const std::string& filename, const std::string& name);
		//bool ReadTexmap(const std::string &name, const std::string &texmap);
		bool ReadCamera(std::istream& istr);

		// Rendering tools
		//void ApplySpheresToCurrentProgram();
		//void ApplyGlobalSettingsToCurrentProgram();
		//void ApplyMaterialToCurrentProgram(SimpleMaterial& mtl, bool useMaps);
		//void DisableCurrentTextures();

	private:
		BoundingBoxf boundingBox;

		//TSimpleResourceManager<GLuint> textureUnits;
		//void InitTexUnits();
		//void KillTexUnits();

	public:
		//GLuint GetTexUnit() { return textureUnits.Create(); }
		//void FreeTexUnit(GLuint& id) {
		//	textureUnits.Delete(id);
		//	id = 0;
		//}
		//void FreeTexUnit(GLint& id) {
		//	if (id > 0) {
		//		GLuint tid = static_cast<GLuint>(id);
		//		textureUnits.Delete(tid);
		//		id = 0;
		//	}
		//}

	public:
		SimpleSceneGraph();
		~SimpleSceneGraph();

		/// <summary>reset() is a method of SimpleSceneGraph.
		/// <para>Resets scene graph to initial conditions. It's completely empty with default values.</para></summary>
		void reset();

		bool Load(const std::string& filename);
		bool Save(const std::string& filename);

		bool Save(const char* path, SceneGraphWriter* writer) const;
		bool Load(const char* path, SceneGraphReader* reader);

		bool Save(const std::string& path, SceneGraphWriter* writer) const {
			return Save(path.c_str(), writer);
		}
		bool Load(const std::string& path, SceneGraphReader* reader) {
			return Load(path.c_str(), reader);
		}

		const BoundingBoxf& GetBoundingBox();

		/// Note this is where we went wrong, by including rendering information in the scene graph...

		//void BuildBuffers();
		//void Render(RendererProgram& program);
		//void RenderZOnly(RendererProgram& program);

		//void Render(RendererProgram& program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f& projectionMatrix, Matrix4f& cameraMatrix);

		//void AdvancedRender(RendererConfig& rc);
		//void AdvancedRenderZOnly(const RendererConfig& rc) const;
		//void RenderZOnly(RendererProgramPtr& program) const;

		//void SetUniforms(RendererProgramPtr& shader);

		//void InitSphls();
		//void MakeSphlsUnclean();
	}; // class SimpleSceneGraph
} // namespace Fluxions

#endif
