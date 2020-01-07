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

#include <fluxions_scene_graph_writer.hpp>

namespace Uf
{
	struct CoronaConfig {
		int maxRayDepth{ 5 };
		int passLimit{ 1 };
		bool clearCache{ false };
		bool enableHQ{ false };
		bool enableHDR{ false };
		bool enableSpecular{ false };
		bool usePreviousRun{ false };
		float simpleExposure{ 0.0f };
		Fluxions::Vector2i resolution{ 0, 0 };
		std::string confname;

		void write(const std::string& path, const std::string& cameraType);
	};

	static constexpr int DefaultHighQualityMaxRayDepth = 25;
	static constexpr int DefaultMedQualityMaxRayDepth = 5;
	static constexpr int DefaultLowQualityMaxRayDepth = 2;

	static constexpr int DefaultHighQualityPassLimit = 25;
	static constexpr int DefaultMedQualityPassLimit = 5;
	static constexpr int DefaultLowQualityPassLimit = 1;

	class CoronaSceneFile {
	public:
		CoronaSceneFile();
		~CoronaSceneFile();

		void stashConfig() { stash_ = currentConfig; }
		void restoreConfig() { currentConfig = stash_; }

		void writeSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		void writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		void writeCubeMapSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, const Fluxions::Vector3f& cameraPosition);
		void writeSkySCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg);
		bool writeSphlGenSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, unsigned lightIndex);
		bool writeSphlVizSCN(const std::string& filename, const Fluxions::SimpleSceneGraph& ssg, int sourceLightIndex, int receivingLightIndex);

		CoronaConfig currentConfig{ DefaultMedQualityMaxRayDepth, DefaultLowQualityPassLimit, false, false, true, false, false };
		CoronaConfig REF{ DefaultHighQualityMaxRayDepth, DefaultHighQualityPassLimit, true, true, true, true, false };
		CoronaConfig VIZ{ DefaultMedQualityMaxRayDepth, DefaultLowQualityPassLimit, false, false, true, false, true };
		CoronaConfig GEN{ DefaultMedQualityMaxRayDepth, DefaultLowQualityPassLimit, false, false, true, false, false };
		CoronaConfig SKY{ DefaultMedQualityMaxRayDepth, DefaultLowQualityPassLimit, false, false, true, false, false };

		int lastFrame{ 0 };
		std::string lastSCN;
		std::string lastCubeSCN;
		std::string lastSkySCN;
		std::string lastSphlVizSCN;
	private:
		CoronaConfig stash_;
		void _copyConf(const CoronaConfig& config, Fluxions::XmlSceneGraphWriter& writer);
	};
} // namespace Uf

#endif
