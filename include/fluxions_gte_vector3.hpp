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

#ifndef FLUXIONS_VECTOR3_HPP
#define FLUXIONS_VECTOR3_HPP

#include <fluxions_gte_common_vector.hpp>
#include <fluxions_gte_scalar_math.hpp>

namespace Fluxions
{
	template <typename T>
	class TVector3 : public TCommonContainer<T>
	{
	public:
		T x, y, z;

		using type = T;
		using type_ptr = T *;
		using const_type_ptr = const T*;
		using type_ref = T &;
		using const_type_ref = const T &;

		using vector_type = TVector3<T>;
		using const_vector_type = const TVector3<T>;

		constexpr type_ptr ptr() noexcept { return &x; }
		constexpr const_type_ptr const_ptr() const noexcept { return &x; }
		constexpr TVector3<T>& ref() noexcept { return *this; }
		constexpr const TVector3<T>& const_ref() const noexcept { return *this; }

		constexpr unsigned size() const noexcept { return 3; }

		using iterator = TCommonIterator<T>;
		using const_iterator = TCommonIterator<const T>;
		iterator begin() noexcept { return iterator(ptr()); }
		iterator end() noexcept { return iterator(ptr() + 3); }
		const_iterator begin() const noexcept { return const_iterator(const_ptr()); }
		const_iterator end() const noexcept { return const_iterator(const_ptr() + size()); }

		const T& operator[](unsigned i) const noexcept { return const_ptr()[i]; }
		T& operator[](unsigned i) noexcept { return ptr()[i]; }

		constexpr TVector3() noexcept {
			x = 0;
			y = 0;
			z = 0;
		}

		constexpr TVector3(const T _x, const T _y, const T _z) noexcept {
			x = _x;
			y = _y;
			z = _z;
		}

		constexpr TVector3(const T value) noexcept {
			x = value;
			y = value;
			z = value;
		}

		constexpr TVector3(const TVector3<T>& V) noexcept {
			x = V.x;
			y = V.y;
			z = V.z;
		}

		constexpr TVector3(TVector3<T>&& V) noexcept {
			x = std::move(V.x);
			y = std::move(V.y);
			z = std::move(V.z);
		}

		constexpr TVector3(const T V[3]) noexcept {
			x = V[0];
			y = V[1];
			z = V[2];
		}

		constexpr TVector3<T>& operator=(const TVector3<T>& V) noexcept {
			x = V.x;
			y = V.y;
			z = V.z;
			return *this;
		}

		constexpr TVector3<T>& operator=(const TVector3<T>&& V) noexcept {
			x = std::move(V.x);
			y = std::move(V.y);
			z = std::move(V.z);
			return *this;
		}

		template <typename U>
		operator TVector3<U>() const {
			return TVector3<U>(
				static_cast<U>(x),
				static_cast<U>(y),
				static_cast<U>(z));
		}

		constexpr TVector3<T> operator-() noexcept {
			if (TVector3<T>::is_signed)
				return TVector3<T>(-x, -y, -z);
			fprintf(stderr, "%s(): ATTEMPTING UNARY NEGATIVE ON SIGNED TYPE", __FUNCTION__);
			return TVector3<T>(x, y, z);
		}

		template <typename U>
		constexpr TVector3<T>& operator+=(const U val) noexcept {
			x += static_cast<T>(val);
			y += static_cast<T>(val);
			z += static_cast<T>(val);
			return *this;
		}

		template <typename U>
		constexpr TVector3<T>& operator-=(const U val) noexcept {
			x -= static_cast<T>(val);
			y -= static_cast<T>(val);
			z -= static_cast<T>(val);
			return *this;
		}

		template <typename U>
		constexpr TVector3<T>& operator*=(const U val) noexcept {
			x *= static_cast<T>(val);
			y *= static_cast<T>(val);
			z *= static_cast<T>(val);
			return *this;
		}

		template <typename U>
		constexpr TVector3<T>& operator/=(const U val) noexcept {
			x /= static_cast<T>(val);
			y /= static_cast<T>(val);
			z /= static_cast<T>(val);
			return *this;
		}

		template <typename U>
		constexpr TVector3<T>& operator+=(const TVector3<U>& V) noexcept {
			x += V.x;
			y += V.y;
			z += V.z;
			return *this;
		}

		template <typename U>
		constexpr TVector3<T>& operator-=(const TVector3<U>& V) noexcept {
			x -= V.x;
			y -= V.y;
			z -= V.z;
			return *this;
		}

		//template <typename U> TVector3<T> operator * (const U val) const;
		//template <typename U> TVector3<T> operator / (const U val) const;
		//template <typename U> TVector3<T> operator + (const TVector3<U> & V) const;
		//template <typename U> TVector3<T> operator - (const TVector3<U> & V) const;

		//constexpr TVector3<T> multiply(const TVector3<T> &V) const
		//{
		//	return TVector3<T>(X*V.X, y*V.y, z*V.z);
		//}

		//constexpr TVector3<T> divide(const TVector3<T> &V) const
		//{
		//	return TVector3<T>(X / V.X, y / V.y, z / V.z);
		//}

		//constexpr TVector3<T> sub(const int val) const
		//{
		//	return TVector3<T>((T)(X - val), (T)(y - val), (T)(z - val));
		//}

		//constexpr TVector3<T> sub(const float val) const
		//{
		//	return TVector3<T>((T)(X - val), (T)(y - val), (T)(z - val));
		//}

		//constexpr TVector3<T> sub(const double val) const
		//{
		//	return TVector3<T>((T)(X - val), (T)(y - val), (T)(z - val));
		//}

		//constexpr TVector3<T> add(const int val) const
		//{
		//	return TVector3<T>((T)(X + val), (T)(y + val), (T)(z + val));
		//}

		//constexpr TVector3<T> add(const float val) const
		//{
		//	return TVector3<T>((T)(X + val), (T)(y + val), (T)(z + val));
		//}

		//constexpr TVector3<T> add(const double val) const
		//{
		//	return TVector3<T>((T)(X + val), (T)(y + val), (T)(z + val));
		//}

		constexpr TVector3<T>& reset(T _x = 0, T _y = 0, T _z = 0) {
			x = _x;
			y = _y;
			z = _z;
			return *this;
		}

		// template <typename U> const T operator * (const TVector3<U> & V) const;

		//constexpr auto Length() const noexcept { return }
		//const T LengthSquared() const;
		//TVector3<T> & Normalize();

		constexpr T length() const noexcept {
			return static_cast<T>(sqrt(x * x + y * y + z * z));
		}

		constexpr T lengthSquared() const noexcept {
			return x * x + y * y + z * z;
		}

		constexpr vector_type norm() const noexcept {
			T invLen = (length() != 0) ? (T)(1.0 / length()) : 0;
			return TVector3<T>(x * invLen, y * invLen, z * invLen);
		}

		constexpr vector_type unit_vector() const noexcept {
			T invLen = length() != 0 ? (T)(1.0 / length()) : 0;
			return TVector3<T>(x * invLen, y * invLen, z * invLen);
		}

		constexpr vector_type& normalize() noexcept {
			return *this = norm();
		}

		constexpr type dot(const TVector3<T> v) const noexcept {
			return x * v.x + y * v.y + z * v.z;
		}

		constexpr vector_type cross(const TVector3<T> v) const noexcept {
			return TVector3<T>(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
		}

		// X is positive towards the viewer's right
		// y is positive up
		// z is positive behind the viewer
		//
		// Physics/ISO:
		// theta = arccos y/r
		//   phi = arctan X/z
		//
		//     X = sin theta sin phi
		//     y = cos theta
		//     z = sin theta cos phi
		//
		// Math:
		// theta = arctan X/z
		//   phi = arccos y/r
		//
		//     X = sin phi sin theta
		//     y = cos phi
		//     z = sin phi cos theta
		//
		//   +y
		//    |
		//    |
		//    +---- +X
		//   /
		//  /
		// +z
		//
		// Mathematics notation (phi is angle away from z axis, theta is angle away from X axis on XY plane)
		// inclination is phi
		// azimuth is theta
		//
		// theta = arctan y/X
		//   phi = arccos z/r
		//
		//     X = sin phi cos theta
		//     y = sin phi sin theta
		//     z = cos phi
		//
		//   +z
		//    |
		//    |
		//    +---- +y
		//   /
		//  /
		// +X
		//
		// Physics/ISO notation (phi is angle away from X axis on XY plane, theta is angle away from z axis)
		// inclination is theta
		// azimuth is phi
		//
		// theta = arccos z/r
		//   phi = arctan y/X
		//
		//     X = sin theta cos phi
		//     y = sin theta sin phi
		//     z = cos theta
		//
		//   +z
		//    |
		//    |
		//    +---- +y
		//   /
		//  /
		// +X
		//

		constexpr T to_straight_theta() const noexcept { return (T)acos((double)z / (double)length()); }
		constexpr T to_straight_phi() const noexcept { return (T)atan2((double)y, (double)x); }

		constexpr T to_math_theta() const noexcept { return (T)atan2((double)x, (double)z); }
		constexpr T to_math_phi() const noexcept { return (T)acos((double)y / (double)length()); }

		constexpr T to_physics_theta() const noexcept { return (T)acos((double)y / (double)length()); }
		constexpr T to_physics_phi() const noexcept { return (T)atan2((double)x, (double)z); }

		constexpr T theta() const noexcept { return to_physics_theta(); }
		constexpr T phi() const noexcept { return to_physics_phi(); }

		constexpr vector_type& from_theta_phi(T theta, T phi) noexcept { return from_physics_theta_phi(theta, phi); }

		constexpr vector_type& from_straight_theta_phi(T theta, T phi) {
			x = (T)(sin(theta) * cos(phi));
			y = (T)(sin(theta) * sin(phi));
			z = (T)(cos(theta));
			return *this;
		}

		constexpr vector_type& from_math_theta_phi(T theta, T phi) {
			x = (T)(sin(phi) * sin(theta));
			y = (T)(cos(phi));
			z = (T)(sin(phi) * cos(theta));
			return *this;
		}

		constexpr vector_type& from_physics_theta_phi(T theta, T phi) {
			x = (T)(sin(theta) * sin(phi));
			y = (T)(cos(theta));
			z = (T)(sin(theta) * cos(phi));
			return *this;
		}
	};

	//////////////////////////////////////////////////////////////////
	// I M P L E M E N T A T I O N ///////////////////////////////////
	//////////////////////////////////////////////////////////////////

	// unsigned specialization
	template <>
	constexpr TVector3<uint8_t> TVector3<uint8_t>::operator-() noexcept {
		return TVector3<uint8_t>(x, y, z);
	}
	template <>
	constexpr TVector3<uint16_t> TVector3<uint16_t>::operator-() noexcept {
		return TVector3<uint16_t>(x, y, z);
	}
	template <>
	constexpr TVector3<uint32_t> TVector3<uint32_t>::operator-() noexcept {
		return TVector3<uint32_t>(x, y, z);
	}
	template <>
	constexpr TVector3<uint64_t> TVector3<uint64_t>::operator-() noexcept {
		return TVector3<uint64_t>(x, y, z);
	}

	//template<typename T>
	//TVector3<T>::TVector3()
	//{
	//	X = 0;
	//	y = 0;
	//	z = 0;
	//}

	//template <typename T>
	//TVector3<T>::TVector3(T _x, T _y, T _z)
	//	: X(_x), y(_y), z(_z)
	//{
	//}

	//template <typename T>
	//TVector3<T>::TVector3(const T V[3])
	//	: X(V[0]), y(V[1]), z(V[2])
	//{
	//}

	//template <typename T>
	//TVector3<T>::TVector3(const TVector3<T> & V)
	//	: X(V.X), y(V.y), z(V.z)
	//{
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> & TVector3<T>::operator = (const TVector3<U> & V)
	//{
	//	X = T(V.X);
	//	y = T(V.y);
	//	z = T(V.z);
	//	return *this;
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> & TVector3<T>::operator *= (const U val)
	//{
	//	X = (T)(X * val);
	//	y = (T)(y * val);
	//	z = (T)(z * val);
	//	return *this;
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> & TVector3<T>::operator /= (const U val)
	//{
	//	X /= val;
	//	y /= val;
	//	z /= val;
	//	return *this;
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> & TVector3<T>::operator += (const TVector3<U> & V)
	//{
	//	X += V.X;
	//	y += V.y;
	//	z += V.z;
	//	return *this;
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> & TVector3<T>::operator -= (const TVector3<U> & V)
	//{
	//	X -= V.X;
	//	y -= V.y;
	//	z -= V.z;
	//	return *this;
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> TVector3<T>::operator * (const U val) const
	//{
	//	return TVector3<T>(X * val, y * val, z * val);
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> TVector3<T>::operator / (const U val) const
	//{
	//	return TVector3<T>(X / val, y / val, z / val);
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> TVector3<T>::operator + (const TVector3<U> & V) const
	//{
	//	return TVector3<T>(X + V.X, y + V.y, z + V.z);
	//}

	//template <typename T>
	//template <typename U>
	//TVector3<T> TVector3<T>::operator - (const TVector3<U> & V) const
	//{
	//	return TVector3<T>(X - V.X, y - V.y, z - V.z);
	//}

	////template <typename T>
	////template <typename U>
	////const T TVector3<T>::operator * (const TVector3<U> & V) const
	////{
	////	return X * V.X + y * V.y + z * V.z;
	////}

	//template <typename T>
	//const T TVector3<T>::Length() const
	//{
	//	return (T)sqrt(X * X + y * y + z * z);
	//}

	//template <typename T>
	//const T TVector3<T>::LengthSquared() const
	//{
	//	return X * X + y * y + z * z;
	//}

	//template <typename T>
	//TVector3<T> & TVector3<T>::Normalize()
	//{
	//	return *this /= Length();
	//}

	template <typename T, typename U>
	constexpr auto DotProduct(const TVector3<T>& v1, const TVector3<U>& v2) noexcept {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <typename T, typename U>
	constexpr auto dot(const TVector3<T>& v1, const TVector3<U>& v2) noexcept {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <typename T, typename U>
	constexpr auto CrossProduct(const TVector3<T>& v1, const TVector3<U>& v2) {
		return TVector3<std::common_type_t<T, U>>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	template <typename T, typename U>
	constexpr auto cross(const TVector3<T>& v1, const TVector3<U>& v2) {
		return TVector3<std::common_type_t<T, U>>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	using Vector3f = TVector3<float>;
	using Vector3d = TVector3<double>;
	using Vector3b = TVector3<char>;
	using Vector3ub = TVector3<unsigned char>;
	using Vector3s = TVector3<short>;
	using Vector3us = TVector3<unsigned short>;
	using Vector3i = TVector3<int>;
	using Vector3ui = TVector3<unsigned int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TVector3<float>;
	extern template class TVector3<double>;
	extern template class TVector3<char>;
	extern template class TVector3<unsigned char>;
	extern template class TVector3<short>;
	extern template class TVector3<unsigned short>;
	extern template class TVector3<int>;
	extern template class TVector3<unsigned int>;
#endif

	template <typename T, typename U>
	constexpr auto operator+(const U a, const TVector3<T>& V) {
		return TVector3<std::common_type_t<U, T>>(a + V.x, a + V.y, a + V.z);
	}

	template <typename T, typename U>
	constexpr auto operator+(const TVector3<T>& V, const U a) noexcept {
		return TVector3<std::common_type_t<U, T>>(V.x + a, V.y + a, V.z + a);
	}

	template <typename T, typename U>
	constexpr auto operator-(const U a, const TVector3<T>& V) {
		return TVector3<std::common_type_t<U, T>>(a - V.x, a - V.y, a - V.z);
	}

	template <typename T, typename U>
	constexpr auto operator-(const TVector3<T>& V, const U a) noexcept {
		return TVector3<std::common_type_t<U, T>>(V.x - a, V.y - a, V.z - a);
	}

	template <typename T, typename U>
	constexpr TVector3<T> operator*(const U a, const TVector3<T>& V) {
		return TVector3<T>(T(a) * V.x, T(a) * V.y, T(a) * V.z);
	}

	template <typename T, typename U>
	constexpr auto operator*(const TVector3<T>& V, const U a) noexcept {
		return TVector3<std::common_type_t<U, T>>(V.x * a, V.y * a, V.z * a);
	}

	template <typename T, typename U>
	constexpr auto operator/(const U a, const TVector3<T>& V) {
		return TVector3<std::common_type_t<U, T>>(a / V.x, a / V.y, a / V.z);
	}

	template <typename T, typename U>
	constexpr auto operator/(const TVector3<T>& V, const U a) noexcept {
		return TVector3<std::common_type_t<U, T>>(V.x / a, V.y / a, V.z / a);
	}

	template <typename T, typename U>
	constexpr auto operator+(const TVector3<T>& v1, const TVector3<U>& v2) noexcept {
		return TVector3<std::common_type_t<T, U>>(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z);
	}

	template <typename T, typename U>
	constexpr auto operator-(const TVector3<T>& v1, const TVector3<U>& v2) noexcept {
		return TVector3<std::common_type_t<T, U>>(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z);
	}

	//////////////////////////////////////////////////////////////////
	// COMPARISON OPERATORS //////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	template <typename T>
	constexpr bool operator==(const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
	}


	template <typename T>
	constexpr bool operator!=(const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return !(a == b);
	}

	template <typename T>
	constexpr TVector3<T> operator < (const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return TVector3<T>(a.x < b.x ? TVector3<T>::one : TVector3<T>::zero,
						   a.y < b.y ? TVector3<T>::one : TVector3<T>::zero,
						   a.z < b.z ? TVector3<T>::one : TVector3<T>::zero);
	}

	template <typename T>
	constexpr TVector3<T> operator <= (const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return TVector3<T>(a.x <= b.x ? TVector3<T>::one : TVector3<T>::zero,
						   a.y <= b.y ? TVector3<T>::one : TVector3<T>::zero,
						   a.z <= b.z ? TVector3<T>::one : TVector3<T>::zero);
	}

	template <typename T>
	constexpr TVector3<T> operator > (const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return TVector3<T>(a.x > b.x ? TVector3<T>::one : TVector3<T>::zero,
						   a.y > b.y ? TVector3<T>::one : TVector3<T>::zero,
						   a.z > b.z ? TVector3<T>::one : TVector3<T>::zero);
	}

	template <typename T>
	constexpr TVector3<T> operator >= (const TVector3<T>& a, const TVector3<T>& b) noexcept {
		return TVector3<T>(a.x >= b.x ? TVector3<T>::one : TVector3<T>::zero,
						   a.y >= b.y ? TVector3<T>::one : TVector3<T>::zero,
						   a.z >= b.z ? TVector3<T>::one : TVector3<T>::zero);
	}

} // namespace Fluxions

#endif
