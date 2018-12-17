// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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

#ifndef FLUXIONS_VECTOR2_HPP
#define FLUXIONS_VECTOR2_HPP

//#include <type_traits>
#include <fluxions_gte_math.hpp>

namespace Fluxions
{
template <typename T>
class TVector2
{
  public:
    T x, y;

    constexpr T *ptr() noexcept { return &x; }
    constexpr const T *const_ptr() const noexcept { return &x; }

    using type = T;

    constexpr TVector2() noexcept
    {
        x = 0;
        y = 0;
    }

    constexpr TVector2(T _x, T _y) noexcept
    {
        x = _x;
        y = _y;
    }

    constexpr TVector2(const TVector2<T> &V) noexcept
    {
        x = V.x;
        y = V.y;
    }

    constexpr TVector2(TVector2<T> &&V) noexcept
    {
        x = std::move(V.x);
        y = std::move(V.y);
    }

    constexpr TVector2(const T V[2]) noexcept
    {
        x = V[0];
        y = V[1];
    }

    constexpr TVector2<T> &operator=(const TVector2<T> &V) noexcept
    {
        x = V.x;
        y = V.y;
        return *this;
    }

    template <typename U>
    operator TVector2<U>() const noexcept
    {
        return TVector2<U>(static_cast<U>(x), static_cast<U>(y));
    }

    constexpr TVector2<T> &reset(T _x = 0, T _y = 0) noexcept
    {
        x = _x;
        y = _y;
        return *this;
    }

    // template <typename U>
    constexpr TVector2<T> &operator+=(const T c) noexcept
    {
        x += c;
        y += c;
        return *this;
    }

    // template <typename U>
    constexpr TVector2<T> &operator-=(const T c) noexcept
    {
        x -= c;
        y -= c;
        return *this;
    }

    // template <typename U>
    constexpr TVector2<T> &operator*=(const T c) noexcept
    {
        x *= c;
        y *= c;
        return *this;
    }

    // template <typename U>
    constexpr TVector2<T> &operator/=(const T c) noexcept
    {
        x /= c;
        y /= c;
        return *this;
    }

    // template <typename U>
    constexpr TVector2<T> &operator+=(const TVector2<T> &V) noexcept
    {
        x += V.x;
        y += V.y;
        return *this;
    }

    // template <typename U>
    const TVector2<T> &operator-=(const TVector2<T> &V) noexcept
    {
        x -= V.x;
        y -= V.y;
        return *this;
    }

    //template <typename U> TVector2<T> operator * (const U val) const;
    //template <typename U> TVector2<T> operator / (const U val) const;
    //template <typename U> TVector2<T> operator + (const TVector2<U> & V) const;
    //template <typename U> TVector2<T> operator - (const TVector2<U> & V) const;

    //template <typename U> const T operator * (const TVector2<U> & V) const;

    constexpr T length() const noexcept
    {
        return static_cast<T>(sqrt(x * x + y * y));
    }

    constexpr T lengthSquared() const noexcept
    {
        return x * x + y * y;
    }

    constexpr TVector2<T> norm() const noexcept
    {
        T invLen = length() != 0 ? (T)(1.0 / length()) : 0;
        return TVector2<T>(x * invLen, y * invLen);
    }

    constexpr TVector2<T> &normalize() noexcept
    {
        return *this = norm();
    }
};

////////////////////////////////////////////////////////////////////
//// I M P L E M E N T A T I O N ///////////////////////////////////
////////////////////////////////////////////////////////////////////

//template<typename T>
//TVector2<T>::TVector2()
//	: X(0), y(0)
//{
//}

//template <typename T>
//TVector2<T>::TVector2(T _x, T _y)
//	: X(_x), y(_y)
//{
//}

//template <typename T>
//TVector2<T>::TVector2(const T V[2])
//	: X(V[0]), y(V[1])
//{
//}

//template <typename T>
//TVector2<T>::TVector2(const TVector2<T> & V)
//	: X(V.X), y(V.y)
//{
//}

//template <typename T>
//template <typename U>
//const TVector2<T> & TVector2<T>::operator = (const TVector2<U> & V)
//{
//	X = V.X;
//	y = V.y;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector2<T> & TVector2<T>::operator *= (const U val)
//{
//	X *= val;
//	y *= val;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector2<T> & TVector2<T>::operator /= (const U val)
//{
//	X /= val;
//	y /= val;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector2<T> & TVector2<T>::operator += (const TVector2<U> & V)
//{
//	X += V.X;
//	y += V.y;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TVector2<T> & TVector2<T>::operator -= (const TVector2<U> & V)
//{
//	X -= V.X;
//	y -= V.y;
//	return *this;
//}

//template <typename T>
//template <typename U>
//TVector2<T> TVector2<T>::operator * (const U val) const
//{
//	return TVector2<T>(X * val, y * val);
//}

//template <typename T>
//template <typename U>
//TVector2<T> TVector2<T>::operator / (const U val) const
//{
//	return TVector2<T>(X / val, y / val);
//}

//template <typename T>
//template <typename U>
//TVector2<T> TVector2<T>::operator + (const TVector2<U> & V) const
//{
//	return TVector2<T>(X + V.X, y + V.y);
//}

//template <typename T>
//template <typename U>
//TVector2<T> TVector2<T>::operator - (const TVector2<U> & V) const
//{
//	return TVector2<T>(X - V.X, y - V.y);
//}

//template <typename T>
//template <typename U>
//const T TVector2<T>::operator * (const TVector2<U> & V) const
//{
//	return X * V.X + y * V.y;
//}

//template <typename T>
//const T TVector2<T>::Length() const
//{
//	return (T)sqrt(X * X + y * y);
//}

//template <typename T>
//const T TVector2<T>::LengthSquared() const
//{
//	return X * X + y * y;
//}

using Vector2f = TVector2<float>;
using Vector2d = TVector2<double>;
using Vector2b = TVector2<char>;
using Vector2ub = TVector2<unsigned char>;
using Vector2s = TVector2<short>;
using Vector2us = TVector2<unsigned short>;
using Vector2i = TVector2<int>;
using Vector2ui = TVector2<unsigned int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TVector2<float>;
extern template class TVector2<double>;
extern template class TVector2<char>;
extern template class TVector2<unsigned char>;
extern template class TVector2<short>;
extern template class TVector2<unsigned short>;
extern template class TVector2<int>;
extern template class TVector2<unsigned int>;
#endif

template <typename T>
constexpr T DotProduct(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
constexpr T dot(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return v1.x * v2.x + v1.y * v2.y;
}

// N O N - C L A S S   O P E R A T I O N S //////////////////////////

template <typename T>
constexpr TVector2<T> operator*(const T a, const TVector2<T> V)
{
    return TVector2<T>(a * V.x, a * V.y);
}

template <typename T>
constexpr TVector2<T> operator*(const TVector2<T> V, const T a)
{
    return TVector2<T>(a * V.x, a * V.y);
}

template <typename T>
constexpr TVector2<T> operator/(const TVector2<T> V, const T a)
{
    return TVector2<T>(V.x / a, V.y / a);
}

template <typename T>
constexpr TVector2<T> operator/(const T a, const TVector2<T> V)
{
    return TVector2<T>(a / V.x, a / V.y);
}

template <typename T>
constexpr auto operator+(const T a, const TVector2<T> &V)
{
    return TVector2<T>(a + V.x, a + V.y);
}

template <typename T>
constexpr auto operator+(const TVector2<T> &V, const T a) noexcept
{
    return TVector2<T>(V.x + a, V.y + a);
}

template <typename T>
constexpr auto operator-(const T a, const TVector2<T> &V)
{
    return TVector2<T>(a - V.x, a - V.y);
}

template <typename T>
constexpr auto operator-(const TVector2<T> &V, const T a) noexcept
{
    return TVector2<T>(V.x - a, V.y - a);
}

template <typename T>
constexpr auto operator*(const T a, const TVector2<T> &V)
{
    return TVector2<T>(a * V.x, a * V.y);
}

template <typename T>
constexpr auto operator*(const TVector2<T> &V, const T a) noexcept
{
    return TVector2<T>(V.x * a, V.y * a);
}

template <typename T>
constexpr auto operator/(const T a, const TVector2<T> &V)
{
    return TVector2<T>(a / V.x, a / V.y);
}

template <typename T>
constexpr auto operator/(const TVector2<T> &V, const T a) noexcept
{
    return TVector2<T>(V.x / a, V.y / a);
}

template <typename T>
constexpr auto operator+(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return TVector2<T>(
        v1.x + v2.x,
        v1.y + v2.y);
}

template <typename T>
constexpr auto operator-(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return TVector2<T>(
        v1.x - v2.x,
        v1.y - v2.y);
}

template <typename T>
constexpr bool operator==(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return (v1.x == v2.x && v1.y == v2.y);
}

template <typename T>
constexpr bool operator!=(const TVector2<T> &v1, const TVector2<T> &v2) noexcept
{
    return (v1.x != v2.x || v1.y != v2.y);
}
} // namespace Fluxions

#endif
