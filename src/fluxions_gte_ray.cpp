#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_ray.hpp>

namespace Fluxions {
	// template specializations
	template class TRay<float>;
	template class TRay<double>;
	template class TRay<int>;
} // namespace Fluxions
