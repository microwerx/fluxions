#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_bounding_box.hpp>

namespace Fluxions {
	template class TBoundingBox<float>;
	template class TBoundingBox<double>;
	template class TBoundingBox<int>;
} // namespace Fluxions
