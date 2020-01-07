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

	void CoronaSceneFile::writeSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setPerspectiveCamera(ssg.camera.origin(),
									ssg.camera.target(),
									ssg.camera.roll(),
									ssg.camera.actualFov);
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", Uf::CoronaJob::confPathPrefix + "export_corona_ground_truth.conf" });
		currentConfig.confname = fpi.fname + ".conf";
		_copyConf(currentConfig, writer);
		ssg.Save(fpi.fullfname, &writer);
		currentConfig.write(fpi.dir + currentConfig.confname, "perspective");
	}

	void CoronaSceneFile::writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::Vector3f cameraPosition = -ssg.camera.actualViewMatrix.col4().xyz();
		writeCubeMapSCN(filename, ssg, cameraPosition);
	}

	void CoronaSceneFile::writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, const Fluxions::Vector3f& cameraPosition) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setCubeMapCamera(cameraPosition,
								cameraPosition + Fluxions::Vector3f(0.0f, 0.0f, -1.0f),
								Fluxions::Vector3f(0.0f, 1.0f, 0.0f));
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", Uf::CoronaJob::confPathPrefix + "export_corona_ground_truth_cube.conf" });
		currentConfig.confname = fpi.fname + ".conf";
		_copyConf(currentConfig, writer);
		ssg.Save(fpi.fullfname, &writer);
		currentConfig.write(fpi.dir + currentConfig.confname, "cubemap");
	}

	void CoronaSceneFile::writeSkySCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setCubeMapCamera(Fluxions::Vector3f(0.0f, 0.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 1.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 0.0f, 1.0f));
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", Uf::CoronaJob::confPathPrefix + "ssphh_sky.conf" });
		writer.write_geometry = false;
		SKY.confname = "ssphh_sky.conf";
		_copyConf(SKY, writer);
		ssg.Save(fpi.fullfname, &writer);
		SKY.write(fpi.dir + SKY.confname, "cubemap");
	}

	bool CoronaSceneFile::writeSphlGenSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, unsigned lightIndex) {
		if (!ssg.anisoLights.isAHandle(lightIndex + 1)) return false;
		const Fluxions::SimpleAnisoLight& light = ssg.anisoLights[lightIndex + 1];

		Fluxions::XmlSceneGraphWriter writer;
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;

		Fluxions::Vector3f origin = light.position.xyz();
		Fluxions::Vector3f target = origin - Fluxions::Vector3f(0.0f, 0.0f, 1.0f);
		Fluxions::Vector3f roll = { 0.0f, 1.0f, 0.0f };
		writer.setCubeMapCamera(origin, target, roll);

		GEN.confname = fpi.fname + ".conf";;
		_copyConf(GEN, writer);
		ssg.Save(fpi.fullfname, &writer);
		GEN.write(fpi.dir + GEN.confname, "cubemap");
		return true;
	}

	bool CoronaSceneFile::writeSphlVizSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, int sourceLightIndex, int receivingLightIndex) {
		if (!ssg.anisoLights.isAHandle(sourceLightIndex + 1)) return false;
		if (!ssg.anisoLights.isAHandle(receivingLightIndex + 1)) return false;
		const Fluxions::SimpleAnisoLight& src_sphl = ssg.anisoLights[sourceLightIndex + 1];
		const Fluxions::SimpleAnisoLight& rcv_sphl = ssg.anisoLights[receivingLightIndex + 1];

		Fluxions::XmlSceneGraphWriter writer;
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", Uf::CoronaJob::confPathPrefix + "sphlviz.conf" });

		Fluxions::Vector3f src_sphl_position = src_sphl.position.xyz();
		Fluxions::Vector3f src_sphl_target = src_sphl_position - Fluxions::Vector3f(0.0f, 0.0f, 1.0f);
		Fluxions::Vector3f src_sphl_roll = { 0.0f, 1.0f, 0.0f };
		writer.setCubeMapCamera(src_sphl_position,
								src_sphl_target,
								src_sphl_roll);

		writer.write_sun = false;

		Fluxions::Vector3f rcv_sphl_position = rcv_sphl.position.xyz();
		Fluxions::Matrix4f lightMatrix = Fluxions::Matrix4f::MakeTranslation(rcv_sphl_position);

		if (!fpi.Exists()) {
			HFLOGINFO("Writing out sphlviz.mtl");
			std::ofstream svout("sphlviz.mtl");
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
		Fx::XmlAffineMatrix4f(fout, "transform", lightMatrix, 3) << "\n";
		Fx::XmlEndTag(fout, "instance", 2) << "\n";
		Fx::XmlEndTag(fout, "geometryGroup", 1) << "\n";
		writer.extra_tags.push_back({ "", fout.str() });

		VIZ.confname = fpi.fname + ".conf";
		_copyConf(VIZ, writer);
		ssg.Save(fpi.fullfname, &writer);
		VIZ.write(fpi.dir + VIZ.confname, "cubemap");
		return true;
	}


	void CoronaSceneFile::_copyConf(const CoronaConfig& config, Fluxions::XmlSceneGraphWriter& writer) {
		writer.enableKs = config.enableSpecular;
		writer.clearCache = config.clearCache;
		writer.extra_tags.push_back({ "conffile", config.confname });
	}


	void CoronaConfig::write(const std::string& path, const std::string& cameraType) {
		if (path.empty()) return;
		std::ofstream conf(path);
		if (resolution.lengthSquared() > 1) {
			int w = resolution.x;
			int h = resolution.y;
			if (cameraType == "cubemap") {
				w = h * 6;
			}
			if (enableHQ) {
				w *= 2;
				h *= 2;
			}
			conf << "       Int image.width  = " << w << "\n";
			conf << "       Int image.height = " << h << "\n";
		}
		conf << "     Float colorMap.simpleExposure = " << simpleExposure << "\n";
		conf << "       Int shading.maxRayDepth     = " << maxRayDepth << "\n";
		conf << "       Int progressive.passLimit   = " << passLimit << "\n";
	}
} // namespace Uf