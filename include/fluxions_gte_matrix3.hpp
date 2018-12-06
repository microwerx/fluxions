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
#ifndef FLUXIONS_MATRIX3_HPP
#define FLUXIONS_MATRIX3_HPP

#include <fluxions_gte_math.hpp>

namespace Fluxions
{
template <typename T1>
class TMatrix3
{
  public:
	union {
		// normal access (column major)
		struct
		{
			T1 m11, m21, m31;
			T1 m12, m22, m32;
			T1 m13, m23, m33;
		};

		// transposed access
		struct
		{
			T1 t11, t12, t13;
			T1 t21, t22, t23;
			T1 t31, t32, t33;
		};

		// 2D array form
		T1 mm[3][3];

		// array form
		T1 m[9];
		T1 v[9];
	};

	TMatrix3();
	// a[row][col], 1-indexed ex: a32 = row 3, col 2, a11 = row 1, col 1 (no 0 row or col)
	TMatrix3(
		const T1 a11, const T1 a12, const T1 a13,
		const T1 a21, const T1 a22, const T1 a23,
		const T1 a31, const T1 a32, const T1 a33);
	TMatrix3(const TMatrix3<T1> &M);
	TMatrix3(const T1 M[3][3]);
	TMatrix3(const T1 M[9]);
	const TMatrix3<T1> &operator=(const TMatrix3<T1> &M);

	template <typename T2>
	operator TMatrix3<T2>() const;

	void LoadIdentity();
	void MultMatrix(const TMatrix3<T1> &M);
	template <typename T2>
	const TMatrix3<T1> &operator*=(const TMatrix3<T2> &M);
	const TMatrix3<T1> &operator+=(const TMatrix3<T1> &M);
	const TMatrix3<T1> &operator-=(const TMatrix3<T1> &M);

	template <typename T2>
	const TMatrix3<T1> operator*(const TMatrix3<T2> &M) const;
	const TMatrix3<T1> operator+(const TMatrix3<T1> &M) const;
	const TMatrix3<T1> operator-(const TMatrix3<T1> &M) const;

	const TMatrix3<T1> &scale(double x);

	const TMatrix3<T1> &Rotate(double angleInDegrees);
	const TMatrix3<T1> &Rotate(double angleInDegrees, double x, double y, double z);
	const TMatrix3<T1> &Scale(double x, double y);

	const T1 Determinant() const;
	const T1 Trace() const;
	bool Invert();
	void Transpose();
	const TMatrix3<T1> AsTranspose() const;
	void Adjugate();
};

//////////////////////////////////////////////////////////////////
// I M P L E M E N T2 A T2 I O N ///////////////////////////////////
//////////////////////////////////////////////////////////////////

template <typename T1>
TMatrix3<T1>::TMatrix3()
{
	m11 = 1;
	m12 = 0;
	m13 = 0;
	m21 = 0;
	m22 = 1;
	m23 = 0;
	m31 = 0;
	m32 = 0;
	m33 = 1;
}

template <typename T1>
TMatrix3<T1>::TMatrix3(
	const T1 a11, const T1 a12, const T1 a13,
	const T1 a21, const T1 a22, const T1 a23,
	const T1 a31, const T1 a32, const T1 a33)
{
	m11 = a11;
	m12 = a12;
	m13 = a13;
	m21 = a21;
	m22 = a22;
	m23 = a23;
	m31 = a31;
	m32 = a32;
	m33 = a33;
}

template <typename T1>
TMatrix3<T1>::TMatrix3(const TMatrix3<T1> &M)
{
	m11 = M.m11;
	m12 = M.m12;
	m13 = M.m13;
	m21 = M.m21;
	m22 = M.m22;
	m23 = M.m23;
	m31 = M.m31;
	m32 = M.m32;
	m33 = M.m33;
}

template <typename T1>
TMatrix3<T1>::TMatrix3(const T1 M[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mm[i][j] = M[j][i];
		}
	}
}

template <typename T1>
TMatrix3<T1>::TMatrix3(const T1 M[9])
{
	for (int i = 0; i < 9; i++)
	{
		m[i] = M[i];
	}
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::operator=(const TMatrix3<T1> &M)
{
	m11 = M.m11;
	m12 = M.m12;
	m13 = M.m13;
	m21 = M.m21;
	m22 = M.m22;
	m23 = M.m23;
	m31 = M.m31;
	m32 = M.m32;
	m33 = M.m33;
	return *this;
}

template <typename T1>
template <typename T2>
TMatrix3<T1>::operator TMatrix3<T2>() const
{
	return TMatrix3<T1>(
		(T1)m11, (T1)m12, (T1)m13,
		(T1)m21, (T1)m22, (T1)m23,
		(T1)m31, (T1)m32, (T1)m33);
}

template <typename T1>
void TMatrix3<T1>::LoadIdentity()
{
	m11 = 1;
	m12 = 0;
	m13 = 0;
	m21 = 0;
	m22 = 1;
	m23 = 0;
	m31 = 0;
	m32 = 0;
	m33 = 1;
}

template <typename T1>
void TMatrix3<T1>::MultMatrix(const TMatrix3<T1> &M)
{
	TMatrix3<T1> u;
	u.m11 = m11 * M.m11 + m12 * M.m21 + m13 * M.m31;
	u.m12 = m11 * M.m12 + m12 * M.m22 + m13 * M.m32;
	u.m13 = m11 * M.m13 + m12 * M.m23 + m13 * M.m33;
	u.m21 = m21 * M.m11 + m22 * M.m21 + m23 * M.m31;
	u.m22 = m21 * M.m12 + m22 * M.m22 + m23 * M.m32;
	u.m23 = m21 * M.m13 + m22 * M.m23 + m23 * M.m33;
	u.m31 = m31 * M.m11 + m32 * M.m21 + m33 * M.m31;
	u.m32 = m31 * M.m12 + m32 * M.m22 + m33 * M.m32;
	u.m33 = m31 * M.m13 + m32 * M.m23 + m33 * M.m33;
	*this = u;
}

template <typename T1>
template <typename T2>
const TMatrix3<T1> &TMatrix3<T1>::operator*=(const TMatrix3<T2> &M)
{
	return *this = TMatrix3<T1>(
			   (T1)(m11 * M.m11 + m12 * M.m21 + m13 * M.m31),
			   (T1)(m11 * M.m12 + m12 * M.m22 + m13 * M.m32),
			   (T1)(m11 * M.m13 + m12 * M.m23 + m13 * M.m33),
			   (T1)(m21 * M.m11 + m22 * M.m21 + m23 * M.m31),
			   (T1)(m21 * M.m12 + m22 * M.m22 + m23 * M.m32),
			   (T1)(m21 * M.m13 + m22 * M.m23 + m23 * M.m33),
			   (T1)(m31 * M.m11 + m32 * M.m21 + m33 * M.m31),
			   (T1)(m31 * M.m12 + m32 * M.m22 + m33 * M.m32),
			   (T1)(m31 * M.m13 + m32 * M.m23 + m33 * M.m33));
}

template <typename T1>
template <typename T2>
const TMatrix3<T1> TMatrix3<T1>::operator*(const TMatrix3<T2> &M) const
{
	return TMatrix3<T1>(
		m11 * M.m11 + m12 * M.m21 + m13 * M.m31,
		m11 * M.m12 + m12 * M.m22 + m13 * M.m32,
		m11 * M.m13 + m12 * M.m23 + m13 * M.m33,
		m21 * M.m11 + m22 * M.m21 + m23 * M.m31,
		m21 * M.m12 + m22 * M.m22 + m23 * M.m32,
		m21 * M.m13 + m22 * M.m23 + m23 * M.m33,
		m31 * M.m11 + m32 * M.m21 + m33 * M.m31,
		m31 * M.m12 + m32 * M.m22 + m33 * M.m32,
		m31 * M.m13 + m32 * M.m23 + m33 * M.m33);
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::operator+=(const TMatrix3<T1> &M)
{
	m11 += M.m11;
	m12 += M.m12;
	m13 += M.m13;
	m21 += M.m21;
	m22 += M.m22;
	m23 += M.m23;
	m31 += M.m31;
	m32 += M.m32;
	m33 += M.m33;
	return *this;
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::operator-=(const TMatrix3<T1> &M)
{
	m11 -= M.m11;
	m21 -= M.m21;
	m31 -= M.m31;
	m12 -= M.m12;
	m22 -= M.m22;
	m32 -= M.m32;
	m13 -= M.m13;
	m23 -= M.m23;
	m33 -= M.m33;
	return *this;
}

template <typename T1>
const TMatrix3<T1> TMatrix3<T1>::operator+(const TMatrix3<T1> &M) const
{
	return TMatrix3<T1>(
		m11 + M.m11, m12 + M.m12, m13 + M.m13,
		m21 + M.m21, m22 + M.m22, m23 + M.m23,
		m31 + M.m31, m32 + M.m32, m33 + M.m33);
}

template <typename T1>
const TMatrix3<T1> TMatrix3<T1>::operator-(const TMatrix3<T1> &M) const
{
	return TMatrix3<T1>(
		m11 - M.m11, m12 - M.m12, m13 - M.m13,
		m21 - M.m21, m22 - M.m22, m23 - M.m23,
		m31 - M.m31, m32 - M.m32, m33 - M.m33);
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::scale(double x)
{
	m11 *= (T1)x;
	m12 *= (T1)x;
	m13 *= (T1)x;
	m21 *= (T1)x;
	m22 *= (T1)x;
	m23 *= (T1)x;
	m31 *= (T1)x;
	m32 *= (T1)x;
	m33 *= (T1)x;
	return *this;
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::Rotate(double angleInDegrees)
{
	double c = cos(angleInDegrees * FX_DEGREES_TO_RADIANS);
	double s = sin(angleInDegrees * FX_DEGREES_TO_RADIANS);

	return *this *= TMatrix3<double>(
			   c, -s, 0.0,
			   s, c, 0.0,
			   0.0, 0.0, 1.0);
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::Rotate(double angleInDegrees, double x, double y, double z)
{
	double c = cos(angleInDegrees * FX_DEGREES_TO_RADIANS);
	double s = sin(angleInDegrees * FX_DEGREES_TO_RADIANS);
	double invLength = 1.0 / sqrt(x * x + y * y + z * z);
	x *= invLength;
	y *= invLength;
	z *= invLength;

	return *this *= TMatrix3<double>(
			   x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s,
			   y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s,
			   x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c);
}

template <typename T1>
const TMatrix3<T1> &TMatrix3<T1>::Scale(double x, double y)
{
	return *this *= TMatrix3<double>(
			   x, 0.0, 0.0,
			   0.0, y, 0.0,
			   0.0, 0.0, 1.0);
}

template <typename T1>
void TMatrix3<T1>::Transpose()
{
	*this = AsTranspose();
}

template <typename T1>
const TMatrix3<T1> TMatrix3<T1>::AsTranspose() const
{
	TMatrix3<T1> u = *this;

	return TMatrix3<T1>(
		t11, t12, t13,
		t21, t22, t23,
		t31, t32, t33);
}

template <typename T1>
const T1 TMatrix3<T1>::Determinant() const
{
	T1 det;

	det = 0;
	det += m11 * (m22 * m33 - m32 * m23);
	det -= m21 * (m12 * m33 - m32 * m13);
	det += m31 * (m12 * m23 - m22 * m13);

	return det;
}

template <typename T1>
const T1 TMatrix3<T1>::Trace() const
{
	return m11 + m22 + m33;
}

template <typename T1>
void TMatrix3<T1>::Adjugate()
{
	T1 c00, c10, c20;
	T1 c01, c11, c21;
	T1 c02, c12, c22;

	c00 = m22 * m33 - m23 * m32;
	c10 = m13 * m32 - m12 * m33;
	c20 = m12 * m23 - m13 * m22;
	c01 = m23 * m31 - m21 * m33;
	c11 = m11 * m33 - m13 * m31;
	c21 = m13 * m21 - m11 * m23;
	c02 = m21 * m32 - m22 * m31;
	c12 = m12 * m31 - m11 * m32;
	c22 = m11 * m22 - m12 * m21;

	m11 = c00;
	m21 = c10;
	m31 = c20;

	m12 = c01;
	m22 = c11;
	m32 = c21;

	m13 = c02;
	m23 = c12;
	m33 = c22;
}

template <typename T1>
bool TMatrix3<T1>::Invert()
{
	T1 det;

	det = Determinant();
	if (det == 0)
		return false;

	double invDet = 1.0 / det;

	Adjugate();
	this->scale(invDet);

	return true;
}

using Matrix3f = TMatrix3<float>;
using Matrix3d = TMatrix3<double>;
extern template class TMatrix3<float>;
extern template class TMatrix3<double>;
} // namespace Fluxions

#endif
