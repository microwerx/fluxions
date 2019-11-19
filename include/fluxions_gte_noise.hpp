#ifndef FLUXIONS_GTE_NOISE_HPP
#define FLUXIONS_GTE_NOISE_HPP

#include <random>
#include <fluxions_gte_scalar_math.hpp>

namespace Fluxions
{
	class Noise {
	public:
		Noise() {
			gen.seed(rd());
			std::uniform_real_distribution<float> urd(0.0f, 1.0f);
			
			samples.resize(MAX_SAMPLES);
			for (auto& s : samples) {
				s = urd(gen);
			}
		}

		~Noise() {}

		inline float noise(float x) const {
			int ix = int(x);
			float t = x - ix;
			float s1 = samples[ix % MAX_SAMPLES];
			float s2 = samples[(ix + 1) % MAX_SAMPLES];
			return smootherlerp(t, s1, s2);
		}

		inline float snoise(float x) const {
			return 2.0f * noise(x) - 1.0f;
		}

		float fractal1(float x, float minfreq, float maxfreq) const {
			float value = 0.0f;
			for (float f = minfreq; f <= maxfreq; f *= 2.0f) {
				value += snoise(x * f) / f;
			}
			return value;
		}

		float turbulence1(float x, float minfreq, float maxfreq) const {
			float value = 0.0f;
			for (float f = minfreq; f <= maxfreq; f *= 2.0f) {
				value += fabsf(snoise(x * f)) / f;
			}
			return value;
		}
	private:	
		std::vector<float> samples;
		std::random_device rd;
		std::mt19937 gen;
		static constexpr unsigned MAX_SAMPLES = 256;
	};
}

#endif