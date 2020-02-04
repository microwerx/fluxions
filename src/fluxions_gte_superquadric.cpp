#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_superquadric.hpp>

namespace Fluxions {
	template class SuperQuadricEllipsoid<float>;
	template class SuperQuadricEllipsoid<double>;
	template class SuperQuadricToroid<float>;
	template class SuperQuadricToroid<double>;
} // namespace Fluxions
