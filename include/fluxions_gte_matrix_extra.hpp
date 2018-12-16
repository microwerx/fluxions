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
#ifndef FLUXIONS_GTE_MATRIX_EXTRA_HPP
#define FLUXIONS_GTE_MATRIX_EXTRA_HPP

#include <vector>
#include <fluxions_gte_matrix2.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
// Forward declarations
template <typename T>
class TMatrix2x3;
template <typename T>
class TMatrix2x4;
template <typename T>
class TMatrix3x2;
template <typename T>
class TMatrix3x4;
template <typename T>
class TMatrix4x2;
template <typename T>
class TMatrix4x3;

template <typename T>
class TColor3;
template <typename T>
class TColor4;

template <typename T>
class TMatrixM
{
  public:
    T m11, m21, m31, m41;
    T m12, m22, m32, m42;
    T m13, m23, m33, m43;
    T m14, m24, m34, m44;

    constexpr T *ptr() noexcept { return &m11; }
    constexpr const T *const_ptr() const noexcept { return &m11; }

    using type = T;

    TMatrixM()
        : m11(0), m21(0), m31(0), m41(0), m12(0), m22(0), m32(0), m42(0), m13(0), m23(0), m33(0), m43(0), m14(0), m24(0), m34(0), m44(0)
    {
    }

    TMatrixM(
        const T a11, const T a12, const T a13, const T a14,
        const T a21, const T a22, const T a23, const T a24,
        const T a31, const T a32, const T a33, const T a34,
        const T a41, const T a42, const T a43, const T a44)
        : m11(a11), m21(a21), m31(a31), m41(a41), m12(a12), m22(a22), m32(a32), m42(a42), m13(a13), m23(a23), m33(a33), m43(a43), m14(a14), m24(a24), m34(a34), m44(a44)
    {
    }

    TMatrixM(const TMatrixM<T> &M)
        : m11(M.m11), m21(M.m21), m31(M.m31), m41(M.m41), m12(M.m12), m22(M.m22), m32(M.m32), m42(M.m42), m13(M.m13), m23(M.m23), m33(M.m33), m43(M.m43), m14(M.m14), m24(M.m24), m34(M.m34), m44(M.m44)
    {
    }

    TMatrixM(const T M[16])
        : m11(M[0]), m21(M[1]), m31(M[2]), m41(M[3]), m12(M[4]), m22(M[5]), m32(M[6]), m42(M[7]), m13(M[8]), m23(M[9]), m33(M[10]), m43(M[11]), m14(M[12]), m24(M[13]), m34(M[14]), m44(M[15])
    {
    }

    template <typename Other>
    const TMatrixM<T> &operator=(const TMatrixM<Other> &M)
    {
        m11 = M.m11;
        m21 = M.m21;
        m31 = M.m31;
        m41 = M.m41;
        m12 = M.m12;
        m22 = M.m22;
        m32 = M.m32;
        m42 = M.m42;
        m13 = M.m13;
        m23 = M.m23;
        m33 = M.m33;
        m43 = M.m43;
        m14 = M.m14;
        m24 = M.m24;
        m34 = M.m34;
        m44 = M.m44;
        return *this;
    }

    template <typename Other>
    operator TMatrixM<Other>() const
    {
        return TMatrixM<Other>(
            m11, m12, m13, m14,
            m21, m22, m23, m24,
            m31, m32, m33, m34,
            m41, m42, m43, m44);
    }

    TMatrixM<T> AsTranspose() const
    {
        return TMatrixM<T>(
            m11, m21, m31, m41,
            m12, m22, m32, m42,
            m13, m23, m33, m43,
            m14, m24, m34, m44);
    }

    T *AsColMajorArray()
    {
        return &m11;
    }

    TMatrix2<T> AsMatrix2() const
    {
        return TMatrix2<T>(
            m11, m12,
            m21, m22);
    }

    TMatrix3<T> AsMatrix3() const
    {
        return TMatrix3<T>(
            m11, m12, m13,
            m21, m22, m23,
            m31, m32, m33);
    }

    TMatrix4<T> AsMatrix4() const
    {
        return TMatrix4<T>(
            m11, m12, m13, m14,
            m21, m22, m23, m24,
            m31, m32, m33, m34,
            m41, m42, m43, m44);
    }
};

//// TMatrix2x3
//template <typename T>
//class TMatrix2x3
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21;
//			T m12, m22;
//			T m13, m23;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12, t13;
//			T t21, t22, t23;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[16];
//		};
//	};
//
//	TMatrix2x3()
//		: m11(0), m12(0), m13(0),
//		  m21(0), m22(0), m23(0)
//	{
//	}
//
//	TMatrix2x3(
//		const T a11, const T a12, const T a13,
//		const T a21, const T a22, const T a23)
//		: m11(a11), m12(a12), m13(a13),
//		  m21(a21), m22(a22), m23(a23)
//	{
//	}
//
//	TMatrix2x3(const TMatrix2x3<T> &M) : m11(M.m11), m12(M.m12), m13(M.m13),
//										 m21(M.m21), m22(M.m22), m23(M.m23)
//	{
//	}
//
//	TMatrix2x3(const T M[16]) : m11(M[0]), m12(M[2]), m13(M[4]),
//								m21(M[1]), m22(M[3]), m23(M[5])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix2x3<T> &operator=(const TMatrix2x3<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m13 = M.m13;
//		m21 = M.m21;
//		m22 = M.m22;
//		m23 = M.m23;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix2x3<Other>() const
//	{
//		return TMatrix2x3<Other>(
//			(Other)m11, (Other)m12, (Other)m13,
//			(Other)m21, (Other)m22, (Other)m23);
//	}
//
//	const TMatrix3x2<T> AsTranspose() const
//	{
//		return TMatrix3x2<T>(
//			t11, t21,
//			t12, t22,
//			t13, t23);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, m13,
//			m21, m22, m23,
//			0, 0, 0);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, m13, 0,
//			m21, m22, m23, 0,
//			0, 0, 0, 0,
//			0, 0, 0, 0);
//	}
//};
//
//// TMatrix2x4
//template <typename T>
//class TMatrix2x4
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21;
//			T m12, m22;
//			T m13, m23;
//			T m14, m24;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12, t13, t14;
//			T t21, t22, t23, t24;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[8];
//		};
//	};
//
//	TMatrix2x4()
//		: m11(0), m12(0), m13(0), m14(0),
//		  m21(0), m22(0), m23(0), m24(0)
//	{
//	}
//
//	TMatrix2x4(
//		const T a11, const T a12, const T a13, const T a14,
//		const T a21, const T a22, const T a23, const T a24)
//		: m11(a11), m12(a12), m13(a13), m14(a14),
//		  m21(a21), m22(a22), m23(a23), m24(a24)
//	{
//	}
//
//	TMatrix2x4(const TMatrix2x4<T> &M) : m11(M.m11), m12(M.m12), m13(M.m13), m14(M.m14),
//										 m21(M.m21), m22(M.m22), m23(M.m23), m24(M.m24)
//	{
//	}
//
//	TMatrix2x4(const T M[16]) : m11(M[0]), m12(M[2]), m13(M[4]), m14(M[6]),
//								m21(M[1]), m22(M[3]), m23(M[5]), m24(M[7])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix2x4<T> &operator=(const TMatrix2x4<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m13 = M.m13;
//		m14 = M.m14;
//		m21 = M.m21;
//		m22 = M.m22;
//		m23 = M.m23;
//		m24 = M.m24;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix2x4<Other>() const
//	{
//		return TMatrix2x4<Other>(
//			(Other)m11, (Other)m12, (Other)m13, (Other)m14,
//			(Other)m21, (Other)m22, (Other)m23, (Other)m24);
//	}
//
//	const TMatrix4x2<T> AsTranspose() const
//	{
//		return TMatrix4x2<T>(
//			t11, t21,
//			t12, t22,
//			t13, t23,
//			t14, t24);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, m13,
//			m21, m22, m23,
//			0, 0, 0);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, m13, m14,
//			m21, m22, m23, m24,
//			0, 0, 0, 0,
//			0, 0, 0, 0);
//	}
//};
//
//// TMatrix3x2
//template <typename T>
//class TMatrix3x2
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21, m31;
//			T m12, m22, m32;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12;
//			T t21, t22;
//			T t31, t32;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[6];
//		};
//	};
//
//	TMatrix3x2()
//		: m11(0), m12(0),
//		  m21(0), m22(0),
//		  m31(0), m32(0)
//	{
//	}
//
//	TMatrix3x2(
//		const T a11, const T a12,
//		const T a21, const T a22,
//		const T a31, const T a32)
//		: m11(a11), m12(a12),
//		  m21(a21), m22(a22),
//		  m31(a31), m32(a32)
//	{
//	}
//
//	TMatrix3x2(const TMatrix3x2<T> &M) : m11(M.m11), m12(M.m12),
//										 m21(M.m21), m22(M.m22),
//										 m31(M.m31), m32(M.m32)
//	{
//	}
//
//	TMatrix3x2(const T M[16]) : m11(M[0]), m12(M[3]),
//								m21(M[1]), m22(M[4]),
//								m31(M[2]), m32(M[5])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix3x2<T> &operator=(const TMatrix3x2<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m21 = M.m21;
//		m22 = M.m22;
//		m31 = M.m31;
//		m32 = M.m32;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix3x2<Other>() const
//	{
//		return TMatrix3x2<Other>(
//			(Other)m11, (Other)m12,
//			(Other)m21, (Other)m22,
//			(Other)m31, (Other)m32);
//	}
//
//	const TMatrix2x3<T> AsTranspose() const
//	{
//		return TMatrix2x3<T>(
//			t11, t21, t31,
//			t12, t22, t32);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, 0,
//			m21, m22, 0,
//			m31, m32, 0);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, 0, 0,
//			m21, m22, 0, 0,
//			m31, m32, 0, 0,
//			0, 0, 0, 0);
//	}
//};
//
//// TMatrix3x4
//template <typename T>
//class TMatrix3x4
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21, m31;
//			T m12, m22, m32;
//			T m13, m23, m33;
//			T m14, m24, m34;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12, t13, t14;
//			T t21, t22, t23, t24;
//			T t31, t32, t33, t34;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[12];
//		};
//	};
//
//	TMatrix3x4()
//		: m11(0), m12(0), m13(0), m14(0),
//		  m21(0), m22(0), m23(0), m24(0),
//		  m31(0), m32(0), m33(0), m34(0)
//	{
//	}
//
//	TMatrix3x4(
//		const T a11, const T a12, const T a13, const T a14,
//		const T a21, const T a22, const T a23, const T a24,
//		const T a31, const T a32, const T a33, const T a34)
//		: m11(a11), m12(a12), m13(a13), m14(a14),
//		  m21(a21), m22(a22), m23(a23), m24(a24),
//		  m31(a31), m32(a32), m33(a33), m34(a34)
//	{
//	}
//
//	TMatrix3x4(const TMatrix3x4<T> &M) : m11(M.m11), m12(M.m12), m13(M.m13), m14(M.m14),
//										 m21(M.m21), m22(M.m22), m23(M.m23), m24(M.m24),
//										 m31(M.m31), m32(M.m32), m33(M.m33), m34(M.m34)
//	{
//	}
//
//	TMatrix3x4(const T M[16]) : m11(M[0]), m12(M[3]), m13(M[6]), m14(M[9]),
//								m21(M[1]), m22(M[4]), m23(M[7]), m24(M[10]),
//								m31(M[2]), m32(M[5]), m33(M[8]), m34(M[11])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix3x4<T> &operator=(const TMatrix3x4<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m13 = M.m13;
//		m14 = M.m14;
//		m21 = M.m21;
//		m22 = M.m22;
//		m23 = M.m23;
//		m24 = M.m24;
//		m31 = M.m31;
//		m32 = M.m32;
//		m33 = M.m33;
//		m34 = M.m34;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix3x4<Other>() const
//	{
//		return TMatrix3x4<Other>(
//			(Other)m11, (Other)m12, (Other)m13, (Other)m14,
//			(Other)m21, (Other)m22, (Other)m23, (Other)m24,
//			(Other)m31, (Other)m32, (Other)m33, (Other)m34);
//	}
//
//	const TMatrix4x3<T> AsTranspose() const
//	{
//		return TMatrix4x3<T>(
//			t11, t21, t31,
//			t12, t22, t32,
//			t13, t23, t33,
//			t14, t24, t34);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, m13,
//			m21, m22, m23,
//			m31, m32, m33);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, m13, m14,
//			m21, m22, m23, m24,
//			m31, m32, m33, m34,
//			0, 0, 0, 0);
//	}
//};
//
//// TMatrix4x2
//template <typename T>
//class TMatrix4x2
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21, m31, m41;
//			T m12, m22, m32, m42;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12;
//			T t21, t22;
//			T t31, t32;
//			T t41, t42;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[8];
//		};
//	};
//
//	TMatrix4x2()
//		: m11(0), m12(0),
//		  m21(0), m22(0),
//		  m31(0), m32(0),
//		  m41(0), m42(0)
//	{
//	}
//
//	TMatrix4x2(
//		const T a11, const T a12,
//		const T a21, const T a22,
//		const T a31, const T a32,
//		const T a41, const T a42)
//		: m11(a11), m12(a12),
//		  m21(a21), m22(a22),
//		  m31(a31), m32(a32),
//		  m41(a41), m42(a42)
//	{
//	}
//
//	TMatrix4x2(const TMatrix4x2<T> &M) : m11(M.m11), m12(M.m12),
//										 m21(M.m21), m22(M.m22),
//										 m31(M.m31), m32(M.m32),
//										 m41(M.m41), m42(M.m42)
//	{
//	}
//
//	TMatrix4x2(const T M[16]) : m11(M[0]), m12(M[4]),
//								m21(M[1]), m22(M[5]),
//								m31(M[2]), m32(M[6]),
//								m41(M[3]), m42(M[7])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix4x2<T> &operator=(const TMatrix4x2<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m21 = M.m21;
//		m22 = M.m22;
//		m31 = M.m31;
//		m32 = M.m32;
//		m41 = M.m41;
//		m42 = M.m42;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix4x2<Other>() const
//	{
//		return TMatrix4x2<Other>(
//			(Other)m11, (Other)m12,
//			(Other)m21, (Other)m22,
//			(Other)m31, (Other)m32,
//			(Other)m41, (Other)m42);
//	}
//
//	const TMatrix2x4<T> AsTranspose() const
//	{
//		return TMatrix2x4<T>(
//			t11, t21, t31, t41,
//			t12, t22, t32, t42);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, 0,
//			m21, m22, 0,
//			m31, m32, 0);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, 0, 0,
//			m21, m22, 0, 0,
//			m31, m32, 0, 0,
//			m41, m42, 0, 0);
//	}
//};
//
//// TMatrix4x3
//template <typename T>
//class TMatrix4x3
//{
//  public:
//	union {
//		// normal access (like the JavaScript library)
//		struct
//		{
//			T m11, m21, m31, m41;
//			T m12, m22, m32, m42;
//			T m13, m23, m33, m43;
//		};
//
//		// transposed access
//		struct
//		{
//			T t11, t12, t13;
//			T t21, t22, t23;
//			T t31, t32, t33;
//			T t41, t42, t43;
//		};
//
//		// array form
//		struct
//		{
//			T ptr[12];
//		};
//	};
//
//	TMatrix4x3()
//		: m11(0), m12(0), m13(0),
//		  m21(0), m22(0), m23(0),
//		  m31(0), m32(0), m33(0),
//		  m41(0), m42(0), m43(0)
//	{
//	}
//
//	TMatrix4x3(
//		const T a11, const T a12, const T a13,
//		const T a21, const T a22, const T a23,
//		const T a31, const T a32, const T a33,
//		const T a41, const T a42, const T a43)
//		: m11(a11), m21(a21), m31(a31), m41(a41),
//		  m12(a12), m22(a22), m32(a32), m42(a42),
//		  m13(a13), m23(a23), m33(a33), m43(a43)
//	{
//	}
//
//	TMatrix4x3(const TMatrix4x3<T> &M) : m11(M.m11), m12(M.m12), m13(M.m13),
//										 m21(M.m21), m22(M.m22), m23(M.m23),
//										 m31(M.m31), m32(M.m32), m33(M.m33),
//										 m41(M.m41), m42(M.m42), m43(M.m43)
//	{
//	}
//
//	TMatrix4x3(const T M[16]) : m11(M[0]), m12(M[4]), m13(M[8]),
//								m21(M[1]), m22(M[5]), m23(M[9]),
//								m31(M[2]), m32(M[6]), m33(M[10]),
//								m41(M[3]), m42(M[7]), m43(M[11])
//	{
//	}
//
//	template <typename Other>
//	const TMatrix4x3<T> &operator=(const TMatrix4x3<Other> &M)
//	{
//		m11 = M.m11;
//		m12 = M.m12;
//		m13 = M.m13;
//		m21 = M.m21;
//		m22 = M.m22;
//		m23 = M.m23;
//		m31 = M.m31;
//		m32 = M.m32;
//		m33 = M.m33;
//		m41 = M.m41;
//		m42 = M.m42;
//		m43 = M.m43;
//		return *this;
//	}
//
//	template <typename Other>
//	operator TMatrix4x3<Other>() const
//	{
//		return TMatrix4x3<Other>(
//			(Other)m11, (Other)m12, (Other)m13,
//			(Other)m21, (Other)m22, (Other)m23,
//			(Other)m31, (Other)m32, (Other)m33,
//			(Other)m41, (Other)m42, (Other)m43);
//	}
//
//	const TMatrix3x4<T> AsTranspose() const
//	{
//		return TMatrix3x4<T>(
//			t11, t21, t31, t41,
//			t12, t22, t32, t42,
//			t13, t23, t33, t43);
//	}
//
//	const TMatrix2<T> AsMatrix2() const
//	{
//		return TMatrix2<T>(
//			m11, m12,
//			m21, m22);
//	}
//
//	const TMatrix3<T> AsMatrix3() const
//	{
//		return TMatrix3<T>(
//			m11, m12, m13,
//			m21, m22, m23,
//			m31, m32, m33);
//	}
//
//	const TMatrix4<T> AsMatrix4() const
//	{
//		return TMatrix4<T>(
//			m11, m12, m13, 0,
//			m21, m22, m23, 0,
//			m31, m32, m33, 0,
//			m41, m42, m43, 0);
//	}
//};

template <typename T>
class TMatrix
{
  public:
    std::vector<T> m;
    int cols = 0;
    int rows = 0;

    TMatrix(int newCols, int newRows) { Resize(newCols, newRows); }
    TMatrix(const TMatrix<T> &M)
    {
        m.resize(M.m.size());
        std::copy(M.m.begin(), M.m.end(), m.begin());
        cols = M.cols;
        rows = M.rows;
    }

    void LoadMatrix(const TMatrix<T> &M)
    {
        m.resize(M.m.size());
        std::copy(M.m.begin(), M.m.end(), m.begin());
        cols = M.cols;
        rows = M.rows;
    }

    void Resize(int newCols, int newRows)
    {
        std::vector<T> newM;
        newM.resize(newCols * newRows);

        // int minCols = min2(newCols, cols);
        // int minRows = min2(newRows, rows);

        for (int i = 0; i < newCols; i++)
        {
            for (int j = 0; j < newRows; j++)
            {
                // don't copy anything that doesn't exist in the new matrix
                if (i >= newCols || j >= newRows)
                    continue;
                int oldIndex = j * cols + i;
                int newIndex = j * newCols + i;
                // if the element doesn't exist in the old matrix, then use 0 as an element
                if (i >= cols || j >= rows)
                    newM[newIndex] = T(0);
                else
                    newM[newIndex] = m[oldIndex];
            }
        }

        m = newM;
        cols = newCols;
        rows = newRows;
    }

    const T GetElement(int col, int row) const
    {
        if (col < 0 || col >= cols || row < 0 || row >= rows)
            return T(0);

        return m[row * cols + col];
    }

    const T &SetElement(int col, int row, const T &value)
    {
        if (col < 0 || col >= cols || row < 0 || row >= rows)
            throw std::out_of_range("TMatrix<T>::SetElement(col, row, value) indices out of range!");

        m[row * cols + col] = value;
        return m[row * cols + col];
    }

    TMatrix<T> Transpose() const
    {
        TMatrix<T> newM(rows, cols);

        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                newM.SetElement(j, i, GetElement(i, j));
            }
        }

        return newM;
    }
};

template <typename T>
constexpr TVector3<T> operator*(const TMatrix3<T> &lhs, const TVector3<T> &rhs)
{
    return TVector3<T>(
        lhs.m11 * rhs.x + lhs.m12 * rhs.y + lhs.m13 * rhs.z,
        lhs.m21 * rhs.x + lhs.m22 * rhs.y + lhs.m23 * rhs.z,
        lhs.m31 * rhs.x + lhs.m32 * rhs.y + lhs.m33 * rhs.z);
}

template <typename T>
constexpr TColor3<T> operator*(const TMatrix3<T> &lhs, const TColor3<T> &rhs)
{
    return TColor3<T>(
        lhs.m11 * rhs.r + lhs.m12 * rhs.g + lhs.m13 * rhs.b,
        lhs.m21 * rhs.r + lhs.m22 * rhs.g + lhs.m23 * rhs.b,
        lhs.m31 * rhs.r + lhs.m32 * rhs.g + lhs.m33 * rhs.b);
}

//using Matrix2x3f = TMatrix2x3<float>;
//using Matrix2x4f = TMatrix2x4<float>;
//using Matrix3x2f = TMatrix3x2<float>;
//using Matrix3x4f = TMatrix3x4<float>;
//using Matrix4x2f = TMatrix4x2<float>;
//using Matrix4x3f = TMatrix4x3<float>;
//
//using Matrix2x3d = TMatrix2x3<double>;
//using Matrix2x4d = TMatrix2x4<double>;
//using Matrix3x2d = TMatrix3x2<double>;
//using Matrix3x4d = TMatrix3x4<double>;
//using Matrix4x2d = TMatrix4x2<double>;
//using Matrix4x3d = TMatrix4x3<double>;
//
//extern template class TMatrix2x3<float>;
//extern template class TMatrix2x4<float>;
//extern template class TMatrix3x2<float>;
//extern template class TMatrix3x4<float>;
//extern template class TMatrix4x2<float>;
//extern template class TMatrix4x3<float>;
//
//extern template class TMatrix2x3<double>;
//extern template class TMatrix2x4<double>;
//extern template class TMatrix3x2<double>;
//extern template class TMatrix3x4<double>;
//extern template class TMatrix4x2<double>;
//extern template class TMatrix4x3<double>;
//
//using Matrixf = TMatrix<float>;
//using Matrixd = TMatrix<double>;
//
//extern template class TMatrix<float>;
//extern template class TMatrix<double>;
} // namespace Fluxions

#endif