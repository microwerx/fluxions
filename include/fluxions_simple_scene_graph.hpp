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

#include <fluxions_simple_scene_graph_base.hpp>
#include <fluxions_scene_graph_reader.hpp>
#include <fluxions_simple_camera.hpp>
#include <fluxions_simple_environment.hpp>
#include <fluxions_simple_materials.hpp>
#include <fluxions_simple_sphere.hpp>
#include <fluxions_simple_geometry_group.hpp>
#include <fluxions_simple_dirto_light.hpp>
#include <fluxions_simple_point_light.hpp>
#include <fluxions_simple_aniso_light.hpp>
#include <fluxions_simple_path_animation.hpp>

namespace Fluxions
{

	class SimpleSceneGraph : public IBaseObject {
	public:
		//std::string name;
		std::vector<std::string> sceneFileLines;
		std::vector<std::string> pathsToTry;
		std::vector<FileTypeStringPair> pathsToLoad;
		Matrix4f currentTransform;

		float requestedExposure{ 0.0f };
		Vector2f requestedResolution{ 1024, 512 };

	public:
		SimpleCamera camera;
		SimpleEnvironment environment;

		TResourceManager<SimpleCamera> cameras;
		TResourceManager<SimpleSphere> spheres;
		TResourceManager<SimpleGeometryGroup> geometryGroups;
		TResourceManager<SimpleDirToLight> dirToLights;
		TResourceManager<SimplePointLight> pointLights;
		TResourceManager<SimpleAnisoLight> anisoLights;
		TResourceManager<SimplePathAnimation> pathanims;

		std::map<std::string, SimpleSceneGraphNode*> nodes;

		TResourceManager<SimpleGeometryMesh> staticMeshes;
		SimpleMaterialLibrary materials;

		ISimpleRendererPlugin* userdata = nullptr;

		bool ReadObjFile(const std::string& filename, const std::string& name);
		bool ReadMaterialLibrary(const std::string& type, std::istream& istr);
		bool ReadGeometryGroup(const std::string& type, std::istream& istr);
		bool ReadEnviro(const std::string& type, std::istream& istr);
		bool ReadEnviroPbsky(const std::string& type, std::istream& istr);
		bool ReadEnviroDatetime(const std::string& type, std::istream& istr);
		bool ReadCamera(const std::string& type, std::istream& istr);
		bool ReadOldDirectionalLight(const std::string& type, std::istream& istr);
		bool ReadPointLight(const std::string& type, std::istream& istr);
		bool ReadDirToLight(const std::string& type, std::istream& istr);
		bool ReadSphere(const std::string& type, std::istream& istr);
		bool ReadPath(const std::string& keyword, std::istream& istr);

		const char* type() const override { return "SimpleSceneGraph"; }
		const char* keyword() const override { return "scenegraph"; }
		const char* status() const override { return "unknown"; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream & ostr) const override;
		
	private:
		SimpleSceneGraphNode* createNode(const std::string& name, SimpleSceneGraphNode* node);
		SimpleSceneGraphNode* createCamera(const std::string& name);
		SimpleSceneGraphNode* createSphere(const std::string& name);
		SimpleSceneGraphNode* createDirToLight(const std::string& name);
		SimpleSceneGraphNode* createPointLight(const std::string& name);
		SimpleSceneGraphNode* createAnisoLight(const std::string& name);
		SimpleSceneGraphNode* createPathAnim(const std::string& name);
		SimpleSceneGraphNode* createGeometry(const std::string& name);

		void _assignIdsToMeshes();
	private:
		BoundingBoxf boundingBox;

	public:
		SimpleSceneGraph();
		~SimpleSceneGraph();

		// Resets scene graph to initial conditions.
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

		const BoundingBoxf& getBoundingBox() const { return boundingBox; }
		void calcBounds();
	}; // class SimpleSceneGraph
} // namespace Fluxions

#endif
