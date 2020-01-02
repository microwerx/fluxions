#ifndef FLUXIONS_SCENE_GRAPH_WRITER_HPP
#define FLUXIONS_SCENE_GRAPH_WRITER_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_file_reader_writer.hpp>
#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{

// Older Corona Inspired Plain Text Scene Graph Writer
struct SceneGraphWriter : TFileWriter<SimpleSceneGraph> {
	bool open(const char* path) override;
	void close() override;
	bool write(const SimpleSceneGraph* ssg) override;
protected:
	std::ofstream fout;
};

// Newer Corona Inspired Xml Scene Graph Writer
struct XmlSceneGraphWriter : SceneGraphWriter {
	// Writes out the sequence
	// extra_tags
	// camera
	// lights
	// geometryGroups groups
	bool write(const SimpleSceneGraph* ssg) override;

	void setPerspectiveCamera(const Vector3f& origin,
							  const Vector3f& target,
							  const Vector3f& roll,
							  const float horizontalFieldOfViewInDegrees);
	void setCubeMapCamera(const Vector3f& origin,
						  const Vector3f& target,
						  const Vector3f& roll);

	// These tags are written out before the sequence
	// camera
	// lights
	// geometryGroups groups
	using string_string = std::pair<std::string, std::string>;
	
	// If first string is "", then the second is interpreted as already formatted
	std::vector<string_string> extra_tags;

	bool write_sun = true;
	bool write_geometry = true;
	bool enableKs = true;
	std::string export_path_prefix{ "./xml_scn_output/" };
private:
	std::string cameraType{ "perspective" };
	Fluxions::Vector3f cameraOrigin;
	Fluxions::Vector3f cameraTarget;
	Fluxions::Vector3f cameraRoll;
	Fluxions::Matrix4f cameraMatrix;
	float cameraHorizontalFieldOfViewInDegrees = 45.0f;
	bool camera_specified_ = false;

	std::string lights_mtllib{ "ssphh_lights.mtl" };

	using string_string_Matrix4f = std::tuple<std::string, std::string, Matrix4f>;
	std::vector<string_string_Matrix4f> geometryGroups;
	std::map<std::string, int> written_materials;
	std::map<std::string, std::string> written_maps;
	std::string mtl_name;

	void writeCamera(std::ostream& ostr);
	void writeSun(std::ostream& ostr, const SimpleSceneGraph* ssg);
	void writeGeometryGroups(std::ostream& ostr, const SimpleSceneGraph* ssg);
	void writeLightMtllib(const std::string& path, const SimpleSceneGraph* ssg);
	void writeMtl(std::ostream& mtl_fout,
				  std::ostream& mtlxml_fout,
				  const SimpleMaterial* mtl,
				  const SimpleSceneGraph* ssg);
	void writeObj(std::ostream& obj_fout,
				  const SimpleGeometryMesh& obj,
				  const SimpleGeometryMesh::Surface& surface);
	void writeCache(const SimpleSceneGraph* ssg);
};

}

#endif
