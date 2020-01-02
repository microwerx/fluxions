// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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

#include <fluxions_base.hpp>
#include <fluxions_file_system.hpp>
#include <fluxions_scene_graph_writer.hpp>
#include <fluxions_ssg_ssphh_renderer_plugin.hpp>
#include <unicornfish_corona_scene_file.hpp>
#include <unicornfish_corona_job.hpp>

namespace Uf
{
	namespace Fx = Fluxions;

	CoronaSceneFile::CoronaSceneFile() {}

	CoronaSceneFile::~CoronaSceneFile() {}

	void CoronaSceneFile::setCameraType(const std::string& newCameraType) {
		if (newCameraType == "perspective")
			cameraType_ = newCameraType;
		if (newCameraType == "cubemap")
			cameraType_ = newCameraType;
	}

	void CoronaSceneFile::setPerspectiveCamera(const Fluxions::Vector3f& origin, const Fluxions::Vector3f& target, const Fluxions::Vector3f& roll, const float horizontalFieldOfViewInDegrees) {
		cameraOrigin_ = origin;
		cameraTarget_ = target;
		cameraRoll_ = roll;
		cameraHFovInDegrees_ = horizontalFieldOfViewInDegrees;
		cameraType_ = "perspective";
	}

	void CoronaSceneFile::setCubeMapCamera(const Fluxions::Vector3f& origin,
										   const Fluxions::Vector3f& target,
										   const Fluxions::Vector3f& roll) {
		cameraOrigin_ = origin;
		cameraTarget_ = target;
		cameraRoll_ = roll;
		cameraType_ = "cubemap";
	}

	void CoronaSceneFile::writeSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setPerspectiveCamera(ssg.camera.origin(),
									ssg.camera.target(),
									ssg.camera.roll(),
									ssg.camera.fov);
		writer.export_path_prefix = CoronaJob::exportPathPrefix;
		writer.extra_tags.push_back({ "conffile", "../" + CoronaJob::confPathPrefix + "export_corona_ground_truth.conf" });
		ssg.Save(filename, &writer);
		return;
	}

	void CoronaSceneFile::writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::Vector3f cameraPosition = ssg.camera.actualViewMatrix.col4().xyz();
		writeCubeMapSCN(filename, ssg, cameraPosition);
	}

	void CoronaSceneFile::writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, const Fluxions::Vector3f& cameraPosition) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setCubeMapCamera(cameraPosition,
								cameraPosition + Fluxions::Vector3f(0.0f, 0.0f, -1.0f),
								Fluxions::Vector3f(0.0f, 1.0f, 0.0f));
		writer.export_path_prefix = CoronaJob::exportPathPrefix;
		writer.extra_tags.push_back({ "conffile", "../" + CoronaJob::confPathPrefix + "export_corona_ground_truth.conf" });
		ssg.Save(filename, &writer);
	}

	void CoronaSceneFile::writeSkySCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setCubeMapCamera(Fluxions::Vector3f(0.0f, 0.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 1.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 0.0f, 1.0f));
		writer.export_path_prefix = CoronaJob::exportPathPrefix;
		writer.extra_tags.push_back({ "conffile", "../" + CoronaJob::confPathPrefix + "ssphh_sky.conf" });
		writer.write_geometry = false;
		ssg.Save(filename, &writer);
	}

	bool CoronaSceneFile::writeSphlVizSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, int sourceLightIndex, int receivingLightIndex) {
		Fx::SSG_SSPHHRendererPlugin* ssphh = dynamic_cast<Fx::SSG_SSPHHRendererPlugin*>(ssg.userdata);
		if (!ssphh) {
			HFLOGERROR("ssphh pointer is null");
			return false;
		}

		int lastIndex = (int)ssphh->ssphhLights.size() - 1;
		if (!Fx::within(sourceLightIndex, -1, lastIndex) || !Fx::within(receivingLightIndex, -1, lastIndex)) {
			return false;
		}

		Fluxions::XmlSceneGraphWriter writer;
		writer.export_path_prefix = CoronaJob::exportPathPrefix;
		writer.extra_tags.push_back({ "conffile", "../" + CoronaJob::confPathPrefix + "sphlviz.conf" });

		// Camera
		if (receivingLightIndex >= 0) {
			const Fx::SimpleSSPHHLight& sphl2 = ssphh->ssphhLights[receivingLightIndex];
			Fluxions::Vector3f tmp = sphl2.position;
			Fluxions::Vector3f position(tmp.x, -tmp.z, tmp.y);
			Fluxions::Vector3f target(position.x, position.y - 1.0f, position.z);

			writer.setCubeMapCamera(position,
									target,
									Fluxions::Vector3f(0.0f, 0.0f, 1.0f));

		}

		if (sourceLightIndex >= 0) {
			writer.write_sun = false;
			const Fx::SimpleSSPHHLight& sphl1 = ssphh->ssphhLights[sourceLightIndex];
			Fluxions::Matrix4f lightMatrix(
				0.1f, 0.0f, 0.0f, sphl1.position.x,
				0.0f, 0.1f, 0.0f, -sphl1.position.z,
				0.0f, 0.0f, 0.1f, sphl1.position.y,
				0.0f, 0.0f, 0.0f, 1.0f);
			Fx::FilePathInfo fpi(CoronaJob::exportPathPrefix + "sphlviz.mtl");
			if (!fpi.Exists()) {
				HFLOGINFO("Writing out sphlviz.mtl");
				std::ofstream svout(CoronaJob::exportPathPrefix + "sphlviz.mtl");
				svout << "<mtlLib>"
					"<materialDefinition name = \"sphlSphereLight\">"
					"<material class = \"Native\">"
					"<emission>"
					"<color>100 100 100 </color>"
					"</emission>"
					"</material>"
					"</materialDefinition>"
					"</mtlLib>";
				svout.close();
			}

			std::ostringstream fout;
			Fx::XmlString(fout, "mtllib", "sphlviz.mtl", 1) << "\n";
			Fx::XmlBeginTag(fout, "geometryGroup", 1) << "\n";
			Fx::XmlBeginTag(fout, "object", "sphere", 2) << "\n";
			Fx::XmlInt(fout, "materialId", 0, 3) << "\n";
			Fx::XmlEndTag(fout, "object", 2) << "\n";
			Fx::XmlBeginTag(fout, "instance", 2) << "\n";
			Fx::XmlBeginTag(fout, "material", "Reference", 3) << "sphlSphereLight";
			Fx::XmlEndTag(fout, "material") << "\n";
			Fx::XmlMatrix4f(fout, "transform", lightMatrix, 3) << "\n";
			Fx::XmlEndTag(fout, "instance", 2) << "\n";
			Fx::XmlEndTag(fout, "geometryGroup", 1) << "\n";
			writer.extra_tags.push_back({ "", fout.str() });
		}
		ssg.Save(filename, &writer);
		return true;
	}

	void CoronaSceneFile::_writeCamera(std::ostream& ostr) {
		Fluxions::Matrix4f fixMatrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		Fx::XmlBeginTag(ostr, "camera", cameraType_, 1) << "\n";
		if (cameraType_ == "perspective") {
			Fluxions::Matrix4f fixed = cameraMatrix_ * fixMatrix;
			Fx::XmlMatrix4f(ostr, "tm", fixed, 2) << "\n";
			Fx::XmlFloat(ostr, "fov", cameraHFovInDegrees_, 2) << "\n";
		}
		if (cameraType_ == "cubemap") {
			Fx::XmlVector3f(ostr, "origin", cameraOrigin_, 2) << "\n";
			Fx::XmlVector3f(ostr, "target", cameraTarget_, 2) << "\n";
			Fx::XmlVector3f(ostr, "roll", cameraRoll_, 2) << "\n";
		}
		Fx::XmlEndTag(ostr, "camera", 1) << "\n\n";
	}

	void CoronaSceneFile::_writeSun(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg) {
		std::string lights_mtllib = "ssphh_lights.mtl";
		Fx::XmlString(ostr, "mtllib", lights_mtllib, 1) << "\n";
		Fx::XmlBeginTag(ostr, "sun", 1) << "\n";
		Fx::Vector3f sunDirTo(ssg.environment.curSunDirTo.x, -ssg.environment.curSunDirTo.z, ssg.environment.curSunDirTo.y);
		Fx::XmlVector3f(ostr, "dirTo", sunDirTo, 2) << "\n";
		Fx::XmlFloat(ostr, "turbidity", ssg.environment.pbsky.GetTurbidity(), 2) << "\n";
		Fx::XmlEndTag(ostr, "sun", 1) << "\n\n";

		ostr << "\t<environment>1 1 1<map class=\"Reference\">Skylight_environment</map></environment>" << "\n\n";

		const float size = 0.1f;
		for (unsigned i = 0; i < ssg.pointLights.size(); i++) {
			const Fx::SimplePointLight& pointLight = ssg.pointLights[i];
			Fluxions::Matrix4f lightMatrix(
				size, 0.0f, 0.0f, pointLight.position.x,
				0.0f, size, 0.0f, -pointLight.position.z,
				0.0f, 0.0f, size, pointLight.position.y,
				0.0f, 0.0f, 0.0f, 1.0f);
			// int tab = 0;
			std::ostringstream name;
			name << "PointLight" << i;

			Fx::XmlComment(ostr, name.str(), 1);
			Fx::XmlBeginTag(ostr, "geometryGroup", 1) << "\n";
			Fx::XmlBeginTag(ostr, "object", "sphere", 2) << "\n";
			Fx::XmlSize(ostr, "materialId", i, 3) << "\n";
			Fx::XmlEndTag(ostr, "object", 2) << "\n";
			Fx::XmlBeginTag(ostr, "instance", 2) << "\n";
			Fx::XmlBeginTag(ostr, "material", "Reference", 3) << "PointLight" << i;
			Fx::XmlEndTag(ostr, "material") << "\n";
			Fx::XmlMatrix4f(ostr, "transform", lightMatrix, 3) << "\n";
			Fx::XmlEndTag(ostr, "instance", 2) << "\n";
			Fx::XmlEndTag(ostr, "geometryGroup", 1) << "\n\n";
		}

		// Generate lights MTLLIB

		std::string lights_mtllib_path = CoronaJob::exportPathPrefix + ssg.name + "/" + lights_mtllib;
		HFLOGINFO("Writing lights_mtllib %s", lights_mtllib_path.c_str());
		std::ofstream fout(lights_mtllib_path);
		Fx::XmlBeginTag(fout, "mtlLib") << "\n";
		Fx::XmlBeginTag(fout, "mapDefinition", "name", "Skylight_environment", 1) << "\n";
		fout << "\t<map class = \"Sky\" originalMapClass = \"SkyShader\">\n";
		Fx::XmlVector3f(fout, "groundColor", ssg.environment.pbsky.GetGroundAlbedo().ToVector3(), 2) << "\n";
		Fx::XmlEndTag(fout, "map", 1) << "\n";
		Fx::XmlEndTag(fout, "mapDefinition", 1) << "\n\n";

		for (unsigned i = 0; i < ssg.pointLights.size(); i++) {
			std::ostringstream name;
			name << "PointLight" << i;
			Fx::XmlBeginTag(fout, "materialDefinition", "name", name.str(), 1) << "\n";
			Fx::XmlBeginTag(fout, "material", "Native", 2) << "\n";
			Fx::XmlBeginTag(fout, "emission", 3) << "\n";
			Fx::XmlVector3f(fout, "color", Fluxions::Vector3f(ssg.pointLights[i].E0 / size), 4) << "\n";
			Fx::XmlEndTag(fout, "emission", 3) << "\n";
			Fx::XmlEndTag(fout, "material", 2) << "\n";
			Fx::XmlEndTag(fout, "materialDefinition", 1) << "\n\n";
		}

		Fx::XmlEndTag(fout, "mtlLib") << "\n";
		fout.close();
	}

	void CoronaSceneFile::clearCache() {
		geometryGroups_.clear();
	}

	void CoronaSceneFile::writeCache(const Fluxions::SimpleSceneGraph& ssg) {
		std::string basePath = CoronaJob::exportPathPrefix + ssg.name;
		Fx::FilePathInfo ssgfpi(basePath);
		if (!ssgfpi.IsDirectory()) {
			HFLOGERROR("Cannot export, '%s' is a regular file", ssgfpi.path.c_str());
			return;
		}
		if (ssgfpi.DoesNotExist()) {
			std::filesystem::path ssgpath(basePath);
			std::filesystem::path ssgassetspath(basePath + "/assets");
			std::filesystem::path ssggeometrypath(basePath + "/geometry");
			if (!std::filesystem::create_directory(ssgpath)) return;
			if (!std::filesystem::create_directory(ssgassetspath)) return;
			if (!std::filesystem::create_directory(ssggeometrypath)) return;
		}
		writeGeometryCache(ssg);
		writeMaterialCache(ssg);
	}

	void CoronaSceneFile::writeGeometryCache(const Fluxions::SimpleSceneGraph& ssg) {
		if (!geometryGroups_.empty()) return;
		int obj_count = 0;
		for (auto& [objname, mesh] : ssg.staticMeshes) {
			for (auto& [mtlname, mtllib] : mesh.Materials) {
				std::ostringstream obj_pathname;
				obj_pathname << CoronaJob::exportPathPrefix + "geometry/";
				obj_pathname << ssg.name << "_";
				obj_pathname << std::setw(3) << std::setfill('0') << obj_count << "_";
				obj_pathname << "object_" << objname << "_";
				obj_pathname << mtlname << ".obj";
				std::string OBJpath = obj_pathname.str();
				HFLOGINFO("Writing out '%s'", OBJpath.c_str());
				mesh.saveOBJ(OBJpath);

				auto& group = ssg.geometryGroups[objname];

				const Fluxions::Matrix4f fixMatrix(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, -1.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				std::string objectPath = obj_pathname.str();
				Fluxions::Matrix4f transform = fixMatrix * group.worldMatrix();
				geometryGroups_.push_back(string_string_Matrix4f(objectPath, mtlname, transform));

				obj_count++;
			}
		}
	}

	void CoronaSceneFile::writeMaterialCache(const Fluxions::SimpleSceneGraph& ssg) {
		std::string basePath = CoronaJob::exportPathPrefix + ssg.name + "/";
		std::string MTLpath = basePath + "materials.mtl";
		std::string CoronaMTLpath = basePath + "materials_corona.mtl";
		std::string CoronaMapsPath = basePath + "materials_corona-maps.mtl";

		HFLOGINFO("Writing out '%s'", MTLpath.c_str());
		ssg.materials.saveMTL(MTLpath);

		HFLOGINFO("Writing out '%s'", CoronaMTLpath.c_str());
		int mtl_flags = Fx::SimpleMaterialLibrary::WRITE_MATERIALS;
		if (enableKs) mtl_flags |= Fx::SimpleMaterialLibrary::WRITE_SPECULAR;
		ssg.materials.saveXml(CoronaMTLpath, mtl_flags);
		ssg.materials.saveXml(CoronaMapsPath, Fx::SimpleMaterialLibrary::WRITE_MAPS);

		for (const auto& [mapname, mappath] : ssg.materials.maps) {
			Fx::FilePathInfo fpi(mappath);
			std::string asset_path = basePath + "assets/" + fpi.fullfname;
			std::filesystem::copy_file(mappath, asset_path);
		}
	}

	void CoronaSceneFile::_writeGeometryGroups(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg) {
		writeCache(ssg);
		Fx::XmlBeginTag(ostr, "mtllib", 1);
		ostr << "materials_corona.mtl";
		Fx::XmlEndTag(ostr, "mtllib");
		ostr << "\n";
		for (auto& gg : geometryGroups_) {
			Fx::XmlBeginTag(ostr, "geometryGroup", 1) << "\n";
			Fx::XmlBeginTag(ostr, "object", "file", 2) << std::get<0>(gg);
			Fx::XmlEndTag(ostr, "object") << "\n";
			Fx::XmlBeginTag(ostr, "instance", 2) << "\n";
			Fx::XmlBeginTag(ostr, "material", "Reference", 3) << std::get<1>(gg);
			Fx::XmlEndTag(ostr, "material", 0) << "\n";
			Fx::XmlMatrix4f(ostr, "transform", std::get<2>(gg), 3) << "\n";
			Fx::XmlEndTag(ostr, "instance", 2) << "\n";
			Fx::XmlEndTag(ostr, "geometryGroup", 1) << "\n";
		}
	}
} // namespace Uf