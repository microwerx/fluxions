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
									ssg.camera.fov);
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", "export_corona_ground_truth.conf" });
		writer.enableKs = enableKs;
		ssg.Save(fpi.fullfname, &writer);
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
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", "export_corona_ground_truth.conf" });
		writer.enableKs = enableKs;
		ssg.Save(fpi.fullfname, &writer);
	}

	void CoronaSceneFile::writeSkySCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg) {
		Fluxions::XmlSceneGraphWriter writer;
		writer.setCubeMapCamera(Fluxions::Vector3f(0.0f, 0.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 1.0f, 0.0f),
								Fluxions::Vector3f(0.0f, 0.0f, 1.0f));
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", "ssphh_sky.conf" });
		writer.enableKs = enableKs;
		writer.write_geometry = false;
		ssg.Save(fpi.fullfname, &writer);
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
		Fx::FilePathInfo fpi(filename);
		writer.export_path_prefix = fpi.dir;
		writer.extra_tags.push_back({ "conffile", "sphlviz.conf" });
		writer.enableKs = enableKs;

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
			Fx::XmlMatrix4f(fout, "transform", lightMatrix, 3) << "\n";
			Fx::XmlEndTag(fout, "instance", 2) << "\n";
			Fx::XmlEndTag(fout, "geometryGroup", 1) << "\n";
			writer.extra_tags.push_back({ "", fout.str() });
		}
		ssg.Save(fpi.fullfname, &writer);
		return true;
	}

} // namespace Uf