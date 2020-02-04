#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_scalar_math.hpp>
#include <random>

namespace Fluxions {
	float randomSampler(float _min0, float _max0) {
		static std::random_device rd;
		static std::mt19937 s(rd());
		std::uniform_real_distribution<float> urd(_min0, _max0);

		return urd(s);
	}
} // namespace Fluxions
