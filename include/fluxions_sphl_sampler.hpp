#ifndef FLUXIONS_SPHL_SAMPLER_HPP
#define FLUXIONS_SPHL_SAMPLER_HPP

#include <vector>
#include <fluxions_gte.hpp>
#include <fluxions_ssg_scene_graph.hpp>
#include <fluxions_sphl.hpp>

namespace Fluxions {
	class SphlSampler {
	public:
		SphlSampler();

		void resize(unsigned samplesX, unsigned samplesY);
		void saveSampleMap(const std::string& path, int pxscale);
		void sampleCubeMap(const Image4f& cubeMap, MultispectralSph4f& msph);

	private:
		const unsigned pxscale = 4;
		Image3i sampleMap;
		unsigned ix = 128;
		unsigned iy = 64;
		unsigned numSamples = 0;
		std::vector<Vector3f> randomVectors;
		std::vector<double> theta;
		std::vector<double> phi;
		std::vector<double> sph;
		std::vector<unsigned> debug_sides;
	};
} // namespace Fluxions

#endif
