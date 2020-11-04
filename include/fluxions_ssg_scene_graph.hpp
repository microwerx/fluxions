#ifndef FLUXIONS_SSG_SCENE_GRAPH_HPP
#define FLUXIONS_SSG_SCENE_GRAPH_HPP

#include <fluxions_ssg_base.hpp>
#include <fluxions_ssg_reader.hpp>
#include <fluxions_ssg_writer.hpp>
#include <fluxions_ssg_camera.hpp>
#include <fluxions_ssg_environment.hpp>
#include <fluxions_ssg_materials.hpp>
#include <fluxions_ssg_sphere.hpp>
#include <fluxions_ssg_geometry_group.hpp>
#include <fluxions_ssg_dirto_light.hpp>
#include <fluxions_ssg_point_light.hpp>
#include <fluxions_ssg_aniso_light.hpp>
#include <fluxions_ssg_path_animation.hpp>
#include <fluxions_ssg_renderer_plugin.hpp>

namespace Fluxions {
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
		//bool ReadOldDirectionalLight(const std::string& type, std::istream& istr);
		bool ReadDirToLight(const std::string& type, std::istream& istr);
		bool ReadPointLight(const std::string& type, std::istream& istr);
		bool ReadAnisoLight(const std::string& type, std::istream& istr);
		bool ReadSphere(const std::string& type, std::istream& istr);
		bool ReadPath(const std::string& keyword, std::istream& istr);

		bool addGeometryGroup(const std::string name, const std::string& path);
		bool addDirToLight(const std::string& name);

		const char* type() const override { return "SimpleSceneGraph"; }
		const char* keyword() const override { return "scenegraph"; }
		const char* status() const override { return "unknown"; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;

		void addPath(const std::string& path);
	private:
		std::string _findPath(std::string path) const;
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

		FileTimeValue lastWriteTime;

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
