#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_iostream.hpp>

namespace Fluxions {
	template std::ostream& operator<<(std::ostream& ostr, const Vector3f& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3d& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3b& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3ub& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3s& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3us& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3i& V);
	template std::ostream& operator<<(std::ostream& ostr, const Vector3ui& V);
} // namespace Fluxions
