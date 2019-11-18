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
#ifndef FLUXIONS_MATRIX2_HPP
#define FLUXIONS_MATRIX2_HPP

#include <fluxions_gte_scalar_math.hpp>

namespace Fluxions
{

	template <typename T>
	class TMatrix2
	{
	public:
		T m11, m21;
		T m12, m22;

		constexpr T* ptr() noexcept { return &m11; }
		constexpr const T* const_ptr() const noexcept { return &m11; }

		using type = T;

		static constexpr size_t size() noexcept { return 4; }
		static constexpr size_t numcols() noexcept { return 2; }
		static constexpr size_t numrows() noexcept { return 2; }

		constexpr TMatrix2() noexcept {
			m11 = 1;
			m12 = 0;
			m21 = 0;
			m22 = 1;
		}

		constexpr TMatrix2(
			T a11, T a12,
			T a21, T a22) noexcept {
			m11 = a11;
			m12 = a12;
			m21 = a21;
			m22 = a22;
		}

		constexpr TMatrix2(const T M[2][2]) noexcept {
			m11 = M[0][0];
			m12 = M[0][1];
			m21 = M[1][0];
			m22 = M[1][1];
		}

		constexpr TMatrix2(const T M[4]) noexcept {
			T* m = &m11;
			m[0] = M[0];
			m[1] = M[1];
			m[2] = M[2];
			m[3] = M[3];
		}

		constexpr TMatrix2(const TMatrix2<T>& M) noexcept {
			m11 = M.m11;
			m12 = M.m12;
			m21 = M.m21;
			m22 = M.m22;
		}

		constexpr TMatrix2(TMatrix2<T>&& M) noexcept {
			m11 = std::move(M.m11);
			m12 = std::move(M.m12);
			m21 = std::move(M.m21);
			m22 = std::move(M.m22);
		}

		constexpr TMatrix2<T>& operator=(const TMatrix2<T>& M) noexcept {
			m11 = M.m11;
			m12 = M.m12;
			m21 = M.m21;
			m22 = M.m22;
			return *this;
		}

		constexpr TMatrix2<T>& operator=(TMatrix2<T>&& M) noexcept {
			m11 = std::move(M.m11);
			m12 = std::move(M.m12);
			m21 = std::move(M.m21);
			m22 = std::move(M.m22);
			return *this;
		}

		template <typename U>
		operator TMatrix2<U>() const noexcept {
			return TMatrix2<U>(
				static_cast<U>(m11), static_cast<U>(m12),
				static_cast<U>(m21), static_cast<U>(m22));
		}

		constexpr TMatrix2<T>& LoadIdentity() noexcept {
			return *this = MakeIdentity();
		}

		constexpr TMatrix2<T>& LoadZero() noexcept {
			return *this = MakeZero();
		}

		constexpr TMatrix2<T>& MultMatrix(const TMatrix2<T>& M) noexcept {
			return *this = (TMatrix2<T>)multiply(*this, M);
		}

		constexpr TMatrix2<T>& operator*=(const TMatrix2<T>& M) noexcept {
			return *this = (TMatrix2<T>)multiply(*this, M);
		}

		constexpr TMatrix2<T>& operator+=(const TMatrix2<T>& M) noexcept {
			return *this = TMatrix2<T>(
				m11 + M.m11, m12 + M.m12,
				m21 + M.m21, m22 + M.m22);
		}

		constexpr TMatrix2<T>& operator-=(const TMatrix2<T>& M) noexcept {
			return *this = TMatrix2<T>(
				m11 - M.m11, m12 - M.m12,
				m21 - M.m21, m22 - M.m22);
		}

		template <typename U>
		constexpr TMatrix2<T>& operator+=(const U c) noexcept {
			return *this = TMatrix2<T>(
				m11 + c, m12 + c,
				m21 + c, m22 + c);
		}

		template <typename U>
		constexpr TMatrix2<T>& operator-=(const U c) noexcept {
			return *this = TMatrix2<T>(
				m11 - c, m12 - c,
				m21 - c, m22 - c);
		}

		template <typename U>
		constexpr TMatrix2<T>& operator*=(const U c) noexcept {
			return *this = TMatrix2<T>(
				m11 * c, m12 * c,
				m21 * c, m22 * c);
		}

		template <typename U>
		constexpr TMatrix2<T>& operator/=(const U c) noexcept {
			return *this = TMatrix2<T>(
				m11 / c, m12 / c,
				m21 / c, m22 / c);
		}

		constexpr T Determinant() const noexcept {
			return m11 * m22 - m21 * m12;
		}

		constexpr bool IsInvertible() const noexcept {
			return Determinant() != 0;
		}

		constexpr auto AsInverse() const noexcept {
			T det = Determinant();
			if (det == 0)
				return MakeZero();
			T invDet = (T)(1.0 / det);
			return TMatrix2<T>(
				m22 * invDet, -m21 * invDet,
				-m12 * invDet, m11 * invDet);
		}

		constexpr auto AsAdjugate() const noexcept {
			return TMatrix2<T>(
				m22, -m21,
				-m12, m11);
		}

		constexpr auto AsTranspose() const noexcept {
			return TMatrix2<T>(
				m11, m21,
				m12, m22);
		}

		constexpr auto Transpose() noexcept {
			return *this = AsTranspose();
		}

		constexpr auto Invert() noexcept {
			return *this = AsInverse();
		}

		template <typename U>
		constexpr auto Rotate(T angleInDegrees) noexcept {
			return *this = MakeRotation((T)angleInDegrees);
		}

		template <typename U>
		constexpr auto Scale(const U sx, const U sy) noexcept {
			return *this = MakeScaling((T)sx, (T)sy);
		}

		static constexpr auto MakeIdentity() {
			return TMatrix2<T>(
				1, 0,
				0, 1);
		}

		static constexpr auto MakeZero() {
			return TMatrix2<T>(
				0, 0,
				0, 0);
		}

		static constexpr auto MakeRotation(T angleInDegrees) {
			T angle = static_cast<T>(angleInDegrees * FX_DEGREES_TO_RADIANS);

			T c = cos(angle);
			T s = sin(angle);

			return TMatrix2<T>(
				c, -s,
				s, c);
		}

		static constexpr auto MakeScaling(T sx, T sy) noexcept {
			return TMatrix2<T>(
				sx, 0,
				0, sy);
		}

		template <typename U>
		static constexpr auto multiply(const TMatrix2<T>& m1, const TMatrix2<U> m2) noexcept {
			return TMatrix2<std::common_type_t<T, U>>(
				m1.m11 * m2.m11 + m1.m12 * m2.m21,
				m1.m11 * m2.m12 + m1.m12 * m2.m22,
				m1.m21 * m2.m11 + m1.m22 * m2.m21,
				m1.m21 * m2.m12 + m1.m22 * m2.m22);
		}
	};

	using Matrix2f = TMatrix2<float>;
	using Matrix2d = TMatrix2<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TMatrix2<float>;
	extern template class TMatrix2<double>;
#endif
} // namespace Fluxions

#endif
