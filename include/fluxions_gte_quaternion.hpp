// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#ifndef FLUXIONS_GTE_QUATERNION_HPP
#define FLUXIONS_GTE_QUATERNION_HPP

#include <fluxions_gte_common_vector.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
using QScalar = float;

// Quaterion operations:
// q = a + bi + cj + dk

template <typename T = QScalar>
class TQuaternion : public TCommonContainer<T>
{
public:
	T a, b, c, d;

	using value_type = T;
	using value_ref = T&;
	using value_ptr = T*;

	using const_value_type = const T;
	using const_value_ref = const T&;
	using const_value_ptr = const T*;

	inline value_ptr ptr() { return &a; }
	inline const_value_ptr const_ptr() const { return &a; }

	using type = T;

	constexpr TQuaternion() noexcept
		: a(0), b(0), c(0), d(0) {}

	constexpr TQuaternion(const T aVal, const T bVal, const T cVal, const T dVal) noexcept
		: a(aVal), b(bVal), c(cVal), d(dVal) {}

	constexpr TQuaternion(const TQuaternion& q) noexcept
		: a(q.a), b(q.b), c(q.c), d(q.d) {}

	const TQuaternion<T>& operator=(const TQuaternion<T>& q) noexcept {
		b = q.b;
		c = q.c;
		d = q.d;
		a = q.a;
		return *this;
	}

	static TQuaternion<T> makeFromAngles(double azimuthInDegrees, double pitchInDegrees, double rollInDegrees) noexcept;
	static TQuaternion<T> makeFromAngleAxis(double angleInDegrees, double x, double y, double z) noexcept;
	static TQuaternion<T> makeFromMatrix3(const TMatrix3<T>& M) noexcept;

	void toAngleAxis(double& angleInDegrees, double& x, double& y, double& z) const noexcept;
	TMatrix3<T> toMatrix3() const noexcept;
	TMatrix4<T> toMatrix4() const noexcept;

	TQuaternion<T> conjugate() const noexcept {
		return TQuaternion(-b, -c, -d, a);
	}

	T length() const noexcept {
		return sqrt(b * b + c * c + d * d + a * a);
	}

	TQuaternion<T> normalized() const noexcept {
		T invlength = T(1) / length();
		return TQuaternion<T>{a* invlength, b* invlength, c* invlength, d* invlength};
	}

	TQuaternion<T> operator-() const noexcept {
		return TQuaternion<T>{-a, -b, -c, -d};
	}

	TQuaternion<T> operator+(const TQuaternion<T>& q) const noexcept {
		return TQuaternion<T>(a + q.a, b + q.b, c + q.c, d + q.d);
	}

	TQuaternion<T> operator-(const TQuaternion<T>& q) const noexcept {
		return TQuaternion<T>(a - q.a, b - q.b, c - q.c, d - q.d);
	}

	// Hamiltonian Product
	TQuaternion<T> operator*(const TQuaternion<T>& q) const noexcept {
		return TQuaternion<T>(a * q.a - b * q.b - c * q.c - d * q.d,
							  a * q.b + b * q.a + c * q.d - d * q.c,
							  a * q.c - b * q.d + c * q.a + d * q.b,
							  a * q.d + b * q.c - c * q.b + d * q.a
							  );
	}

	TQuaternion<T> operator+(T alpha) const noexcept {
		return TQuaternion(a + alpha, b + alpha, c + alpha, d + alpha);
	}

	TQuaternion<T> operator-(T alpha) const noexcept {
		return TQuaternion(a - alpha, b - alpha, c - alpha, d - alpha);
	}

	TQuaternion<T> scale(T alpha) const noexcept {
		return TQuaternion(a * alpha, b * alpha, c * alpha, d * alpha);
	}

	TQuaternion<T> operator*(T alpha) const noexcept {
		return TQuaternion(a * alpha, b * alpha, c * alpha, d * alpha);
	}

	T dot(const TQuaternion& q) const noexcept {
		return (b * q.b + c * q.c + d * q.d);
	}

	TQuaternion<T> cross(const TQuaternion<T>& q) const noexcept {
		return TQuaternion(
			c * q.d - d * q.c,
			d * q.b - b * q.d,
			b * q.c - c * q.b,
			0.);
	}

	inline constexpr value_type m11() const noexcept { return 1 - 2 * c * c - 2 * d * d; }
	inline constexpr value_type m12() const noexcept { return 2 * b * c - 2 * a * d; }
	inline constexpr value_type m13() const noexcept { return 2 * b * d + 2 * a * c; }
	inline constexpr value_type m21() const noexcept { return 2 * b * c + 2 * a * d; }
	inline constexpr value_type m22() const noexcept { return 1 - 2 * b * b - 2 * d * d; }
	inline constexpr value_type m23() const noexcept { return 2 * c * d - 2 * a * b; }
	inline constexpr value_type m31() const noexcept { return 2 * b * d - 2 * a * c; }
	inline constexpr value_type m32() const noexcept { return 2 * c * d + 2 * a * b; }
	inline constexpr value_type m33() const noexcept { return 1 - 2 * b * b - 2 * c * c; }
};

template <typename T>
TQuaternion<T> TQuaternion<T>::makeFromAngles(double azimuthInDegrees, double pitchInDegrees, double rollInDegrees) noexcept {
	TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(pitchInDegrees, 1.0, 0.0, 0.0);
	TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(azimuthInDegrees, 0.0, 1.0, 0.0);
	TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(rollInDegrees, 0.0, 0.0, 1.0);

	return (qZ * qX * qY).normalized();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::makeFromAngleAxis(double angleInDegrees, double x, double y, double z) noexcept {
	double c = cos(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
	double s = sin(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
	return TQuaternion<T>((T)c, (T)(s * x), (T)(s * y), (T)(s * z)).normalized();
}

template <typename T>
TQuaternion<T> TQuaternion<T>::makeFromMatrix3(const TMatrix3<T>& M) noexcept {
	T trace = M.Trace();
	T r = (T)sqrt(1 + trace);
	T w = (T)(0.5 * r);
	T x = (T)copysign(0.5 * sqrt(1 + M.m11 - M.m22 - M.m33), M.m32 - M.m23);
	T y = (T)copysign(0.5 * sqrt(1 - M.m11 + M.m22 - M.m33), M.m13 - M.m31);
	T z = (T)copysign(0.5 * sqrt(1 - M.m11 - M.m22 + M.m33), M.m21 - M.m12);
	return TQuaternion<T>(w, x, y, z).normalized();
}

template <typename T>
TQuaternion<T> operator*(T a, TQuaternion<T>& q) {
	return TQuaternion<T>{a* q.a, a* q.b, a* q.c, a* q.d};
}

template <typename T>
void TQuaternion<T>::toAngleAxis(double& angleInDegrees, double& x, double& y, double& z) const noexcept {
	angleInDegrees = 2.0 * acos(a) * FX_RADIANS_TO_DEGREES;
	double denominator = sqrt(1 - a * a);
	if (denominator > 0.0) {
		x = b / denominator;
		y = c / denominator;
		z = d / denominator;
	}
	else {
		x = 1.0;
		y = 0.0;
		z = 0.0;
	}
}

template <typename T>
TMatrix3<T> TQuaternion<T>::toMatrix3() const noexcept {
	return TMatrix3<T>(m11(), m12(), m13(),
					   m21(), m22(), m23(),
					   m31(), m32(), m33());
}


template <typename T>
TMatrix4<T> TQuaternion<T>::toMatrix4() const noexcept {
	return TMatrix4<T>(m11(), m12(), m13(), 0,
					   m21(), m22(), m23(), 0,
					   m31(), m32(), m33(), 0,
					   0, 0, 0, 1);
}

template <typename T>
TQuaternion<T> slerp(T t, const TQuaternion<T>& a, const TQuaternion<T>& b) noexcept {
	// Code adapted from Wikipedia article / Ken Shoemake article
	TQuaternion<T> q1 = a.normalized();
	TQuaternion<T> q2 = b.normalized();
	T dot = a.dot(b);

	if (dot < 0) {
		q2 = -q2;
		dot = -dot;
	}

	const T threshold = T(0.9995);
	if (dot > threshold) {
		TQuaternion<T> q = q1 + (q2 - q1).scale(t);
		return q.normalized();
	}

	T theta_0 = acos(dot);
	T theta = theta_0 * t;
	T sin_theta = sin(theta);
	T sin_theta_0 = sin(theta_0);

	T s0 = cos(theta) - dot * sin_theta / sin_theta_0;
	T s1 = sin_theta / sin_theta_0;

	return ((s0 * q1) + (s1 * q2)).normalized();
}

using Quaternionf = TQuaternion<float>;
using Quaterniond = TQuaternion<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TQuaternion<float>;
extern template class TQuaternion<double>;
#endif

} // namespace Fluxions

#endif
