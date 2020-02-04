#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_rect.hpp>

namespace Fluxions {
	template class TRect<float>;
	template class TRect<double>;
	template class TRect<short>;
	template class TRect<unsigned short>;
	template class TRect<int>;
	template class TRect<unsigned int>;
} // namespace Fluxions
