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
#ifndef FLUXIONS_CORONA_SCENE_FILE_HPP
#define FLUXIONS_CORONA_SCENE_FILE_HPP

#include <fluxions_simple_scene_graph.hpp>

namespace Uf
{
	class CoronaSceneFile
	{
	public:
		CoronaSceneFile();
		~CoronaSceneFile();

		using Vector3f = Fluxions::Vector3f;
		using Matrix4f = Fluxions::Matrix4f;

		// newCameraType = { "perspective" | "cubemap" }
		void setCameraType(const std::string& newCameraType);

		// returns "perspective" or "cubemap"
		std::string getCameraType() const { return cameraType_; }

		void setPerspectiveCamera(const Vector3f& origin, const Vector3f& target, const Vector3f& roll, const float hfovInDegrees);
		void setCubeMapCamera(const Vector3f& origin, const Vector3f& target, const Vector3f& roll);

		void writeSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		void writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		void writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, const Fluxions::Vector3f& cameraPosition);
		void writeSkySCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		bool writeSphlVizSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, int sourceLightIndex, int receivingLightIndex);

		void clearCache();
		void writeCache(const Fluxions::SimpleSceneGraph& ssg);
		void writeGeometryCache(const Fluxions::SimpleSceneGraph& ssg);
		void writeMaterialCache(const Fluxions::SimpleSceneGraph& ssg);

		bool enableKs = false;

	private:
		// cache
		using string_string_Matrix4f = std::tuple<std::string, std::string, Fluxions::Matrix4f>;
		std::vector<string_string_Matrix4f> geometryGroups_;
		
		std::string cameraType_ = "perspective";
		Vector3f cameraOrigin_;
		Vector3f cameraTarget_;
		Vector3f cameraRoll_;
		Matrix4f cameraMatrix_;
		float cameraHFovInDegrees_{ 45.0f };

		void _writeCamera(std::ostream& ostr);
		void _writeSun(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg);
		void _writeGeometryGroups(std::ostream& ostr, const Fluxions::SimpleSceneGraph& ssg);
	};
} // namespace Uf

#endif
