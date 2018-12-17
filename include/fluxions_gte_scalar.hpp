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
template <typename T>
class TScalar
{
  public:
	union {
		T x;
		T v[1];
	};

	TScalar()
		: x(0)
	{
	}

	TScalar(const T x_)
		: x(x_)
	{
	}

	TScalar(const TScalar<T> &value)
		: x(value.x)
	{
	}

	// template <typename OtherType> constexpr TScalar<OtherType> OtherType()
	// {
	// 	return TScalar<OtherType>(x);
	// }

	//constexpr TScalar<T> pow(T y)
	//{
	//	return TScalar<T>(::pow(X, y));
	//}

	//constexpr TScalar<T> cos()
	//{
	//	return TScalar<T>(cos(X));
	//}

	//constexpr TScalar<T> sin()
	//{
	//	return TScalar<T>(sin(X));
	//}

	//constexpr TScalar<T> tan()
	//{
	//	return TScalar<T>(tan(X));
	//}

	//constexpr TScalar<T> sqrt()
	//{
	//	return TScalar<T>(sqrt(X));
	//}

	// constexpr enable_if_t<is_arithmetic_v<T>, TScalar<T>> abs()
	// {
	// 	return TScalar<T>(x < 0 ? -x : x);
	// }

	// constexpr enable_if_t<is_arithmetic_v<T>, TScalar<T>> sgn()
	// {
	// 	return TScalar<T>(x < 0 ? T(-1) : x > 0 ? T(1) : 0);
	// }

	// constexpr enable_if_t<is_arithmetic_v<T>, TScalar<T>> operator - () noexcept
	// {
	// 	return TScalar<T>(-x);
	// }
};

using Scalarf = TScalar<float>;
using Scalard = TScalar<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TScalar<float>;
extern template class TScalar<double>;
#endif
} // namespace Fluxions

#endif
