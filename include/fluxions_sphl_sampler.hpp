// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#ifndef FLUXIONS_SPHL_SAMPLER_HPP
#define FLUXIONS_SPHL_SAMPLER_HPP

#include <vector>
#include <fluxions_gte.hpp>
#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_sphl.hpp>

using namespace std;
namespace Fluxions
{
class SphlSampler
{
  public:
	SphlSampler();

	void resize(int samplesX, int samplesY);
	void saveSampleMap(const string &path, int pxscale);
	void sampleCubeMap(const Image4f &cubeMap, MultispectralSph4f &msph);

  private:
	const size_t pxscale = 4;
	Image3i sampleMap;
	size_t ix = 128; // 628;
	size_t iy = 64;  // 314;
	size_t numSamples = 0;
	vector<Vector3f> randomVectors;
	vector<double> theta;
	vector<double> phi;
	vector<double> sph;
	vector<int> debug_sides;
};
} // namespace Fluxions

#endif
