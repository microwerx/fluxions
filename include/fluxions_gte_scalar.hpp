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
#ifndef FLUXIONS_GTE_SCALAR_HPP
#define FLUXIONS_GTE_SCALAR_HPP


namespace Fluxions
{
	template <typename T1>
	class TScalar
	{
	public:
		union {
			T1 x;
			T1 v[1];
		};

		TScalar()
			: x(0)
		{ }

		TScalar(const T1 x_)
			: x(x_)
		{ }

		TScalar(const TScalar<T1> & value)
			: x(value.x)
		{ }

		// template <typename OtherType> constexpr TScalar<OtherType> OtherType()
		// {
		// 	return TScalar<OtherType>(x);
		// }

		//constexpr TScalar<T1> pow(T1 y)
		//{
		//	return TScalar<T1>(::pow(X, y));
		//}

		//constexpr TScalar<T1> cos()
		//{
		//	return TScalar<T1>(cos(X));
		//}

		//constexpr TScalar<T1> sin()
		//{
		//	return TScalar<T1>(sin(X));
		//}

		//constexpr TScalar<T1> tan()
		//{
		//	return TScalar<T1>(tan(X));
		//}

		//constexpr TScalar<T1> sqrt()
		//{
		//	return TScalar<T1>(sqrt(X));
		//}

		// constexpr enable_if_t<is_arithmetic_v<T1>, TScalar<T1>> abs()
		// {
		// 	return TScalar<T1>(x < 0 ? -x : x);
		// }

		// constexpr enable_if_t<is_arithmetic_v<T1>, TScalar<T1>> sgn()
		// {
		// 	return TScalar<T1>(x < 0 ? T1(-1) : x > 0 ? T1(1) : 0);
		// }

		// constexpr enable_if_t<is_arithmetic_v<T1>, TScalar<T1>> operator - () noexcept
		// {
		// 	return TScalar<T1>(-x);
		// }
	};


	using Scalarf = TScalar<float>;
	using Scalard = TScalar<double>;

	extern template class TScalar<float>;
	extern template class TScalar<double>;
}


#endif
