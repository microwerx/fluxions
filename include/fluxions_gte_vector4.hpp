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

#ifndef FLUXIONS_VECTOR4_HPP
#define FLUXIONS_VECTOR4_HPP

#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>

namespace Fluxions
{
template <typename T>
struct tvec4
{
    T x, y, z, w;
};

using vec4 = tvec4<float>;
using dvec4 = tvec4<double>;
using ivec4 = tvec4<int>;
using ubvec4 = tvec4<unsigned char>;

template <typename T>
class TVector4
{
  public:
    T x, y, z, w;

    constexpr T *ptr() noexcept { return &x; }
    constexpr const T *const_ptr() const noexcept { return &x; }

    using type = T;

    constexpr TVector4() noexcept
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    constexpr TVector4(const T value) noexcept
    {
        x = value;
        y = value;
        z = value;
        w = value;
    }

    constexpr TVector4(T _x, T _y, T _z, T _w) noexcept
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    constexpr TVector4(const TVector4<T> &V) noexcept
    {
        x = V.x;
        y = V.y;
        z = V.z;
        w = V.w;
    }

    constexpr TVector4(const T V[4]) noexcept
    {
        x = V[0];
        y = V[1];
        z = V[2];
        w = V[3];
    }

    constexpr TVector4(const TVector2<T> &V, const T _z = 0, const T _w = 0) noexcept
    {
        x = V.x;
        y = V.y;
        z = _z;
        w = _w;
    }

    constexpr TVector4(const TVector3<T> &V, const T _w = 0) noexcept
    {
        x = V.x;
        y = V.y;
        z = V.z;
        w = _w;
    }

    template <typename U>
    constexpr auto operator=(const TVector4<U> &V) noexcept
    {
        x = V.x;
        y = V.y;
        z = V.z;
        w = V.w;
        return *this;
    }

    template <typename U>
    constexpr auto operator=(const TVector3<U> &V) noexcept
    {
        x = V.x;
        y = V.y;
        z = V.z;
        w = 0;
        return *this;
    }

    template <typename U>
    operator TVector4<U>() const
    {
        return TVector4<U>(
            static_cast<U>(x),
            static_cast<U>(y),
            static_cast<U>(z),
            static_cast<U>(w));
    }

    constexpr TVector4<T> operator-() noexcept
    {
        return TVector4<T>(-x, -y, -z, -w);
    }

    constexpr TVector4<T> &reset(T _x = 0, T _y = 0, T _z = 0, T _w = 1) noexcept
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
        return *this;
    }

    template <typename U>
    constexpr auto operator+=(const U val) noexcept
    {
        x += val;
        y += val;
        z += val;
        w += val;
        return *this;
    }

    template <typename U>
    constexpr auto operator-=(const U val) noexcept
    {
        x -= val;
        y -= val;
        z -= val;
        w -= val;
        return *this;
    }

    template <typename U>
    constexpr auto operator*=(const U val) noexcept
    {
        x *= val;
        y *= val;
        z *= val;
        w *= val;
        return *this;
    }

    template <typename U>
    constexpr auto operator/=(const U val) noexcept
    {
        x /= val;
        y /= val;
        z /= val;
        w /= val;
        return *this;
    }

    template <typename U>
    constexpr auto operator+=(const TVector2<U> &V) noexcept
    {
        x += V.x;
        y += V.y;
        return *this;
    }

    template <typename U>
    constexpr auto operator-=(const TVector2<U> &V) noexcept
    {
        x -= V.x;
        y -= V.y;
        return *this;
    }

    template <typename U>
    constexpr auto operator+=(const TVector3<U> &V) noexcept
    {
        x += V.x;
        y += V.y;
        z += V.z;
        return *this;
    }

    template <typename U>
    constexpr auto operator-=(const TVector3<U> &V) noexcept
    {
        x -= V.x;
        y -= V.y;
        z -= V.z;
        return *this;
    }

    template <typename U>
    constexpr auto operator+=(const TVector4<U> &V) noexcept
    {
        x += V.x;
        y += V.y;
        z += V.z;
        w += V.w;
        return *this;
    }

    template <typename U>
    constexpr auto operator-=(const TVector4<U> &V) noexcept
    {
        x -= V.x;
        y -= V.y;
        z -= V.z;
        w -= V.w;
        return *this;
    }

    //template <typename U> TVector4<T> operator * (const U val) const;
    //template <typename U> TVector4<T> operator / (const U val) const;
    //template <typename U> TVector4<T> operator + (const TVector4<U> & V) const;
    //template <typename U> TVector4<T> operator - (const TVector4<U> & V) const;

    //template <typename U> const T operator * (const TVector4<U> & V) const;

    constexpr auto xy() const noexcept { return TVector2<T>(x, y); }
    constexpr auto xyz() const noexcept { return TVector3<T>(x, y, z); }
    constexpr auto xyzw() const noexcept { return TVector4<T>(x, y, z, w); }
    //constexpr auto swizzle(IndexType v0, IndexType v1) const noexcept { return TVector2<T>(ptr[v0], ptr[v1]); }
    //constexpr auto swizzle(IndexType v0, IndexType v1, IndexType v2) const noexcept { return TVector3<T>(ptr[v0], ptr[v1], ptr[v2]); }
    //constexpr auto swizzle(IndexType v0, IndexType v1, IndexType v2, IndexType v3) const noexcept { return TVector4<T>(ptr[v0], ptr[v1], ptr[v2], ptr[v3]); }

    //const T Length() const;
    //const T LengthSquared() const;
    //void Normalize();

    constexpr T length() const noexcept
    {
        return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
    }

    constexpr T lengthSquared() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    constexpr auto norm() const noexcept
    {
        auto len = length();
        auto invLen = len != 0 ? static_cast<T>(1.0 / len) : 0;
        return TVector4<T>(
            T(x * invLen),
            T(y * invLen),
            T(z * invLen),
            T(w * invLen));
    }

    constexpr auto normalize() noexcept
    {
        return *this = norm();
    }

    constexpr auto unit_vector() const noexcept
    {
        return norm();
    }

    template <typename U>
    constexpr T dot(const TVector4<T> &v) const noexcept
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }
};

//////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N ///////////////////////////////////
//////////////////////////////////////////////////////////////////

// unsigned specialization

template <>
constexpr TVector4<uint8_t> TVector4<uint8_t>::operator-() noexcept
{
    return TVector4<uint8_t>(x, y, z, w);
}
template <>
constexpr TVector4<uint16_t> TVector4<uint16_t>::operator-() noexcept
{
    return TVector4<uint16_t>(x, y, z, w);
}
template <>
constexpr TVector4<uint32_t> TVector4<uint32_t>::operator-() noexcept
{
    return TVector4<uint32_t>(x, y, z, w);
}
template <>
constexpr TVector4<uint64_t> TVector4<uint64_t>::operator-() noexcept
{
    return TVector4<uint64_t>(x, y, z, w);
}

//template<typename T>
//TVector4<T>::TVector4()
//{
//	X = 0;
//	y = 0;
//	z = 0;
//	w = 1;
//}

//template <typename T>
//TVector4<T>::TVector4(T _x, T _y, T _z, T _w)
//	: X(_x), y(_y), z(_z), w(_w)
//{
//}

//template <typename T>
//TVector4<T>::TVector4(const TVector4<T> & V)
//	: X(V.X), y(V.y), z(V.z), w(V.w)
//{
//}

//template <typename T>
//TVector4<T>::TVector4(const T V[4])
//	: X(V[0]), y(V[1]), z(V[2]), w(V[3])
//{
//}

//template <typename T>
//TVector4<T>::TVector4(const TVector3<T> & V, const T &W)
//	: X(V.X), y(V.y), z(V.z), w(W)
//{
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator = (const TVector4<U> & V)
//{
//	X = V.X;
//	y = V.y;
//	z = V.z;
//	w = V.w;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator = (const TVector3<U> & V)
//{
//	X = V.X;
//	y = V.y;
//	z = V.z;
//	w = 1;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator *= (const U val)
//{
//	X *= val;
//	y *= val;
//	z *= val;
//	w *= val;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator /= (const U val)
//{
//	X /= val;
//	y /= val;
//	z /= val;
//	w /= val;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator += (const TVector4<U> & V)
//{
//	X += V.X;
//	y += V.y;
//	z += V.z;
//	w += V.w;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator -= (const TVector4<U> & V)
//{
//	X -= V.X;
//	y -= V.y;
//	z -= V.z;
//	w -= V.w;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator += (const TVector2<U> & V)
//{
//	X += V.X;
//	y += V.y;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator -= (const TVector2<U> & V)
//{
//	X -= V.X;
//	y -= V.y;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator += (const TVector3<U> & V)
//{
//	X += V.X;
//	y += V.y;
//	z += V.z;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector4<T> & TVector4<T>::operator -= (const TVector3<U> & V)
//{
//	X -= V.X;
//	y -= V.y;
//	z -= V.z;
//	return *this;
//}

//template <typename T>
//template <typename U>
//TVector4<T> TVector4<T>::operator * (const U val) const
//{
//	return TVector4<T>(X * val, y * val, z * val, w * val);
//}

//template <typename T>
//template <typename U>
//TVector4<T> TVector4<T>::operator / (const U val) const
//{
//	return TVector4<T>(X / val, y / val, z / val, w / val);
//}

//template <typename T>
//template <typename U>
//TVector4<T> TVector4<T>::operator + (const TVector4<U> & V) const
//{
//	return TVector4<T>(X + V.X, y + V.y, z + V.z, w + V.w);
//}

//template <typename T>
//template <typename U>
//TVector4<T> TVector4<T>::operator - (const TVector4<U> & V) const
//{
//	return TVector4<T>(X - V.X, y - V.y, z - V.z, w - V.w);
//}

//template <typename T>
//template <typename U>
//const T TVector4<T>::operator * (const TVector4<U> & V) const
//{
//	return X * V.X + y * V.y + z * V.z + w * V.w;
//}

//template <typename T>
//const T TVector4<T>::Length() const
//{
//	return (T)sqrt(X * X + y * y + z * z + w * w);
//}

//template <typename T>
//const T TVector4<T>::LengthSquared() const
//{
//	return X * X + y * y + z * z + w * w;
//}

//template <typename T>
//void TVector4<T>::Normalize()
//{
//	*this /= Length();
//}

template <typename T, typename U>
constexpr auto DotProduct(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <typename T, typename U>
constexpr auto dot(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <typename T, typename U>
constexpr bool operator!=(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    if (v1.x != v2.x)
        return true;
    if (v1.y != v2.y)
        return true;
    if (v1.z != v2.z)
        return true;
    if (v1.w != v2.w)
        return true;
    return false;
}

template <typename T, typename U>
constexpr bool operator==(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return !(v1 != v2);
}

using Vector4f = TVector4<float>;
using Vector4d = TVector4<double>;
using Vector4b = TVector4<char>;
using Vector4ub = TVector4<unsigned char>;
using Vector4s = TVector4<short>;
using Vector4us = TVector4<unsigned short>;
using Vector4i = TVector4<int>;
using Vector4ui = TVector4<unsigned int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TVector4<float>;
extern template class TVector4<double>;
extern template class TVector4<char>;
extern template class TVector4<unsigned char>;
extern template class TVector4<short>;
extern template class TVector4<unsigned short>;
extern template class TVector4<int>;
extern template class TVector4<unsigned int>;
#endif

template <typename T, typename U>
constexpr auto operator+(const U a, const TVector4<T> &V)
{
    return TVector4<std::common_type_t<U, T>>(a + V.x, a + V.y, a + V.z, a + V.w);
}

template <typename T, typename U>
constexpr auto operator+(const TVector4<T> &V, const U a) noexcept
{
    return TVector4<std::common_type_t<U, T>>(V.x + a, V.y + a, V.z + a, V.w + a);
}

template <typename T, typename U>
constexpr auto operator-(const U a, const TVector4<T> &V)
{
    return TVector4<std::common_type_t<U, T>>(a - V.x, a - V.y, a - V.z, a - V.w);
}

template <typename T, typename U>
constexpr auto operator-(const TVector4<T> &V, const U a) noexcept
{
    return TVector4<std::common_type_t<U, T>>(V.x - a, V.y - a, V.z - a, V.w - a);
}

template <typename T, typename U>
constexpr auto operator*(const U a, const TVector4<T> &V)
{
    return TVector4<std::common_type_t<U, T>>(a * V.x, a * V.y, a * V.z, a * V.w);
}

template <typename T, typename U>
constexpr auto operator*(const TVector4<T> &V, const U a) noexcept
{
    return TVector4<std::common_type_t<U, T>>(V.x * a, V.y * a, V.z * a, V.w * a);
}

template <typename T, typename U>
constexpr auto operator/(const U a, const TVector4<T> &V)
{
    return TVector4<std::common_type_t<U, T>>(a / V.x, a / V.y, a / V.z, a / V.w);
}

template <typename T, typename U>
constexpr auto operator/(const TVector4<T> &V, const U a) noexcept
{
    return TVector4<std::common_type_t<U, T>>(V.x / a, V.y / a, V.z / a, V.w / a);
}

template <typename T, typename U>
constexpr auto operator+(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return TVector4<std::common_type_t<T, U>>(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w);
}

template <typename T, typename U>
constexpr auto operator-(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return TVector4<std::common_type_t<T, U>>(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w);
}

template <typename T, typename U>
constexpr auto operator*(const TVector4<T> &v1, const TVector4<U> &v2) noexcept
{
    return TVector4<std::common_type_t<T, U>>(
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
        v1.w * v2.w);
}
} // namespace Fluxions

#endif
