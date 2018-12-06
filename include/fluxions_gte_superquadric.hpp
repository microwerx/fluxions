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
#ifndef FLUXIONS_GTE_SUPERQUADRIC_HPP
#define FLUXIONS_GTE_SUPERQUADRIC_HPP


#include <fluxions_gte_vector3.hpp>


namespace Fluxions
{
	template <typename T>
	class SuperQuadricEllipsoid
	{
	public:
		SuperQuadricEllipsoid(T n, T e, T a1 = 1, T a2 = 1, T a3 = 1)
		{
			this->n = fabs(n);
			this->e = fabs(e);
			this->e_over_n = e / n;
			this->_2_over_n = 2 / n;
			this->_2_over_e = 2 / e;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->inva1 = 1 / a1;
			this->inva2 = 1 / a2;
			this->inva3 = 1 / a3;
		}

		TVector3<T> operator()(T u, T v) const {
			return TVector3<T>(
				a1 * c(u, n) * c(v, e),
				a2 * c(u, n) * s(v, e),
				a3 * s(u, n));
		}

		TVector3<T> Normal(T u, T v) const {
			return TVector3<T>(
				inva1 * c(u, 2 - n) * c(v, 2 - e),
				inva2 * c(u, 2 - n) * s(v, 2 - e),
				inva3 * s(u, 2 - n));
		}

		T InsideOutside(T x, T y, T z) const {
			return pow(pow(inva1 * x, _2_over_e) + pow(inva2 * y, _2_over_e), e_over_n) + pow(inva3 * z, _2_over_n);
		}
	private:
		T c(const T x, const T &y) const {
			T cosine = cos(x);
			return sgn(cosine) * pow(abs(cosine), y);
		}
		T s(const T x, const T y) const {
			T sine = sin(x);
			return sgn(sine) * pow(abs(sine), y);
		}
		T cT(const T alpha, const T x, const T y) const {
			return alpha + c(x, y);
		}
		T a1;
		T a2;
		T a3;
		T inva1;
		T inva2;
		T inva3;
		T n;
		T e;
		T e_over_n;
		T _2_over_n;
		T _2_over_e;
	};


	template <typename T>
	class SuperQuadricToroid : public SuperQuadricEllipsoid<T>
	{
	public:
		SuperQuadricToroid(T n, T e, T alpha, T a1 = 1, T a2 = 1, T a3 = 1)
			: SuperQuadricEllipsoid<T>(n, e, a1, a2, a3)
		{
			this->alpha = alpha;
			a4 = alpha / sqrt(a1*a1 + a2*a2);
		}

		TVector3<T> operator()(T u, T v) const {
			return TVector3<T>(
				a1 * (alpha + c(u, n)) * c(v, e),
				a2 * (alpha + c(u, n)) * s(v, e),
				a3 * SuperQuadricEllipsoid::s(u, n));
		}

		TVector3<T> Normal(T u, T v) const {
			return TVector3<T>(
				inva1 * c(u, 2 - n) * c(v, 2 - e),
				inva2 * c(u, 2 - n) * s(v, 2 - e),
				inva3 * s(u, 2 - n));
		}

		T InsideOutside(T x, T y, T z) const {
			return (pow(pow(inva1 * x, _2_over_e) + pow(inva2 * y, _2_over_e), e_over_n) - a4, _2_over_n) + pow(inva3 * z, _2_over_n);
		}
	private:
		T a4;
		T alpha;
	};


	using SuperQuadricEllipsoidf = SuperQuadricEllipsoid<float>;
	using SuperQuadricEllipsoidd = SuperQuadricEllipsoid<double>;
	using SuperQuadricToroidf = SuperQuadricToroid<float>;
	using SuperQuadricToroidd = SuperQuadricToroid<double>;


	extern template class SuperQuadricEllipsoid<float>;
	extern template class SuperQuadricEllipsoid<double>;
	extern template class SuperQuadricToroid<float>;
	extern template class SuperQuadricToroid<double>;
}


#endif

