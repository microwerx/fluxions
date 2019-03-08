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
#ifndef FLUXIONS_MATRIX4_HPP
#define FLUXIONS_MATRIX4_HPP

#include <functional>
#include <fluxions_gte_math.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{

template <typename T>
class TMatrix4
{
  public:
    // normal access (column major)
    T m11, m21, m31, m41;
    T m12, m22, m32, m42;
    T m13, m23, m33, m43;
    T m14, m24, m34, m44;

    constexpr T *ptr() noexcept { return &m11; }
    constexpr const T *const_ptr() const noexcept { return &m11; }

    using type = T;

    static constexpr size_t size() { return 16; }
    static constexpr size_t numrows() { return 4; }
    static constexpr size_t numcols() { return 4; }

    constexpr TMatrix4() noexcept
    {
        m11 = 1;
        m12 = 0;
        m13 = 0;
        m14 = 0;
        m21 = 0;
        m22 = 1;
        m23 = 0;
        m24 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
    }

    constexpr TMatrix4(const TMatrix4<T> &M) noexcept
    {
        m11 = M.m11;
        m12 = M.m12;
        m13 = M.m13;
        m14 = M.m14;
        m21 = M.m21;
        m22 = M.m22;
        m23 = M.m23;
        m24 = M.m24;
        m31 = M.m31;
        m32 = M.m32;
        m33 = M.m33;
        m34 = M.m34;
        m41 = M.m41;
        m42 = M.m42;
        m43 = M.m43;
        m44 = M.m44;
    }

    constexpr TMatrix4(TMatrix4<T> &&M) noexcept
    {
        m11 = std::move(M.m11);
        m12 = std::move(M.m12);
        m13 = std::move(M.m13);
        m14 = std::move(M.m14);
        m21 = std::move(M.m21);
        m22 = std::move(M.m22);
        m23 = std::move(M.m23);
        m24 = std::move(M.m24);
        m31 = std::move(M.m31);
        m32 = std::move(M.m32);
        m33 = std::move(M.m33);
        m34 = std::move(M.m34);
        m41 = std::move(M.m41);
        m42 = std::move(M.m42);
        m43 = std::move(M.m43);
        m44 = std::move(M.m44);
    }

    constexpr TMatrix4(const T value) noexcept
    {
        m11 = value;
        m12 = value;
        m13 = value;
        m14 = value;
        m21 = value;
        m22 = value;
        m23 = value;
        m24 = value;
        m31 = value;
        m32 = value;
        m33 = value;
        m34 = value;
        m41 = value;
        m42 = value;
        m43 = value;
        m44 = value;
    }

    constexpr TMatrix4(
        const T a11, const T a12, const T a13, const T a14,
        const T a21, const T a22, const T a23, const T a24,
        const T a31, const T a32, const T a33, const T a34,
        const T a41, const T a42, const T a43, const T a44) noexcept
    {
        m11 = a11;
        m12 = a12;
        m13 = a13;
        m14 = a14;
        m21 = a21;
        m22 = a22;
        m23 = a23;
        m24 = a24;
        m31 = a31;
        m32 = a32;
        m33 = a33;
        m34 = a34;
        m41 = a41;
        m42 = a42;
        m43 = a43;
        m44 = a44;
    }

    constexpr TMatrix4(const T M[numrows()][numcols()]) noexcept
    {
        const T *_v = M[0];
        T *v = &m11;
        for (size_t i = 0; i < size(); i++)
        {
            v[i] = _v[i];
        }
    }

    constexpr TMatrix4(const T M[size()]) noexcept
    {
        const T *_v = M;
        T *v = &m11;
        for (size_t i = 0; i < size(); i++)
        {
            v[i] = _v[i];
        }
    }

    constexpr TMatrix4<T> &operator=(const TMatrix4<T> &M) noexcept
    {
        m11 = M.m11;
        m12 = M.m12;
        m13 = M.m13;
        m14 = M.m14;
        m21 = M.m21;
        m22 = M.m22;
        m23 = M.m23;
        m24 = M.m24;
        m31 = M.m31;
        m32 = M.m32;
        m33 = M.m33;
        m34 = M.m34;
        m41 = M.m41;
        m42 = M.m42;
        m43 = M.m43;
        m44 = M.m44;
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator=(const TMatrix4<U> &M) noexcept
    {
        m11 = static_cast<T>(M.m11);
        m12 = static_cast<T>(M.m12);
        m13 = static_cast<T>(M.m13);
        m14 = static_cast<T>(M.m14);
        m21 = static_cast<T>(M.m21);
        m22 = static_cast<T>(M.m22);
        m23 = static_cast<T>(M.m23);
        m24 = static_cast<T>(M.m24);
        m31 = static_cast<T>(M.m31);
        m32 = static_cast<T>(M.m32);
        m33 = static_cast<T>(M.m33);
        m34 = static_cast<T>(M.m34);
        m41 = static_cast<T>(M.m41);
        m42 = static_cast<T>(M.m42);
        m43 = static_cast<T>(M.m43);
        m44 = static_cast<T>(M.m44);
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator=(const U value) noexcept
    {
        m11 = static_cast<T>(value);
        m12 = static_cast<T>(value);
        m13 = static_cast<T>(value);
        m14 = static_cast<T>(value);
        m21 = static_cast<T>(value);
        m22 = static_cast<T>(value);
        m23 = static_cast<T>(value);
        m24 = static_cast<T>(value);
        m31 = static_cast<T>(value);
        m32 = static_cast<T>(value);
        m33 = static_cast<T>(value);
        m34 = static_cast<T>(value);
        m41 = static_cast<T>(value);
        m42 = static_cast<T>(value);
        m43 = static_cast<T>(value);
        m44 = static_cast<T>(value);
        return *this;
    }

    template <typename U>
    constexpr operator TMatrix4<U>() const noexcept
    {
        return TMatrix4<U>(
            static_cast<U>(m11), static_cast<U>(m12), static_cast<U>(m13), static_cast<U>(m14),
            static_cast<U>(m21), static_cast<U>(m22), static_cast<U>(m23), static_cast<U>(m24),
            static_cast<U>(m31), static_cast<U>(m32), static_cast<U>(m33), static_cast<U>(m34),
            static_cast<U>(m41), static_cast<U>(m42), static_cast<U>(m43), static_cast<U>(m44));
    }

    constexpr TVector4<T> row1() const noexcept { return TVector4<T>(m11, m12, m13, m14); }
    constexpr TVector4<T> row2() const noexcept { return TVector4<T>(m21, m22, m23, m24); }
    constexpr TVector4<T> row3() const noexcept { return TVector4<T>(m31, m32, m33, m34); }
    constexpr TVector4<T> row4() const noexcept { return TVector4<T>(m41, m42, m43, m44); }
    constexpr TVector4<T> col1() const noexcept { return TVector4<T>(m11, m21, m31, m41); }
    constexpr TVector4<T> col2() const noexcept { return TVector4<T>(m12, m22, m32, m42); }
    constexpr TVector4<T> col3() const noexcept { return TVector4<T>(m13, m23, m33, m43); }
    constexpr TVector4<T> col4() const noexcept { return TVector4<T>(m14, m24, m34, m44); }

    // col(i) returns the ith column of the matrix (i = 1 is column 1)
    constexpr TVector4<T> col(int i) const noexcept
    {
        switch (i)
        {
        case 1:
            return col1();
        case 2:
            return col2();
        case 3:
            return col3();
        case 4:
            return col4();
        }
        return TVector4<T>();
    }

    // row(i) returns the ith row of the matrix (i = 1 is row 1)
    constexpr TVector4<T> row(int i) const noexcept
    {
        switch (i)
        {
        case 1:
            return row1();
        case 2:
            return row2();
        case 3:
            return row3();
        case 4:
            return row4();
        }
        return TVector4<T>();
    }

    template <typename U>
    static constexpr auto compAdd(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            m1.m11 + m2.m11, m1.m12 + m2.m12, m1.m13 + m2.m13, m1.m14 + m2.m14,
            m1.m21 + m2.m21, m1.m22 + m2.m22, m1.m23 + m2.m23, m1.m24 + m2.m24,
            m1.m31 + m2.m31, m1.m32 + m2.m32, m1.m33 + m2.m33, m1.m34 + m2.m34,
            m1.m41 + m2.m41, m1.m42 + m2.m42, m1.m43 + m2.m43, m1.m44 + m2.m44);
    }

    template <typename U>
    static constexpr auto compSub(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            m1.m11 - m2.m11, m1.m12 - m2.m12, m1.m13 - m2.m13, m1.m14 - m2.m14,
            m1.m21 - m2.m21, m1.m22 - m2.m22, m1.m23 - m2.m23, m1.m24 - m2.m24,
            m1.m31 - m2.m31, m1.m32 - m2.m32, m1.m33 - m2.m33, m1.m34 - m2.m34,
            m1.m41 - m2.m41, m1.m42 - m2.m42, m1.m43 - m2.m43, m1.m44 - m2.m44);
    }

    template <typename U>
    static constexpr auto compMult(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            m1.m11 * m2.m11, m1.m12 * m2.m12, m1.m13 * m2.m13, m1.m14 * m2.m14,
            m1.m21 * m2.m21, m1.m22 * m2.m22, m1.m23 * m2.m23, m1.m24 * m2.m24,
            m1.m31 * m2.m31, m1.m32 * m2.m32, m1.m33 * m2.m33, m1.m34 * m2.m34,
            m1.m41 * m2.m41, m1.m42 * m2.m42, m1.m43 * m2.m43, m1.m44 * m2.m44);
    }

    template <typename U>
    static constexpr auto compDiv(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            m1.m11 / m2.m11, m1.m12 / m2.m12, m1.m13 / m2.m13, m1.m14 / m2.m14,
            m1.m21 / m2.m21, m1.m22 / m2.m22, m1.m23 / m2.m23, m1.m24 / m2.m24,
            m1.m31 / m2.m31, m1.m32 / m2.m32, m1.m33 / m2.m33, m1.m34 / m2.m34,
            m1.m41 / m2.m41, m1.m42 / m2.m42, m1.m43 / m2.m43, m1.m44 / m2.m44);
    }

    template <typename U>
    static constexpr auto multiply(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            (m1.m14 * m2.m41 + m1.m13 * m2.m31 + m1.m12 * m2.m21 + m1.m11 * m2.m11),
            (m1.m14 * m2.m42 + m1.m13 * m2.m32 + m1.m12 * m2.m22 + m1.m11 * m2.m12),
            (m1.m14 * m2.m43 + m1.m13 * m2.m33 + m1.m12 * m2.m23 + m1.m11 * m2.m13),
            (m1.m14 * m2.m44 + m1.m13 * m2.m34 + m1.m12 * m2.m24 + m1.m11 * m2.m14),

            (m1.m24 * m2.m41 + m1.m23 * m2.m31 + m1.m22 * m2.m21 + m1.m21 * m2.m11),
            (m1.m24 * m2.m42 + m1.m23 * m2.m32 + m1.m22 * m2.m22 + m1.m21 * m2.m12),
            (m1.m24 * m2.m43 + m1.m23 * m2.m33 + m1.m22 * m2.m23 + m1.m21 * m2.m13),
            (m1.m24 * m2.m44 + m1.m23 * m2.m34 + m1.m22 * m2.m24 + m1.m21 * m2.m14),

            (m1.m34 * m2.m41 + m1.m33 * m2.m31 + m1.m32 * m2.m21 + m1.m31 * m2.m11),
            (m1.m34 * m2.m42 + m1.m33 * m2.m32 + m1.m32 * m2.m22 + m1.m31 * m2.m12),
            (m1.m34 * m2.m43 + m1.m33 * m2.m33 + m1.m32 * m2.m23 + m1.m31 * m2.m13),
            (m1.m34 * m2.m44 + m1.m33 * m2.m34 + m1.m32 * m2.m24 + m1.m31 * m2.m14),

            (m1.m44 * m2.m41 + m1.m43 * m2.m31 + m1.m42 * m2.m21 + m1.m41 * m2.m11),
            (m1.m44 * m2.m42 + m1.m43 * m2.m32 + m1.m42 * m2.m22 + m1.m41 * m2.m12),
            (m1.m44 * m2.m43 + m1.m43 * m2.m33 + m1.m42 * m2.m23 + m1.m41 * m2.m13),
            (m1.m44 * m2.m44 + m1.m43 * m2.m34 + m1.m42 * m2.m24 + m1.m41 * m2.m14));
    }

    constexpr auto LoadIdentity() noexcept
    {
        return *this = MakeIdentity();
    }

    constexpr auto operator-() noexcept
    {
        return TMatrix4<T>(
            -m11, -m12, -m13, -m14,
            -m21, -m22, -m23, -m24,
            -m31, -m32, -m33, -m34,
            -m41, -m42, -m43, -m44);
    }

    template <typename U>
    constexpr auto operator()(std::function<U(T x)> Func) noexcept
    {
        return TMatrix4<std::common_type_t<T, U>>(
            Func(m11), Func(m12), Func(m13), Func(m14),
            Func(m21), Func(m22), Func(m23), Func(m24),
            Func(m31), Func(m32), Func(m33), Func(m34),
            Func(m41), Func(m42), Func(m43), Func(m44));
    }

    template <typename U>
    constexpr auto MultMatrix(const TMatrix4<U> &M) noexcept
    {
        return *this = multiply(*this, M);
    }

    template <typename U>
    constexpr TMatrix4<T> &operator*=(const TMatrix4<U> &M) noexcept
    {
        return *this = multiply(*this, M);
    }

    template <typename U>
    constexpr TMatrix4<T> &operator+=(const TMatrix4<U> &M) noexcept
    {
        m11 += M.m11;
        m12 += M.m12;
        m13 += M.m13;
        m14 += M.m14;
        m21 += M.m21;
        m22 += M.m22;
        m23 += M.m23;
        m24 += M.m24;
        m31 += M.m31;
        m32 += M.m32;
        m33 += M.m33;
        m34 += M.m34;
        m41 += M.m41;
        m42 += M.m42;
        m43 += M.m43;
        m44 += M.m44;
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator-=(const TMatrix4<U> &M) noexcept
    {
        m11 -= M.m11;
        m12 -= M.m12;
        m13 -= M.m13;
        m14 -= M.m14;
        m21 -= M.m21;
        m22 -= M.m22;
        m23 -= M.m23;
        m24 -= M.m24;
        m31 -= M.m31;
        m32 -= M.m32;
        m33 -= M.m33;
        m34 -= M.m34;
        m41 -= M.m41;
        m42 -= M.m42;
        m43 -= M.m43;
        m44 -= M.m44;
        return *this;
    }

    //template <typename U> const TMatrix4<T> operator * (const TMatrix4<U> & M) const;
    //template <typename U> const TMatrix4<T> operator + (const TMatrix4<U> & M) const;
    //template <typename U> const TMatrix4<T> operator - (const TMatrix4<U> & M) const;

    template <typename U>
    constexpr TMatrix4<T> &operator+=(const U c) noexcept
    {
        m11 += c;
        m12 += c;
        m13 += c;
        m14 += c;
        m21 += c;
        m22 += c;
        m23 += c;
        m24 += c;
        m31 += c;
        m32 += c;
        m33 += c;
        m34 += c;
        m41 += c;
        m42 += c;
        m43 += c;
        m44 += c;
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator-=(const U c) noexcept
    {
        m11 -= c;
        m12 -= c;
        m13 -= c;
        m14 -= c;
        m21 -= c;
        m22 -= c;
        m23 -= c;
        m24 -= c;
        m31 -= c;
        m32 -= c;
        m33 -= c;
        m34 -= c;
        m41 -= c;
        m42 -= c;
        m43 -= c;
        m44 -= c;
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator*=(const U c) noexcept
    {
        m11 *= c;
        m12 *= c;
        m13 *= c;
        m14 *= c;
        m21 *= c;
        m22 *= c;
        m23 *= c;
        m24 *= c;
        m31 *= c;
        m32 *= c;
        m33 *= c;
        m34 *= c;
        m41 *= c;
        m42 *= c;
        m43 *= c;
        m44 *= c;
        return *this;
    }

    template <typename U>
    constexpr TMatrix4<T> &operator/=(const U c) noexcept
    {
        m11 /= c;
        m12 /= c;
        m13 /= c;
        m14 /= c;
        m21 /= c;
        m22 /= c;
        m23 /= c;
        m24 /= c;
        m31 /= c;
        m32 /= c;
        m33 /= c;
        m34 /= c;
        m41 /= c;
        m42 /= c;
        m43 /= c;
        m44 /= c;
        return *this;
    }

    template <typename U>
    constexpr auto Rotate(U angleInDegrees, U x, U y, U z) noexcept
    {
        return *this *= MakeRotation((T)angleInDegrees, (T)x, (T)y, (T)z);
    }

    template <typename U>
    constexpr auto Scale(U x, U y, U z) noexcept
    {
        return *this *= MakeScaling((T)x, (T)y, (T)z);
    }

    template <typename U>
    constexpr auto Translate(U x, U y, U z) noexcept
    {
        return *this *= MakeTranslation((T)x, (T)y, (T)z);
    }

    template <typename U>
    constexpr auto Ortho(U left, U right, U bottom, U top, U near_value, U far_value) noexcept
    {
        return *this *= MakeOrtho((T)left, (T)right, (T)bottom, (T)top, (T)near_value, (T)far_value);
    }

    template <typename U>
    constexpr auto Ortho2D(U left, U right, U bottom, U top) noexcept
    {
        return *this *= MakeOrtho2D((T)left, (T)right, (T)bottom, (T)top);
    }

    template <typename U>
    constexpr auto Frustum(U left, U right, U bottom, U top, U near_value, U far_value) noexcept
    {
        return *this *= MakeFrustum((T)left, (T)right, (T)bottom, (T)top, (T)near_value, (T)far_value);
    }

    template <typename U>
    constexpr auto Perspective(U angleInDegrees, U aspect, U near_value, U far_value) noexcept
    {
        return *this *= MakePerspective((T)angleInDegrees, (T)aspect, (T)near_value, (T)far_value);
    }

    template <typename U>
    constexpr auto PerspectiveX(U angleInDegrees, U aspect, U near_value, U far_value) noexcept
    {
        return *this *= MakePerspectiveX((T)angleInDegrees, (T)aspect, (T)near_value, (T)far_value);
    }

    template <typename U>
    constexpr auto PerspectiveY(U angleInDegrees, U aspect, U near_value, U far_value) noexcept
    {
        return *this *= MakePerspectiveY((T)angleInDegrees, (T)aspect, (T)near_value, (T)far_value);
    }

    template <typename U>
    constexpr auto LookAt(TVector3<U> eye, TVector3<U> center, TVector3<U> up) noexcept
    {
        return *this *= MakeLookAt((TVector3<T>)eye, (TVector3<T>)center, (TVector3<T>)up);
    }

    constexpr auto ShadowBias() noexcept
    {
        return *this *= MakeShadowBias();
    }

    constexpr auto CubeMatrix(int face) noexcept
    {
        return *this *= MakeCubeMatrix(face);
    }

    constexpr auto CubeMatrixPosition(int face, const TVector3<T> &position) noexcept
    {
        return *this *= MakeCubeMatrixPosition(face, position);
    }

    constexpr TMatrix4<T> AsTranspose() const noexcept
    {
        return TMatrix4<T>(
            m11, m21, m31, m41,
            m12, m22, m32, m42,
            m13, m23, m33, m43,
            m14, m24, m34, m44);
    }

    constexpr auto Transpose() noexcept
    {
        return *this = AsTranspose();
    }

    constexpr auto AsAdjugate() const noexcept
    {
        T t1 = m32 * m43 - m33 * m42;
        T t2 = m32 * m44 - m34 * m42;
        T t3 = m33 * m44 - m34 * m43;
        T t4 = m22 * m43 - m23 * m42;
        T t5 = m22 * m44 - m24 * m42;
        T t6 = m23 * m44 - m24 * m43;
        T t7 = m22 * m33 - m23 * m32;
        T t8 = m22 * m34 - m24 * m32;
        T t9 = m23 * m34 - m24 * m33;
        T t10 = m31 * m43 - m33 * m41;
        T t11 = m31 * m44 - m34 * m41;
        T t12 = m21 * m43 - m23 * m41;
        T t13 = m21 * m44 - m24 * m41;
        T t14 = m21 * m33 - m23 * m31;
        T t15 = m21 * m34 - m24 * m31;
        T t16 = m31 * m42 - m32 * m41;
        T t17 = m21 * m42 - m22 * m41;
        T t18 = m21 * m32 - m22 * m31;

        return TMatrix4<T>(m22 * t3 - m23 * t2 + m24 * t1,
                           -m12 * t3 + m13 * t2 - m14 * t1,
                           m12 * t6 - m13 * t5 + m14 * t4,
                           -m12 * t9 + m13 * t8 - m14 * t7,
                           -m21 * t3 + m23 * t11 - m24 * t10,
                           m11 * t3 - m13 * t11 + m14 * t10,
                           -m11 * t6 + m13 * t13 - m14 * t12,
                           m11 * t9 - m13 * t15 + m14 * t14,
                           m21 * t2 - m22 * t11 + m24 * t16,
                           -m11 * t2 + m12 * t11 - m14 * t16,
                           m11 * t5 - m12 * t13 + m14 * t17,
                           -m11 * t8 + m12 * t15 - m14 * t18,
                           -m21 * t1 + m22 * t10 - m23 * t16,
                           m11 * t1 - m12 * t10 + m13 * t16,
                           -m11 * t4 + m12 * t12 - m13 * t17,
                           m11 * t7 - m12 * t14 + m13 * t18);
    }

    constexpr auto Adjugate() noexcept
    {
        return *this = AsAdjugate();
    }

    constexpr auto AsQuickInverse() const noexcept
    {
        return TMatrix4<T>(
            m11, m21, m31, -m14,
            m12, m22, m32, -m24,
            m13, m23, m33, -m34,
            T(0), T(0), T(0), T(1));
    }

    constexpr auto QuickInvert() noexcept
    {
        if (Invertible())
            return *this = AsQuickInverse();
        return *this = MakeZero();
    }

    constexpr auto AsInverse() const noexcept
    {
        T t1 = m32 * m43 - m33 * m42;
        T t2 = m32 * m44 - m34 * m42;
        T t3 = m33 * m44 - m34 * m43;
        T t4 = m22 * t3 - m23 * t2 + m24 * t1;
        T t5 = m31 * m42 - m32 * m41;
        T t6 = m31 * m43 - m33 * m41;
        T t7 = -m21 * t1 + m22 * t6 - m23 * t5;
        T t8 = m31 * m44 - m34 * m41;
        T t9 = m21 * t2 - m22 * t8 + m24 * t5;
        T t10 = -m21 * t3 + m23 * t8 - m24 * t6;
        T t11 = static_cast<T>(1.0 / (m11 * t4 + m12 * t10 + m13 * t9 + m14 * t7));
        T t12 = m22 * m43 - m23 * m42;
        T t13 = m22 * m44 - m24 * m42;
        T t14 = m23 * m44 - m24 * m43;
        T t15 = m22 * m33 - m23 * m32;
        T t16 = m22 * m34 - m24 * m32;
        T t17 = m23 * m34 - m24 * m33;
        T t18 = m21 * m43 - m23 * m41;
        T t19 = m21 * m44 - m24 * m41;
        T t20 = m21 * m33 - m23 * m31;
        T t21 = m21 * m34 - m24 * m31;
        T t22 = m21 * m42 - m22 * m41;
        T t23 = m21 * m32 - m22 * m31;

        return TMatrix4<T>(
            t4 * t11,
            (-m12 * t3 + m13 * t2 - m14 * t1) * t11,
            (m12 * t14 - m13 * t13 + m14 * t12) * t11,
            (-m12 * t17 + m13 * t16 - m14 * t15) * t11,
            t10 * t11,
            (m11 * t3 - m13 * t8 + m14 * t6) * t11,
            (-m11 * t14 + m13 * t19 - m14 * t18) * t11,
            (m11 * t17 - m13 * t21 + m14 * t20) * t11,
            t9 * t11,
            (-m11 * t2 + m12 * t8 - m14 * t5) * t11,
            (m11 * t13 - m12 * t19 + m14 * t22) * t11,
            (-m11 * t16 + m12 * t21 - m14 * t23) * t11,
            t7 * t11,
            (m11 * t1 - m12 * t6 + m13 * t5) * t11,
            (-m11 * t12 + m12 * t18 - m13 * t22) * t11,
            (m11 * t15 - m12 * t20 + m13 * t23) * t11);
    }

    constexpr auto Invert() noexcept
    {
        if (Invertible())
            return *this = AsInverse();
        return *this = MakeZero();
    }

    constexpr bool Invertible() const noexcept
    {
        return Determinant() != 0;
    }

    constexpr T Determinant() const noexcept
    {
        T t1 = m31 * m42 - m32 * m41;
        T t2 = m31 * m43 - m33 * m41;
        T t3 = m32 * m43 - m33 * m42;
        T t4 = m31 * m44 - m34 * m41;
        T t5 = m32 * m44 - m34 * m42;
        T t6 = m33 * m44 - m34 * m43;
        return m11 * (m22 * t6 - m23 * t5 + m24 * t3) - m12 * (m21 * t6 - m23 * t4 + m24 * t2) + m13 * (m21 * t5 - m22 * t4 + m24 * t1) - m14 * (m21 * t3 - m22 * t2 + m23 * t1);
    }

    ///////////////////////////////////////////////////////////////
    // S T A T I C   M E M B E R S ////////////////////////////////
    ///////////////////////////////////////////////////////////////

    static constexpr auto MakeIdentity() noexcept
    {
        return TMatrix4<T>(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }

    static constexpr auto MakeZero() noexcept
    {
        return TMatrix4<T>(
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0);
    }

    static constexpr auto MakeRotation(T angleInDegrees, T x, T y, T z) noexcept
    {
        T angleInRadians = static_cast<T>(angleInDegrees * FX_DEGREES_TO_RADIANS);
        T c = cos(angleInRadians);
        T s = sin(angleInRadians);
        T invLength = (T)(1.0 / sqrt(x * x + y * y + z * z));
        x *= invLength;
        y *= invLength;
        z *= invLength;

        return TMatrix4<T>(
            x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0.0,
            y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0.0,
            x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0.0,
            0.0, 0.0, 0.0, 1.0);
    }

    static constexpr auto MakeScaling(T x, T y, T z) noexcept
    {
        return TMatrix4<T>(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1);
    }

    static constexpr auto MakeTranslation(T x, T y, T z) noexcept
    {
        return TMatrix4<T>(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1);
    }

    static constexpr auto MakeOrtho(T left, T right, T bottom, T top, T near_value, T far_value) noexcept
    {
        T tx = -(right + left) / (right - left);
        T ty = -(top + bottom) / (top - bottom);
        T tz = -(far_value + near_value) / (far_value - near_value);

        return TMatrix4<double>(
            2 / (right - left), 0, 0, tx,
            0, 2 / (top - bottom), 0, ty,
            0, 0, -2 / (far_value - near_value), tz,
            0, 0, 0, 1);
    }

    static constexpr auto MakeOrtho2D(T left, T right, T bottom, T top) noexcept
    {
        T tx = -(right + left) / (right - left);
        T ty = -(top + bottom) / (top - bottom);

        return TMatrix4<T>(
            2 / (right - left), 0, 0, tx,
            0, 2 / (top - bottom), 0, ty,
            0, 0, -1, 0,
            0, 0, 0, 1);
    }

    static constexpr auto MakeFrustum(T left, T right, T bottom, T top, T near_value, T far_value) noexcept
    {
        T A = (right + left) / (right - left);
        T B = (top + bottom) / (top - bottom);
        T C = -(far_value + near_value) / (far_value - near_value);
        T D = -2 * far_value * near_value / (far_value - near_value);

        return TMatrix4<T>(
            2 * near_value / (right - left), 0, A, 0,
            0, 2 * near_value / (top - bottom), B, 0,
            0, 0, C, D,
            0, 0, -1, 0);
    }

    static constexpr auto MakePerspective(T angleInDegrees, T aspect, T near_value, T far_value) noexcept
    {
        T f = static_cast<T>(1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees));

        return TMatrix4<T>(
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
            0, 0, -1, 0);
    }

    static constexpr auto MakePerspectiveX(T angleInDegrees, T aspect, T near_value, T far_value) noexcept
    {
        T f = static_cast<T>(1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees));

        return TMatrix4<T>(
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
            0, 0, -1, 0);
    }

    static constexpr auto MakePerspectiveY(T angleInDegrees, T aspect, T near_value, T far_value) noexcept
    {
        T f = static_cast<T>(1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees));

        return TMatrix4<T>(
            f, 0, 0, 0,
            0, f * aspect, 0, 0,
            0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
            0, 0, -1, 0);
    }

    static constexpr auto MakeLookAt(TVector3<T> eye, TVector3<T> center, TVector3<T> up) noexcept
    {
        TVector3<T> F = (center - eye).norm();
        TVector3<T> S = F.cross(up).norm();
        TVector3<T> U = S.cross(F).norm();

        return TMatrix4<T>::multiply(TMatrix4<T>(
                                         S.x, S.y, S.z, 0.0,
                                         U.x, U.y, U.z, 0.0,
                                         -F.x, -F.y, -F.z, 0.0,
                                         0.0, 0.0, 0.0, 1.0),
                                     TMatrix4<T>::MakeTranslation(-eye.x, -eye.y, -eye.z));
    }

    static constexpr auto MakeShadowBias() noexcept
    {
        return TMatrix4<T>(
            0.5, 0.0, 0.0, 0.5,
            0.0, 0.5, 0.0, 0.5,
            0.0, 0.0, 0.5, 0.5,
            0.0, 0.0, 0.0, 1.0);
    }

    static constexpr auto MakeCubeMatrix(int face) noexcept
    {
        // support GL_TEXTURE_CUBE_MAP_POSITIVE_X, ... constants
        if (face >= 0x8515 && face <= 0x851A)
            face -= 0x8515;

        switch (face)
        {
        case 0: // GL_TEXTURE_CUBE_MAP_POSITIVE_X
            return TMatrix4<T>::MakeRotation(90.0, 0.0, 1.0, 0.0);
            break;
        case 1: // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
            return TMatrix4<T>::MakeRotation(270.0, 0.0, 1.0, 0.0);
            break;
        case 2: // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
            return TMatrix4<T>::MakeRotation(270.0, 1.0, 0.0, 0.0);
            break;
        case 3: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            return TMatrix4<T>::MakeRotation(90.0, 1.0, 0.0, 0.0);
            break;
        case 4: // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
            return TMatrix4<T>::MakeIdentity();
            break;
        case 5: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
            return TMatrix4<T>::MakeRotation(180.0, 0.0, 1.0, 0.0);
            break;
        }
        return TMatrix4<T>::MakeIdentity();
    }

    static constexpr TMatrix4<T> MakeCubeMatrixPosition(int face, const TVector3<T> &position) noexcept
    {
        // support GL_TEXTURE_CUBE_MAP_POSITIVE_X, ... constants
        if (face >= 0x8515 && face <= 0x851A)
            face -= 0x8515;

        switch (face)
        {
        case 0: // GL_TEXTURE_CUBE_MAP_POSITIVE_X
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(1, 0, 0), TVector3<T>(0, -1, 0));
            break;
        case 1: // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(-1, 0, 0), TVector3<T>(0, -1, 0));
            break;
        case 2: // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(0, 1, 0), TVector3<T>(0, 0, 1));
            break;
        case 3: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(0, -1, 0), TVector3<T>(0, 0, -1));
            break;
        case 4: // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(0, 0, 1), TVector3<T>(0, -1, 0));
            break;
        case 5: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
            return TMatrix4<T>::MakeLookAt(position, position + TVector3<T>(0, 0, -1), TVector3<T>(0, -1, 0));
            break;
        }
        return TMatrix4<T>::MakeIdentity();
    }
};

//////////////////////////////////////////////////////////////////
// I M P L E M E N T A T I O N ///////////////////////////////////
//////////////////////////////////////////////////////////////////
//template <typename T>
//TMatrix4<T>::TMatrix4()
//{
//	m11 = 1; m12 = 0; m13 = 0; m14 = 0;
//	m21 = 0; m22 = 1; m23 = 0; m24 = 0;
//	m31 = 0; m32 = 0; m33 = 1; m34 = 0;
//	m41 = 0; m42 = 0; m43 = 0; m44 = 1;
//}
//
//template <typename T>
//TMatrix4<T>::TMatrix4(
//	const T a11, const T a12, const T a13, const T a14,
//	const T a21, const T a22, const T a23, const T a24,
//	const T a31, const T a32, const T a33, const T a34,
//	const T a41, const T a42, const T a43, const T a44
//	)
//{
//	m11 = a11; m12 = a12; m13 = a13; m14 = a14;
//	m21 = a21; m22 = a22; m23 = a23; m24 = a24;
//	m31 = a31; m32 = a32; m33 = a33; m34 = a34;
//	m41 = a41; m42 = a42; m43 = a43; m44 = a44;
//}

//template <typename T>
//TMatrix4<T>::TMatrix4(const TMatrix4<T> & M)
//{
//	m11 = M.m11; m12 = M.m12; m13 = M.m13; m14 = M.m14;
//	m21 = M.m21; m22 = M.m22; m23 = M.m23; m24 = M.m24;
//	m31 = M.m31; m32 = M.m32; m33 = M.m33; m34 = M.m34;
//	m41 = M.m41; m42 = M.m42; m43 = M.m43; m44 = M.m44;
//}
//
//template <typename T>
//TMatrix4<T>::TMatrix4(const T M[4][4])
//{
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			mm[i][j] = M[j][i];
//		}
//	}
//}
//
//template <typename T>
//TMatrix4<T>::TMatrix4(const T M[16])
//{
//	for (int i = 0; i < 16; i++)
//	{
//		ptr[i] = M[i];
//	}
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator = (const TMatrix4<U> & M)
//{
//	m11 = T(M.m11); m12 = T(M.m12); m13 = T(M.m13); m14 = T(M.m14);
//	m21 = T(M.m21); m22 = T(M.m22); m23 = T(M.m23); m24 = T(M.m24);
//	m31 = T(M.m31); m32 = T(M.m32); m33 = T(M.m33); m34 = T(M.m34);
//	m41 = T(M.m41); m42 = T(M.m42); m43 = T(M.m43); m44 = T(M.m44);
//	return *this;
//}

//
//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::LoadIdentity()
//{
//	m11 = 1; m12 = 0; m13 = 0; m14 = 0;
//	m21 = 0; m22 = 1; m23 = 0; m24 = 0;
//	m31 = 0; m32 = 0; m33 = 1; m34 = 0;
//	m41 = 0; m42 = 0; m43 = 0; m44 = 1;

//	return *this;
//}

//template <typename T>
//template <typename U>
//const TMatrix4<T> TMatrix4<T>::multiply(const TMatrix4<U> &M) const
//{
//	return TMatrix4<T>(
//		(T)(m14*M.m41 + m13*M.m31 + m12*M.m21 + m11*M.m11),
//		(T)(m14*M.m42 + m13*M.m32 + m12*M.m22 + m11*M.m12),
//		(T)(m14*M.m43 + m13*M.m33 + m12*M.m23 + m11*M.m13),
//		(T)(m14*M.m44 + m13*M.m34 + m12*M.m24 + m11*M.m14),

//		(T)(m24*M.m41 + m23*M.m31 + m22*M.m21 + m21*M.m11),
//		(T)(m24*M.m42 + m23*M.m32 + m22*M.m22 + m21*M.m12),
//		(T)(m24*M.m43 + m23*M.m33 + m22*M.m23 + m21*M.m13),
//		(T)(m24*M.m44 + m23*M.m34 + m22*M.m24 + m21*M.m14),

//		(T)(m34*M.m41 + m33*M.m31 + m32*M.m21 + m31*M.m11),
//		(T)(m34*M.m42 + m33*M.m32 + m32*M.m22 + m31*M.m12),
//		(T)(m34*M.m43 + m33*M.m33 + m32*M.m23 + m31*M.m13),
//		(T)(m34*M.m44 + m33*M.m34 + m32*M.m24 + m31*M.m14),

//		(T)(m44*M.m41 + m43*M.m31 + m42*M.m21 + m41*M.m11),
//		(T)(m44*M.m42 + m43*M.m32 + m42*M.m22 + m41*M.m12),
//		(T)(m44*M.m43 + m43*M.m33 + m42*M.m23 + m41*M.m13),
//		(T)(m44*M.m44 + m43*M.m34 + m42*M.m24 + m41*M.m14));
//}

//template <typename T>
//template <typename U>
//const TMatrix4<T> TMatrix4<T>::compMult(const TMatrix4<U> &ptr) const
//{
//	return TMatrix4<T>(
//		m11 * ptr.m11, m12 * ptr.m12, m13 * ptr.m13, m14 * ptr.m14,
//		m21 * ptr.m21, m22 * ptr.m22, m23 * ptr.m23, m24 * ptr.m24,
//		m31 * ptr.m31, m32 * ptr.m32, m33 * ptr.m33, m34 * ptr.m34,
//		m41 * ptr.m41, m42 * ptr.m42, m43 * ptr.m43, m44 * ptr.m44
//		);
//}

//template<typename T>
//template<typename U>
//constexpr TMatrix4<T> TMatrix4<T>::multiply(TMatrix4<T> m1, TMatrix4<U> m2)
//{
//	return TMatrix4<T>(
//		(T)(m1.m14*m2.m41 + m1.m13*m2.m31 + m1.m12*m2.m21 + m1.m11*m2.m11),
//		(T)(m1.m14*m2.m42 + m1.m13*m2.m32 + m1.m12*m2.m22 + m1.m11*m2.m12),
//		(T)(m1.m14*m2.m43 + m1.m13*m2.m33 + m1.m12*m2.m23 + m1.m11*m2.m13),
//		(T)(m1.m14*m2.m44 + m1.m13*m2.m34 + m1.m12*m2.m24 + m1.m11*m2.m14),

//		(T)(m1.m24*m2.m41 + m1.m23*m2.m31 + m1.m22*m2.m21 + m1.m21*m2.m11),
//		(T)(m1.m24*m2.m42 + m1.m23*m2.m32 + m1.m22*m2.m22 + m1.m21*m2.m12),
//		(T)(m1.m24*m2.m43 + m1.m23*m2.m33 + m1.m22*m2.m23 + m1.m21*m2.m13),
//		(T)(m1.m24*m2.m44 + m1.m23*m2.m34 + m1.m22*m2.m24 + m1.m21*m2.m14),

//		(T)(m1.m34*m2.m41 + m1.m33*m2.m31 + m1.m32*m2.m21 + m1.m31*m2.m11),
//		(T)(m1.m34*m2.m42 + m1.m33*m2.m32 + m1.m32*m2.m22 + m1.m31*m2.m12),
//		(T)(m1.m34*m2.m43 + m1.m33*m2.m33 + m1.m32*m2.m23 + m1.m31*m2.m13),
//		(T)(m1.m34*m2.m44 + m1.m33*m2.m34 + m1.m32*m2.m24 + m1.m31*m2.m14),

//		(T)(m1.m44*m2.m41 + m1.m43*m2.m31 + m1.m42*m2.m21 + m1.m41*m2.m11),
//		(T)(m1.m44*m2.m42 + m1.m43*m2.m32 + m1.m42*m2.m22 + m1.m41*m2.m12),
//		(T)(m1.m44*m2.m43 + m1.m43*m2.m33 + m1.m42*m2.m23 + m1.m41*m2.m13),
//		(T)(m1.m44*m2.m44 + m1.m43*m2.m34 + m1.m42*m2.m24 + m1.m41*m2.m14));
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::MultMatrix(const TMatrix4<U> & M)
//{
//	return *this = multiply(M);
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator *= (const TMatrix4<U> & M)
//{
//	return *this = multiply(M);
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator += (const TMatrix4<U> & M)
//{
//	m11 += M.m11; m12 += M.m12; m13 += M.m13; m14 += M.m14;
//	m21 += M.m21; m22 += M.m22; m23 += M.m23; m24 += M.m24;
//	m31 += M.m31; m32 += M.m32; m33 += M.m33; m34 += M.m34;
//	m41 += M.m41; m42 += M.m42; m43 += M.m43; m44 += M.m44;
//	return *this;
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator -= (const TMatrix4<U> & M)
//{
//	m11 -= M.m11; m12 -= M.m12; m13 -= M.m13; m14 -= M.m14;
//	m21 -= M.m21; m22 -= M.m22; m23 -= M.m23; m24 -= M.m24;
//	m31 -= M.m31; m32 -= M.m32; m33 -= M.m33; m34 -= M.m34;
//	m41 -= M.m41; m42 -= M.m42; m43 -= M.m43; m44 -= M.m44;
//	return *this;
//}

//template <typename T>
//template <typename U>
//const TMatrix4<T> TMatrix4<T>::operator * (const TMatrix4<U> & M) const
//{
//	return multiply(M);
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> TMatrix4<T>::operator + (const TMatrix4<U> & M) const
//{
//	return TMatrix4<T>(
//		m11 + M.m11, m12 + M.m12, m13 + M.m13, m14 + M.m14,
//		m21 + M.m21, m22 + M.m22, m23 + M.m23, m24 + M.m24,
//		m31 + M.m31, m32 + M.m32, m33 + M.m33, m34 + M.m34,
//		m41 + M.m41, m42 + M.m42, m43 + M.m43, m44 + M.m44
//	);
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> TMatrix4<T>::operator - (const TMatrix4<U> & M) const
//{
//	return TMatrix4<T>(
//		m11 - M.m11, m12 - M.m12, m13 - M.m13, m14 - M.m14,
//		m21 - M.m21, m22 - M.m22, m23 - M.m23, m24 - M.m24,
//		m31 - M.m31, m32 - M.m32, m33 - M.m33, m34 - M.m34,
//		m41 - M.m41, m42 - M.m42, m43 - M.m43, m44 - M.m44
//		);
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator *= (const U X)
//{
//	for (int i = 0; i < 16; i++)
//		ptr[i] *= X;
//	return *this;
//}
//
//template <typename T>
//template <typename U>
//const TMatrix4<T> & TMatrix4<T>::operator /= (const U X)
//{
//	for (int i = 0; i < 16; i++)
//		ptr[i] /= X;
//	return *this;
//}

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Rotate(double angleInDegrees, double X, double y, double z)
//{
//	return MultMatrix(TMatrix4<double>::MakeRotation(angleInDegrees, X, y, z));
//}

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Scale(double X, double y, double z)
//{
//	return MultMatrix(TMatrix4<double>::MakeScaling(X, y, z));
//}
//

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Translate(double X, double y, double z)
//{
//	return MultMatrix(TMatrix4<double>::MakeTranslation(X, y, z));
//}
//

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Ortho(double left, double right, double bottom, double top, double near_value, double far_value)
//{
//	return MultMatrix(TMatrix4<double>::MakeOrtho(left, right, bottom, top, near_value, far_value));
//}
//

//template <typename T>
//const TMatrix4<T> &  TMatrix4<T>::Ortho2D(double left, double right, double bottom, double top)
//{
//	return MultMatrix(TMatrix4<double>::MakeOrtho2D(left, right, bottom, top));
//}
//

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Frustum(double left, double right, double bottom, double top, double near_value, double far_value)
//{
//	return MultMatrix(TMatrix4<double>::MakeFrustum(left, right, bottom, top, near_value, far_value));
//}
//

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::Perspective(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	return MultMatrix(TMatrix4<double>::MakePerspective(angleInDegrees, aspect, near_value, far_value));
//}

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::PerspectiveX(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	return MultMatrix(TMatrix4<double>::MakePerspectiveX(angleInDegrees, aspect, near_value, far_value));
//}

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::PerspectiveY(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	return MultMatrix(TMatrix4<double>::MakePerspectiveY(angleInDegrees, aspect, near_value, far_value));
//}

//template <typename T>
//const TMatrix4<T> &TMatrix4<T>::LookAt(TVector3<double> eye, TVector3<double> center, TVector3<double> up)
//{
//	return MultMatrix(TMatrix4<double>::MakeLookAt(eye, center, up));
//}

//template <typename T>
//const TMatrix4<T> & TMatrix4<T>::ShadowBias()
//{
//	return MultMatrix(TMatrix4<double>::MakeShadowBias());
//}

//template <typename T>
//const TMatrix4<T> &TMatrix4<T>::CubeMatrix(int face)
//{
//	return MultMatrix(TMatrix4<double>::MakeCubeMatrix(face));
//}

//template <typename T>
//constexpr TMatrix4<T> & TMatrix4<T>::Transpose()
//{
//	return *this = AsTranspose();
//}

//template <typename T>
//TMatrix4<T> TMatrix4<T>::AsTranspose() const
//{
//	return TMatrix4<T>(
//		t11, t12, t13, t14,
//		t21, t22, t23, t24,
//		t31, t32, t33, t34,
//		t41, t42, t43, t44
//	);
//}

//template <typename T>
//T TMatrix4<T>::Determinant() const
//{
//	T t1 = m31 * m42 - m32 * m41;
//	T t2 = m31 * m43 - m33 * m41;
//	T t3 = m32 * m43 - m33 * m42;
//	T t4 = m31 * m44 - m34 * m41;
//	T t5 = m32 * m44 - m34 * m42;
//	T t6 = m33 * m44 - m34 * m43;
//	return
//		m11 * (m22 * t6 - m23 * t5 + m24 * t3) -
//		m12 * (m21 * t6 - m23 * t4 + m24 * t2) +
//		m13 * (m21 * t5 - m22 * t4 + m24 * t1) -
//		m14 * (m21 * t3 - m22 * t2 + m23 * t1);
//}

//template <typename T>
//TMatrix4<T> TMatrix4<T>::AsAdjugate() const
//{
//	T t1 = m32 * m43 - m33 * m42;
//	T t2 = m32 * m44 - m34 * m42;
//	T t3 = m33 * m44 - m34 * m43;
//	T t4 = m22 * m43 - m23 * m42;
//	T t5 = m22 * m44 - m24 * m42;
//	T t6 = m23 * m44 - m24 * m43;
//	T t7 = m22 * m33 - m23 * m32;
//	T t8 = m22 * m34 - m24 * m32;
//	T t9 = m23 * m34 - m24 * m33;
//	T t10 = m31 * m43 - m33 * m41;
//	T t11 = m31 * m44 - m34 * m41;
//	T t12 = m21 * m43 - m23 * m41;
//	T t13 = m21 * m44 - m24 * m41;
//	T t14 = m21 * m33 - m23 * m31;
//	T t15 = m21 * m34 - m24 * m31;
//	T t16 = m31 * m42 - m32 * m41;
//	T t17 = m21 * m42 - m22 * m41;
//	T t18 = m21 * m32 - m22 * m31;

//	return TMatrix4<T>(m22 * t3 - m23 * t2 + m24 * t1,
//		-m12 * t3 + m13 * t2 - m14 * t1,
//		m12 * t6 - m13 * t5 + m14 * t4,
//		-m12 * t9 + m13 * t8 - m14 * t7,
//		-m21 * t3 + m23 * t11 - m24 * t10,
//		m11 * t3 - m13 * t11 + m14 * t10,
//		-m11 * t6 + m13 * t13 - m14 * t12,
//		m11 * t9 - m13 * t15 + m14 * t14,
//		m21 * t2 - m22 * t11 + m24 * t16,
//		-m11 * t2 + m12 * t11 - m14 * t16,
//		m11 * t5 - m12 * t13 + m14 * t17,
//		-m11 * t8 + m12 * t15 - m14 * t18,
//		-m21 * t1 + m22 * t10 - m23 * t16,
//		m11 * t1 - m12 * t10 + m13 * t16,
//		-m11 * t4 + m12 * t12 - m13 * t17,
//		m11 * t7 - m12 * t14 + m13 * t18);
//}
//
//template <typename T>
//TMatrix4<T> & TMatrix4<T>::Adjugate()
//{
//	return *this = AsAdjugate();
//}
//
//template <typename T>
//bool TMatrix4<T>::Invert()
//{
//	T det;
//
//	det = Determinant();
//	if (det == 0)
//		return false;

//	*this = AsInverse();

//	return true;
//}

////template <typename T>
////TMatrix4<T> TMatrix4<T>::QuickInverse() const
////{
////	return TMatrix4<T>(
////		t11, t12, t13, -m14,
////		t21, t22, t23, -m24,
////		t31, t32, t33, -m34,
////		T(0), T(0), T(0), T(1)
////		);
////}

////template <typename T>
////void TMatrix4<T>::QuickInvert()
////{
////	*this = QuickInverse();
////}

//template <typename T>
//TMatrix4<double> TMatrix4<T>::AsInverse() const
//{
//	double t1 = m32 * m43 - m33 * m42;
//	double t2 = m32 * m44 - m34 * m42;
//	double t3 = m33 * m44 - m34 * m43;
//	double t4 = m22 * t3 - m23 * t2 + m24 * t1;
//	double t5 = m31 * m42 - m32 * m41;
//	double t6 = m31 * m43 - m33 * m41;
//	double t7 = -m21 * t1 + m22 * t6 - m23 * t5;
//	double t8 = m31 * m44 - m34 * m41;
//	double t9 = m21 * t2 - m22 * t8 + m24 * t5;
//	double t10 = -m21 * t3 + m23 * t8 - m24 * t6;
//	double t11 = 1.0 / (m11 * t4 + m12 * t10 + m13 * t9 + m14 * t7);
//	double t12 = m22 * m43 - m23 * m42;
//	double t13 = m22 * m44 - m24 * m42;
//	double t14 = m23 * m44 - m24 * m43;
//	double t15 = m22 * m33 - m23 * m32;
//	double t16 = m22 * m34 - m24 * m32;
//	double t17 = m23 * m34 - m24 * m33;
//	double t18 = m21 * m43 - m23 * m41;
//	double t19 = m21 * m44 - m24 * m41;
//	double t20 = m21 * m33 - m23 * m31;
//	double t21 = m21 * m34 - m24 * m31;
//	double t22 = m21 * m42 - m22 * m41;
//	double t23 = m21 * m32 - m22 * m31;

//	return TMatrix4<double>(
//		t4 * t11,
//		(-m12 * t3 + m13 * t2 - m14 * t1) * t11,
//		(m12 * t14 - m13 * t13 + m14 * t12) * t11,
//		(-m12 * t17 + m13 * t16 - m14 * t15) * t11,
//		t10 * t11,
//		(m11 * t3 - m13 * t8 + m14 * t6) * t11,
//		(-m11 * t14 + m13 * t19 - m14 * t18) * t11,
//		(m11 * t17 - m13 * t21 + m14 * t20) * t11,
//		t9 * t11,
//		(-m11 * t2 + m12 * t8 - m14 * t5) * t11,
//		(m11 * t13 - m12 * t19 + m14 * t22) * t11,
//		(-m11 * t16 + m12 * t21 - m14 * t23) * t11,
//		t7 * t11,
//		(m11 * t1 - m12 * t6 + m13 * t5) * t11,
//		(-m11 * t12 + m12 * t18 - m13 * t22) * t11,
//		(m11 * t15 - m12 * t20 + m13 * t23) * t11
//		);
//}

//template <typename T>
//TMatrix4<double> TMatrix4<T>::makeRotation(double angleInDegrees, double X, double y, double z)
//{
//	double angleInRadians = angleInDegrees * FX_DEGREES_TO_RADIANS;
//	double c = cos(angleInRadians);
//	double s = sin(angleInRadians);
//	double invLength = 1.0 / sqrt(X*X + y*y + z*z);
//	X *= invLength;
//	y *= invLength;
//	z *= invLength;

//	return TMatrix4<double>(
//		X * X * (1 - c) + c, X * y * (1 - c) - z * s, X * z * (1 - c) + y * s, 0.0,
//		y * X * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - X * s, 0.0,
//		X * z * (1 - c) - y * s, y * z * (1 - c) + X * s, z * z * (1 - c) + c, 0.0,
//		0.0, 0.0, 0.0, 1.0
//		);
//}

//template <typename T>
//TMatrix4<double> TMatrix4<T>::makeScale(double X, double y, double z)
//{
//	return TMatrix4<double>(
//		X, 0.0, 0.0, 0.0,
//		0.0, y, 1.0, 0.0,
//		0.0, 0.0, z, 0.0,
//		0.0, 0.0, 0.0, 1.0
//		);
//}

//template <typename T>
//TMatrix4<double> TMatrix4<T>::makeTranslation(double X, double y, double z)
//{
//	return TMatrix4<double>(
//		0.0, 0.0, 0.0, X,
//		0.0, 0.0, 1.0, y,
//		0.0, 0.0, 0.0, z,
//		0.0, 0.0, 0.0, 1.0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeRotation(double angleInDegrees, double X, double y, double z)
//{
//	double angleInRadians = angleInDegrees * FX_DEGREES_TO_RADIANS;
//	double c = cos(angleInRadians);
//	double s = sin(angleInRadians);
//	double invLength = 1.0 / sqrt(X*X + y*y + z*z);
//	X *= invLength;
//	y *= invLength;
//	z *= invLength;

//	return TMatrix4<double>(
//		X * X * (1 - c) + c, X * y * (1 - c) - z * s, X * z * (1 - c) + y * s, 0.0,
//		y * X * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - X * s, 0.0,
//		X * z * (1 - c) - y * s, y * z * (1 - c) + X * s, z * z * (1 - c) + c, 0.0,
//		0.0, 0.0, 0.0, 1.0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeScaling(double X, double y, double z)
//{
//	return TMatrix4<double>(
//		X, 0, 0, 0,
//		0, y, 0, 0,
//		0, 0, z, 0,
//		0, 0, 0, 1
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeTranslation(double X, double y, double z)
//{
//	return TMatrix4<double>(
//		1, 0, 0, X,
//		0, 1, 0, y,
//		0, 0, 1, z,
//		0, 0, 0, 1
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeOrtho(double left, double right, double bottom, double top, double near_value, double far_value)
//{
//	double tx = -(right + left) / (right - left);
//	double ty = -(top + bottom) / (top - bottom);
//	double tz = -(far_value + near_value) / (far_value - near_value);

//	return TMatrix4<double>(
//		2 / (right - left), 0, 0, tx,
//		0, 2 / (top - bottom), 0, ty,
//		0, 0, -2 / (far_value - near_value), tz,
//		0, 0, 0, 1
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeOrtho2D(double left, double right, double bottom, double top)
//{
//	double tx = -(right + left) / (right - left);
//	double ty = -(top + bottom) / (top - bottom);

//	return TMatrix4<double>(
//		2 / (right - left), 0, 0, tx,
//		0, 2 / (top - bottom), 0, ty,
//		0, 0, -1, 0,
//		0, 0, 0, 1
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeFrustum(double left, double right, double bottom, double top, double near_value, double far_value)
//{
//	double A = (right + left) / (right - left);
//	double B = (top + bottom) / (top - bottom);
//	double C = -(far_value + near_value) / (far_value - near_value);
//	double D = -2 * far_value * near_value / (far_value - near_value);

//	return TMatrix4<double>(
//		2 * near_value / (right - left), 0, A, 0,
//		0, 2 * near_value / (top - bottom), B, 0,
//		0, 0, C, D,
//		0, 0, -1, 0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakePerspective(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	double f = 1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees);

//	return TMatrix4<double>(
//		f / aspect, 0, 0, 0,
//		0, f, 0, 0,
//		0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
//		0, 0, -1, 0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakePerspectiveX(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	double f = 1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees);

//	return TMatrix4<double>(
//		f / aspect, 0, 0, 0,
//		0, f, 0, 0,
//		0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
//		0, 0, -1, 0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakePerspectiveY(double angleInDegrees, double aspect, double near_value, double far_value)
//{
//	double f = 1.0 / std::tan(FX_DEGREES_TO_RADIANS * 0.5 * angleInDegrees);

//	return TMatrix4<double>(
//		f, 0, 0, 0,
//		0, f * aspect, 0, 0,
//		0, 0, (far_value + near_value) / (near_value - far_value), 2 * far_value * near_value / (near_value - far_value),
//		0, 0, -1, 0
//		);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeLookAt(TVector3<double> eye, TVector3<double> center, TVector3<double> up)
//{
//	TVector3<double> F = (center - eye).norm();
//	TVector3<double> S = F.cross(up).norm();
//	TVector3<double> U = S.cross(F).norm();

//	return TMatrix4<double>::multiply(TMatrix4<double>(
//		S.X, S.y, S.z, 0.0,
//		U.X, U.y, U.z, 0.0,
//		-F.X, -F.y, -F.z, 0.0,
//		0.0, 0.0, 0.0, 1.0),
//		TMatrix4<double>::MakeTranslation(-eye.X, -eye.y, -eye.z));
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeShadowBias()
//{
//	return TMatrix4<double>(
//		0.5, 0.0, 0.0, 0.5,
//		0.0, 0.5, 0.0, 0.5,
//		0.0, 0.0, 0.5, 0.5,
//		0.0, 0.0, 0.0, 1.0);
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeCubeMatrix(int face)
//{
//	// support GL_TEXTURE_CUBE_MAP_POSITIVE_X, ... constants
//	if (face >= 0x8515 && face <= 0x851A) face -= 0x8515;

//	switch (face)
//	{
//	case 0: // GL_TEXTURE_CUBE_MAP_POSITIVE_X
//		return TMatrix4<double>::MakeRotation(90.0, 0.0, 1.0, 0.0);
//		break;
//	case 1: // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
//		return TMatrix4<double>::MakeRotation(270.0, 0.0, 1.0, 0.0);
//		break;
//	case 2: // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
//		return TMatrix4<double>::MakeRotation(270.0, 1.0, 0.0, 0.0);
//		break;
//	case 3: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
//		return TMatrix4<double>::MakeRotation(90.0, 1.0, 0.0, 0.0);
//		break;
//	case 4: // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
//		return TMatrix4<double>::MakeIdentity();
//		break;
//	case 5: // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
//		return TMatrix4<double>::MakeRotation(180.0, 0.0, 1.0, 0.0);
//		break;
//	}
//	return TMatrix4<double>::MakeIdentity();
//}

//template <typename T>
//constexpr TMatrix4<double> TMatrix4<T>::MakeIdentity()
//{
//	return TMatrix4<double>(
//		1.0, 0.0, 0.0, 0.0,
//		0.0, 1.0, 0.0, 0.0,
//		0.0, 0.0, 1.0, 0.0,
//		0.0, 0.0, 0.0, 1.0);
//}

using Matrix4f = TMatrix4<float>;
using Matrix4d = TMatrix4<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TMatrix4<float>;
extern template class TMatrix4<double>;
#endif

template <typename T, typename U>
constexpr auto operator*(const TMatrix4<T> &m1, const TMatrix4<U> &m2) noexcept
{
    return TMatrix4<T>::multiply(m1, m2);
}

template <typename T, typename U>
constexpr auto operator*(const TMatrix4<T> &M, const TVector2<U> &V) noexcept
{
    return TVector2<std::common_type_t<T, U>>(
        M.m11 * V.x + M.m12 * V.y,
        M.m21 * V.x + M.m22 * V.y);
}

template <typename T, typename U>
constexpr auto operator*(const TMatrix4<T> &M, const TVector3<U> &V) noexcept
{
    return TVector2<std::common_type_t<T, U>>(
        M.m11 * V.x + M.m12 * V.y + M.m13 * V.z,
        M.m21 * V.x + M.m22 * V.y + M.m23 * V.z,
        M.m31 * V.x + M.m32 * V.y + M.m33 * V.z);
}

template <typename T, typename U>
constexpr auto operator*(const TMatrix4<T> &M, const TVector4<U> &V) noexcept
{
    return TVector4<std::common_type_t<T, U>>(
        M.m11 * V.x + M.m12 * V.y + M.m13 * V.z + M.m14 * V.w,
        M.m21 * V.x + M.m22 * V.y + M.m23 * V.z + M.m24 * V.w,
        M.m31 * V.x + M.m32 * V.y + M.m33 * V.z + M.m34 * V.w,
        M.m41 * V.x + M.m42 * V.y + M.m43 * V.z + M.m44 * V.w);
}
} // namespace Fluxions

#endif
