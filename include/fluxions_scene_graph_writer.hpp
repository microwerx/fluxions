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
		bool open(const char* path) override;

		// Writes out the sequence
		// extra_tags
		// camera
		// lights
		// geometryGroups_ groups
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
		// geometryGroups_ groups
		using string_string = std::pair<std::string, std::string>;

		// If first string is "", then the second is interpreted as already formatted
		std::vector<string_string> extra_tags;

		bool write_sun{ true };
		bool write_geometry{ true };
		bool enableKs{ true };
		bool clearCache{ true };
		std::string export_path_prefix{ "./xml_scn_output/" };
	private:
		std::string cameraType_ = "perspective";
		Vector3f cameraOrigin_;
		Vector3f cameraTarget_;
		Vector3f cameraRoll_;
		Matrix4f cameraMatrix_;
		float cameraHFovInDegrees_{ 45.0f };

		bool camera_specified_ = false;

		std::string lights_mtllib{ "ssphh_lights.mtl" };

		using string_string_Matrix4f = std::tuple<std::string, std::string, Matrix4f>;
		std::vector<string_string_Matrix4f> geometryGroups;

		// cache
		void _writeCamera(std::ostream& ostr);
		void _writeSun(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg);
		void _writeGeometryGroups(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg);		

		std::vector<string_string_Matrix4f> geometryGroups_;
		std::string basePath_;
		std::string basePathDir_;
		void _clearCache();
		void _writeCache(const Fluxions::SimpleSceneGraph& ssg);
		void _writeGeometryCache(const Fluxions::SimpleSceneGraph& ssg);
		void _writeMaterialCache(const Fluxions::SimpleSceneGraph& ssg);
	};

}

#endif
