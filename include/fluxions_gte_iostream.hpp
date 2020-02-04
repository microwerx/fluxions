#ifndef FLUXIONS_GTE_IOSTREAM_HPP
#define FLUXIONS_GTE_IOSTREAM_HPP

#include <ostream>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions {
	// Output Routines
	template <typename T>
	std::ostream& operator<<(std::ostream& ostr, const TVector2<T>& V) {
		ostr << V.x << " " << V.y;
		return ostr;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& ostr, const TVector3<T>& V) {
		ostr << V.x << " " << V.y << " " << V.z;
		return ostr;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& ostr, const TVector4<T>& V) {
		ostr << V.x << " " << V.y << " " << V.z << " " << V.w;
		return ostr;
	}

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3f& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3d& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3b& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3ub& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3s& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3us& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3i& V);
	extern template std::ostream& operator<<(std::ostream& ostr, const Vector3ui& V);
#endif
} // namespace Fluxions

#endif
