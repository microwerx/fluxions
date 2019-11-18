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
#ifndef FLUXIONS_GTE_COLOR_MATH_HPP
#define FLUXIONS_GTE_COLOR_MATH_HPP

#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>

namespace Fluxions
{
	template <typename T>
	constexpr TColor3<float> ToColor3f(TColor3<T>& src) noexcept {
		return TColor3<float>(
			(float)src.r * TColor3<T>::to_float_factor,
			(float)src.g * TColor3<T>::to_float_factor,
			(float)src.b * TColor3<T>::to_float_factor);
	}

	template <typename T>
	constexpr TColor3<float> ToColor3f(TColor4<T>& src) noexcept {
		return TColor3<float>(
			(float)src.r * TColor4<T>::to_float_factor,
			(float)src.g * TColor4<T>::to_float_factor,
			(float)src.b * TColor4<T>::to_float_factor);
	}

	template <typename T>
	constexpr void FromColor3f(TColor3<T>& dst, const TColor3<float>& src) {
		dst.r = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.r);
		dst.g = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.g);
		dst.b = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.b);
	}

	template <typename T>
	constexpr void FromColor3f(TColor4<T>& dst, const TColor3<float>& src) {
		dst.r = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.r);
		dst.g = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.g);
		dst.b = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.b);
	}

	// MATH ROUTINES

	template <typename T>
	constexpr T maxof(const TColor3<T>& c) noexcept {
		return c.maxrgb();
	}

	template <typename T>
	constexpr T maxof(const TColor4<T>& c) noexcept {
		return c.maxrgb();
	}

	template<typename T>
	constexpr T color_max_value() {
		bool is_int = std::is_integral_v<T>;
		bool is_flt = std::is_floating_point_v<T>;
		if (is_int && sizeof(T) == 1)
			return T(255);
		else if (is_int && sizeof(T) >= 2)
			return T(65535);
		else if (is_flt)
			return T(1);
		else
			return T(0);
	}

	template<typename T>
	constexpr float color_to_float_factor() {
		bool is_int = std::is_integral_v<T>;
		bool is_flt = std::is_floating_point_v<T>;
		if (is_int && sizeof(T) == 1)
			return float(1.0/255.0);
		else if (is_int && sizeof(T) >= 2)
			return float(1.0/65535.0);
		else if (is_flt)
			return 1.0f;
		else
			return 0.0f;
	}
} // namespace Fluxions
#endif