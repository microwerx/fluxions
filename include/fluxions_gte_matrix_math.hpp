#ifndef FLUXIONS_GTE_MATRIX_MATH_HPP
#define FLUXIONS_GTE_MATRIX_MATH_HPP

// This header defines math between matrices and vector types
// * matrix * vector
// * matrix * color

#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_matrix2.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
	template <typename T>
	constexpr TVector2<T> operator*(const TMatrix2<T>& lhs, const TVector2<T>& rhs) noexcept {
		return TVector2<T>(
			lhs.m11 * rhs.x + lhs.m12 * rhs.y,
			lhs.m21 * rhs.x + lhs.m22 * rhs.y);
	}

	template <typename T>
	constexpr TVector3<T> operator*(const TMatrix3<T>& lhs, const TVector3<T>& rhs) noexcept {
		return TVector3<T>(
			lhs.m11 * rhs.x + lhs.m12 * rhs.y + lhs.m13 * rhs.z,
			lhs.m21 * rhs.x + lhs.m22 * rhs.y + lhs.m23 * rhs.z,
			lhs.m31 * rhs.x + lhs.m32 * rhs.y + lhs.m33 * rhs.z);
	}

	template <typename T>
	constexpr TColor3<T> operator*(const TMatrix3<T>& lhs, const TColor3<T>& rhs) noexcept {
		return TColor3<T>(
			lhs.m11 * rhs.r + lhs.m12 * rhs.g + lhs.m13 * rhs.b,
			lhs.m21 * rhs.r + lhs.m22 * rhs.g + lhs.m23 * rhs.b,
			lhs.m31 * rhs.r + lhs.m32 * rhs.g + lhs.m33 * rhs.b);
	}

	template <typename T>
	constexpr TVector4<T> operator*(const TMatrix4<T>& lhs, const TVector4<T>& rhs) noexcept {
		return TVector4<T>(
			lhs.m11 * rhs.x + lhs.m12 * rhs.y + lhs.m13 * rhs.z + lhs.m14 * rhs.w,
			lhs.m21 * rhs.x + lhs.m22 * rhs.y + lhs.m23 * rhs.z + lhs.m24 * rhs.w,
			lhs.m31 * rhs.x + lhs.m32 * rhs.y + lhs.m33 * rhs.z + lhs.m34 * rhs.w,
			lhs.m41 * rhs.x + lhs.m42 * rhs.y + lhs.m34 * rhs.z + lhs.m44 * rhs.w);
	}

	template <typename T>
	constexpr TColor4<T> operator*(const TMatrix4<T>& lhs, const TColor4<T>& rhs) noexcept {
		return TColor4<T>(
			lhs.m11 * rhs.r + lhs.m12 * rhs.g + lhs.m13 * rhs.b + lhs.m14 * rhs.a,
			lhs.m21 * rhs.r + lhs.m22 * rhs.g + lhs.m23 * rhs.b + lhs.m24 * rhs.a,
			lhs.m31 * rhs.r + lhs.m32 * rhs.g + lhs.m33 * rhs.b + lhs.m34 * rhs.a,
			lhs.m41 * rhs.r + lhs.m42 * rhs.g + lhs.m43 * rhs.b + lhs.m44 * rhs.a);
	}
}

#endif
