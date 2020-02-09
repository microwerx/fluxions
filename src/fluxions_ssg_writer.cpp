#include "fluxions_ssg_pch.hpp"
#include <filesystem>
#include <fluxions_ssg_writer.hpp>
#include <fluxions_ssg_scene_graph.hpp>

namespace Fluxions {
	const Fluxions::Matrix4f notusedfixMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	bool SceneGraphWriter::open(const char* path) {
		fout.open(path);
		if (!fout)
			return false;
		return true;
	}

	void SceneGraphWriter::close() {
		fout.close();
	}

	bool SceneGraphWriter::write(const SimpleSceneGraph* pssg) {
		if (!pssg)
			return false;
		return pssg->write(fout);
	}

	// XML SCENE GRAPH WRITER

	bool make_path(const std::filesystem::path& path) {
		if (!std::filesystem::exists(path)) {
			if (!std::filesystem::is_directory(path)) {
				std::filesystem::create_directory(path);
			}
		}
		return std::filesystem::is_directory(path);
	}

	bool XmlSceneGraphWriter::open(const char* path) {
		if (!make_path(export_path_prefix)) {
			HFLOGERROR("'%s' is a file or cannot be created", export_path_prefix.c_str());
			return false;
		}
		if (!make_path(export_path_prefix + "assets")) {
			HFLOGERROR("'%sassets' is a file or cannot be created", export_path_prefix.c_str());
			return false;
		};
		if (!make_path(export_path_prefix + "geometry")) {
			HFLOGERROR("'%sgeometry' is a file or cannot be created", export_path_prefix.c_str());
			return false;
		}
		fout.open(export_path_prefix + path);
		if (!fout)
			return false;
		return true;
	}

	bool XmlSceneGraphWriter::write(const SimpleSceneGraph* ssg) {
		if (!fout)
			return false;

		XmlBeginTag(fout, "scene") << "\n";

		for (string_string& ss : extra_tags) {
			if (ss.first == "") {
				fout << ss.second << "\n";
			}
			else {
				XmlString(fout, ss.first, ss.second, 1) << "\n";
			}
		}

		if (!camera_specified_) {
			setPerspectiveCamera(ssg->camera.origin(),
								 ssg->camera.target(),
								 ssg->camera.roll(),
								 ssg->camera.fov);
		}

		// Camera
		_writeCamera(fout);

		// Sun
		if (write_sun) {
			_writeSun(fout, *ssg);
		}

		// Geometry Groups
		if (write_geometry) {
			_writeGeometryGroups(fout, *ssg);
		}

		XmlEndTag(fout, "scene") << "\n";

		return true;
	}

	void XmlSceneGraphWriter::setPerspectiveCamera(const Vector3f& origin,
												   const Vector3f& target,
												   const Vector3f& roll,
												   const float horizontalFieldOfViewInDegrees) {
		cameraOrigin_ = origin;
		cameraTarget_ = target;
		cameraRoll_ = roll;
		cameraHFovInDegrees_ = horizontalFieldOfViewInDegrees;
		cameraType_ = "perspective";
		camera_specified_ = true;
	}

	void XmlSceneGraphWriter::setCubeMapCamera(const Vector3f& origin,
											   const Vector3f& target,
											   const Vector3f& roll) {
		cameraOrigin_ = origin;
		cameraTarget_ = target;
		cameraRoll_ = roll;
		cameraType_ = "cubemap";
		camera_specified_ = true;
	}

	void XmlSceneGraphWriter::_writeCamera(std::ostream& ostr) {
		XmlBeginTag(ostr, "camera", cameraType_, 1) << "\n";
		if (cameraType_ == "perspective") {
			XmlVector3f(ostr, "origin", cameraOrigin_, 2) << "\n";
			XmlVector3f(ostr, "target", cameraTarget_, 2) << "\n";
			XmlVector3f(ostr, "roll", cameraRoll_, 2) << "\n";
			XmlFloat(ostr, "fov", cameraHFovInDegrees_, 2) << "\n";
		}
		if (cameraType_ == "cubemap") {
			XmlVector3f(ostr, "origin", cameraOrigin_, 2) << "\n";
			XmlVector3f(ostr, "target", cameraTarget_, 2) << "\n";
			XmlVector3f(ostr, "roll", cameraRoll_, 2) << "\n";
		}
		XmlEndTag(ostr, "camera", 1) << "\n\n";
	}

	void XmlSceneGraphWriter::_writeSun(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg) {
		XmlString(ostr, "mtllib", lights_mtllib, 1) << "\n";
		XmlBeginTag(ostr, "sun", 1) << "\n";
		Vector3f sunDirTo(ssg.environment.curSunDirTo.x, ssg.environment.curSunDirTo.y, ssg.environment.curSunDirTo.z);
		XmlVector3f(ostr, "dirTo", sunDirTo, 2) << "\n";
		XmlVector3f(ostr, "dirUp", { 0.0f, 1.0f, 0.0f }, 2) << "\n";
		XmlFloat(ostr, "turbidity", ssg.environment.pbsky.GetTurbidity(), 2) << "\n";
		XmlEndTag(ostr, "sun", 1) << "\n\n";

		ostr << "\t<environment>1 1 1<map class=\"Reference\">Skylight_environment</map></environment>"
			<< "\n\n";

		constexpr float size = 1.0f/0.1f;
		for (unsigned i = 0; i < ssg.pointLights.size(); i++) {
			const SimplePointLight& pointLight = ssg.pointLights[i];
			Fluxions::Matrix4f lightMatrix(
				size, 0.0f, 0.0f, pointLight.position.x,
				0.0f, size, 0.0f, -pointLight.position.z,
				0.0f, 0.0f, size, pointLight.position.y,
				0.0f, 0.0f, 0.0f, 1.0f);
			// int tab = 0;
			std::ostringstream name;
			name << "PointLight" << i;

			XmlComment(ostr, name.str(), 1);
			XmlBeginTag(ostr, "geometryGroup", 1) << "\n";
			XmlBeginTag(ostr, "object", "sphere", 2) << "\n";
			XmlSize(ostr, "materialId", i, 3) << "\n";
			XmlEndTag(ostr, "object", 2) << "\n";
			XmlBeginTag(ostr, "instance", 2) << "\n";
			XmlBeginTag(ostr, "material", "Reference", 3) << "PointLight" << i;
			XmlEndTag(ostr, "material") << "\n";
			XmlMatrix4f(ostr, "transform", lightMatrix, 3) << "\n";
			XmlEndTag(ostr, "instance", 2) << "\n";
			XmlEndTag(ostr, "geometryGroup", 1) << "\n\n";
		}

		// Generate lights MTLLIB

		std::string lights_mtllib_path = export_path_prefix + lights_mtllib;
		HFLOGINFO("Writing lights_mtllib %s", lights_mtllib_path.c_str());
		std::ofstream lights_mtllib_fout(lights_mtllib_path);
		XmlBeginTag(lights_mtllib_fout, "mtlLib") << "\n";
		XmlBeginTag(lights_mtllib_fout, "mapDefinition", "name", "Skylight_environment", 1) << "\n";
		lights_mtllib_fout << "\t<map class = \"Sky\" originalMapClass = \"SkyShader\">\n";
		XmlVector3f(lights_mtllib_fout, "groundColor", ssg.environment.pbsky.GetGroundAlbedo().ToVector3(), 2) << "\n";
		XmlEndTag(lights_mtllib_fout, "map", 1) << "\n";
		XmlEndTag(lights_mtllib_fout, "mapDefinition", 1) << "\n";

		for (unsigned i = 0; i < ssg.pointLights.size(); i++) {
			std::ostringstream name;
			name << "PointLight" << i;
			XmlBeginTag(lights_mtllib_fout, "materialDefinition", "name", name.str(), 1) << "\n";
			XmlBeginTag(lights_mtllib_fout, "material", "Native", 2) << "\n";
			XmlBeginTag(lights_mtllib_fout, "emission", 3) << "\n";
			XmlVector3f(lights_mtllib_fout, "color", Fluxions::Vector3f(ssg.pointLights[i].E0.ToVector3() * size), 4) << "\n";
			XmlEndTag(lights_mtllib_fout, "emission", 3) << "\n";
			XmlEndTag(lights_mtllib_fout, "material", 2) << "\n";
			XmlEndTag(lights_mtllib_fout, "materialDefinition", 1) << "\n";
		}

		XmlEndTag(lights_mtllib_fout, "mtlLib") << "\n";
		lights_mtllib_fout.close();
	}

	void XmlSceneGraphWriter::_clearCache() {
		geometryGroups_.clear();
	}

	void XmlSceneGraphWriter::_writeCache(const Fluxions::SimpleSceneGraph& ssg) {
		if (clearCache) {
			_clearCache();
			clearCache = false;
		}
		_writeGeometryCache(ssg);
		_writeMaterialCache(ssg);
	}

	void XmlSceneGraphWriter::_writeGeometryCache(const Fluxions::SimpleSceneGraph& ssg) {
		if (!geometryGroups_.empty())
			return;
		int obj_count = 0;
		for (auto& [objname, mesh] : ssg.staticMeshes) {
			for (auto& [mtlname, mtllib] : mesh.Materials) {
				std::string idmtlname = mtlname;
				toidentifier(idmtlname);
				tolower(idmtlname);
				std::string idobjname = ssg.staticMeshes.getNameFromHandle(objname);
				toidentifier(idobjname);
				tolower(idobjname);

				std::ostringstream obj_pathname;
				obj_pathname << "geometry/";
				obj_pathname << ssg.name() << "_";
				//obj_pathname << std::setw(3) << std::setfill('0') << obj_count << "_";
				obj_pathname << "object_" << idobjname << "_";
				obj_pathname << idmtlname << ".obj";
				std::string objectPath = export_path_prefix + obj_pathname.str();
				if (!std::filesystem::exists(objectPath)) {
					HFLOGINFO("Writing out '%s'", obj_pathname.str().c_str());
					mesh.saveOBJByMaterial(objectPath, mtlname, 1);
				}

				auto& group = ssg.geometryGroups[objname];

				Fluxions::Matrix4f transform = group.worldMatrix();
				geometryGroups_.push_back(string_string_Matrix4f(obj_pathname.str(), mtlname, transform));

				obj_count++;
			}
		}
	}

	void XmlSceneGraphWriter::_writeMaterialCache(const Fluxions::SimpleSceneGraph& ssg) {
		std::string MTLpath = export_path_prefix + "materials.mtl";
		std::string CoronaMTLpath = export_path_prefix + "materials_corona.mtl";
		std::string CoronaMapsPath = export_path_prefix + "materials_corona-maps.mtl";

		HFLOGINFO("Writing out '%s'", MTLpath.c_str());
		ssg.materials.saveMTL(MTLpath);

		HFLOGINFO("Writing out '%s'", CoronaMTLpath.c_str());
		int mtl_flags = SimpleMaterialLibrary::WRITE_MATERIALS;
		if (enableKs)
			mtl_flags |= SimpleMaterialLibrary::WRITE_SPECULAR;
		ssg.materials.saveXml(CoronaMTLpath, mtl_flags);
		ssg.materials.saveXml(CoronaMapsPath, SimpleMaterialLibrary::WRITE_MAPS);

		for (const auto& [mapname, mappath] : ssg.materials.maps) {
			FilePathInfo fpi(mappath);
			std::string asset_path = export_path_prefix + "assets/" + fpi.filename();
			if (std::filesystem::exists(asset_path))
				continue;
			if (std::filesystem::exists(mappath)) {
				try {
					std::filesystem::copy_file(mappath, asset_path);
				}
				catch (const std::filesystem::filesystem_error & e) {
					HFLOGERROR("file cannot be copied: '%s'", mappath.c_str());
					HFLOGERROR("system reported '%s'", e.what());
				}
			}
			else {
				HFLOGWARN("file does not exist: '%s'", mappath.c_str());
			}
		}
	}

	void XmlSceneGraphWriter::_writeGeometryGroups(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg) {
		_writeCache(ssg);
		XmlBeginTag(ostr, "mtllib", 1);
		ostr << "materials_corona-maps.mtl";
		XmlEndTag(ostr, "mtllib") << "\n";
		XmlBeginTag(ostr, "mtllib", 1);
		ostr << "materials_corona.mtl";
		XmlEndTag(ostr, "mtllib") << "\n";
		ostr << "\n";
		for (auto& gg : geometryGroups_) {
			XmlBeginTag(ostr, "geometryGroup", 1) << "\n";

			XmlBeginTag(ostr, "instance", 2) << "\n";
			XmlBeginTag(ostr, "material", "Reference", 3) << std::get<1>(gg);
			XmlEndTag(ostr, "material", 0) << "\n";
			XmlMatrix4f(ostr, "transform", std::get<2>(gg), 3) << "\n";
			XmlEndTag(ostr, "instance", 2) << "\n";

			XmlBeginTag(ostr, "object", "file", 2) << std::get<0>(gg);
			XmlEndTag(ostr, "object") << "\n";

			XmlEndTag(ostr, "geometryGroup", 1) << "\n\n";
		}
	}
} // namespace Fluxions
