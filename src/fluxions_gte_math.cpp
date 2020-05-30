#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_scalar_math.hpp>
#include <random>

namespace Fluxions {
	namespace {
		std::random_device rd;
		std::mt19937 s(rd());
	}

	float randomSampler(float _min0, float _max0) {
		std::uniform_real_distribution<float> urd(_min0, _max0);

		return urd(s);
	}
} // namespace Fluxions
