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
#ifndef FLUXIONS_GTE_COLOR4_HPP
#define FLUXIONS_GTE_COLOR4_HPP

#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{
	template <typename T>
	class TColor3;
	template <typename T>
	class TVector3;
	template <typename T>
	class TVector4;

	template <typename T>
	class TColor4 : TCommonContainer<T>
	{
	public:
		T r, g, b, a;

		using value_type = TCommonContainer<T>::value_type;

		static float to_float_factor;
		static float from_float_factor;

		static value_type min_value;
		static value_type max_value;

		static unsigned int gl_type; // UNSIGNED_BYTE, FLOAT, etc.
		static unsigned int gl_size; // 3 for RGB, 4 for RGBA

		using iterator = TCommonIterator<T>;
		using const_iterator = TCommonIterator<const T>;

		constexpr T* ptr() noexcept { return &r; }
		constexpr const T* const_ptr() const noexcept { return &r; }

		constexpr T& operator[](unsigned index) noexcept { return (&r)[index]; }
		constexpr const T& operator[](unsigned index) const noexcept { return (&r)[index]; }

		constexpr unsigned size() const noexcept { return 4; }
		iterator begin() noexcept { return iterator(ptr()); }
		iterator end() noexcept { return iterator(ptr() + size()); }
		const_iterator cbegin() const noexcept { return const_iterator(const_ptr()); }
		const_iterator cend() const noexcept { return const_iterator(const_ptr() + size()); }

		constexpr TColor4()
			: r(0), g(0), b(0), a(0)
		{
		}

		constexpr TColor4(const T value)
			: r(value), g(value), b(value), a(value)
		{
		}

		constexpr TColor4(const T red, const T green, const T blue, const T alpha)
			: r(red), g(green), b(blue), a(alpha)
		{
		}

		constexpr TColor4(const T red, const T green, const T blue)
			: r(red), g(green), b(blue), a(0) {}

		constexpr TColor4(const TColor4<T>& color)
			: r(color.r), g(color.g), b(color.b), a(color.a)
		{
		}

		constexpr TColor4(const TColor3<T>& color, const T alpha = T(1))
			: r(color.r), g(color.g), b(color.b), a(alpha)
		{
		}

		const TColor4<T>& reset(const T value)
		{
			r = g = b = a = value;
			return *this;
		}

		TColor4<T>& reset(const T red, const T green, const T blue) {
			r = red;
			g = green;
			b = blue;
			a = 0;
			return *this;
		}

		TColor4<T>& reset(const T red, const T green, const T blue, const T alpha)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
			return *this;
		}

		template <typename OtherType>
		constexpr operator TColor4<OtherType>() const noexcept
		{
			return TColor4<OtherType>((OtherType)r, (OtherType)g, (OtherType)b, (OtherType)a);
		}

		template <typename OtherType>
		constexpr operator TColor3<OtherType>() const noexcept
		{
			return TColor3<OtherType>((OtherType)r, (OtherType)g, (OtherType)b);
		}

		template <typename OtherType>
		const TColor4<T>& operator=(const TColor3<OtherType>& color)
		{
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator=(const TColor4<OtherType>& color)
		{
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			a = (T)color.a;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator=(const TVector3<OtherType>& color)
		{
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator=(const TVector4<OtherType>& color)
		{
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			a = (T)color.a;
			return *this;
		}

		const TColor4<T>& operator=(const T value)
		{
			r = value;
			g = value;
			b = value;
			a = value;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator+=(const TColor4<OtherType>& color)
		{
			r += (T)color.r;
			g += (T)color.g;
			b += (T)color.b;
			a += (T)color.a;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator-=(const TColor4<OtherType>& color)
		{
			r -= (T)color.r;
			g -= (T)color.g;
			b -= (T)color.b;
			a -= (T)color.a;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator*=(const TColor4<OtherType>& color)
		{
			r *= (T)color.r;
			g *= (T)color.g;
			b *= (T)color.b;
			a *= (T)color.a;
			return *this;
		}

		template <typename OtherType>
		const TColor4<T>& operator/=(const TColor4<OtherType>& color)
		{
			r /= (T)color.r;
			g /= (T)color.g;
			b /= (T)color.b;
			a /= (T)color.a;
			return *this;
		}

		const TColor4<T>& operator+=(const T value)
		{
			r += value;
			g += value;
			b += value;
			a += value;
			return *this;
		}

		const TColor4<T>& operator-=(const T value)
		{
			r -= value;
			g -= value;
			b -= value;
			a -= value;
			return *this;
		}

		const TColor4<T>& operator*=(const T value)
		{
			r *= value;
			g *= value;
			b *= value;
			a *= value;
			return *this;
		}

		const TColor4<T>& operator/=(const T value)
		{
			r /= value;
			g /= value;
			b /= value;
			a /= value;
			return *this;
		}

		template <typename OtherType>
		constexpr TColor4<T> operator+(const TColor4<OtherType>& color)
		{
			return TColor4<T>(
				r + color.r,
				g + color.g,
				b + color.b,
				a + color.a);
		}

		template <typename OtherType>
		constexpr TColor4<T> operator-(const TColor4<OtherType>& color)
		{
			return TColor4<T>(
				r - color.r,
				g - color.g,
				b - color.b,
				a - color.a);
		}

		template <typename OtherType>
		constexpr TColor4<T> operator*(const TColor4<OtherType>& color)
		{
			return TColor4<T>(
				r * color.r,
				g * color.g,
				b * color.b,
				a * color.a);
		}

		template <typename OtherType>
		constexpr TColor4<T> operator/(const TColor4<OtherType>& color)
		{
			return TColor4<T>(
				r / color.r,
				g / color.g,
				b / color.b,
				a / color.a);
		}

		template <typename OtherType>
		constexpr TColor4<T> pow(const OtherType power)
		{
			return TColor4<T>(
				std::pow(r, power),
				std::pow(g, power),
				std::pow(b, power),
				std::pow(a, power));
		}

		template <typename OtherType>
		constexpr TColor4<OtherType> to_other_type(const OtherType min_value, const OtherType max_value)
		{
			return TColor4<OtherType>(
				remap_value_min_max<T, OtherType>(r, min_value, max_value),
				remap_value_min_max<T, OtherType>(g, min_value, max_value),
				remap_value_min_max<T, OtherType>(b, min_value, max_value),
				remap_value_min_max<T, OtherType>(a, min_value, max_value));
		}

		template <typename OtherType>
		constexpr TColor4<OtherType> to_other_type(const OtherType min1, const OtherType max1, const OtherType min2, const OtherType max2)
		{
			return TColor4<OtherType>(
				remap_value_min_max<T, OtherType>(r, min1, max1, min2, max2),
				remap_value_min_max<T, OtherType>(g, min1, max1, min2, max2),
				remap_value_min_max<T, OtherType>(b, min1, max1, min2, max2),
				remap_value_min_max<T, OtherType>(a, min1, max1, min2, max2));
		}

		template <typename OtherType>
		constexpr TColor4<OtherType> Remap(const T src_min_value, const T src_max_value, const OtherType dst_min_value, const OtherType dst_max_value)
		{
			return TColor4<OtherType>(
				remap_value_min_max<T, OtherType>(r, src_min_value, src_max_value, dst_min_value, dst_max_value),
				remap_value_min_max<T, OtherType>(g, src_min_value, src_max_value, dst_min_value, dst_max_value),
				remap_value_min_max<T, OtherType>(b, src_min_value, src_max_value, dst_min_value, dst_max_value),
				remap_value_min_max<T, OtherType>(a, src_min_value, src_max_value, dst_min_value, dst_max_value));
		}

		constexpr TColor4<T> Clamp(T minValue, T maxValue) const
		{
			return TColor4<T>(
				Fluxions::clamp<T>(r, minValue, maxValue),
				Fluxions::clamp<T>(g, minValue, maxValue),
				Fluxions::clamp<T>(b, minValue, maxValue),
				Fluxions::clamp<T>(a, minValue, maxValue));
		}

		constexpr TColor4<T> ScaleClamp(T scale, T minValue, T maxValue) const
		{
			return TColor4<T>(
				Fluxions::clamp<T>(r * scale, minValue, maxValue),
				Fluxions::clamp<T>(g * scale, minValue, maxValue),
				Fluxions::clamp<T>(b * scale, minValue, maxValue),
				Fluxions::clamp<T>(a * scale, minValue, maxValue));
		}

		constexpr TColor4<T>& clamp() noexcept
		{
			r = Fluxions::clamp(r, min_value, max_value);
			g = Fluxions::clamp(r, min_value, max_value);
			b = Fluxions::clamp(r, min_value, max_value);
			return *this;
		}

		template <typename OtherType, typename ScaleType>
		constexpr TColor3<OtherType> ToColor3(const ScaleType scale, const OtherType min_value, const OtherType max_value) const
		{
			return TColor3<OtherType>(
				(OtherType)Fluxions::clamp<OtherType>(OtherType(r * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(g * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(b * scale), min_value, max_value));
		}

		template <typename OtherType, typename ScaleType>
		constexpr TColor4<OtherType> ToColor4(const ScaleType scale, const OtherType min_value, const OtherType max_value) const
		{
			return TColor4<OtherType>(
				(OtherType)Fluxions::clamp<OtherType>(OtherType(r * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(g * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(b * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(a * scale), min_value, max_value));
		}

		template <typename OtherType = T>
		constexpr TColor3<OtherType> ToColor3() const
		{
			return TColor3<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b);
		}

		template <typename OtherType = T>
		constexpr TColor3<OtherType> ToColor4() const
		{
			return TColor3<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b,
				(OtherType)a);
		}

		template <typename OtherType = T>
		constexpr TVector3<OtherType> ToVector3() const
		{
			return TVector3<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b);
		}

		template <typename OtherType = T>
		constexpr TVector4<OtherType> ToVector4() const
		{
			return TVector4<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b,
				(OtherType)a);
		}

		constexpr double Intensity() const
		{
			return (r + g + b) / 3.0;
		}

		constexpr T minrgb() const noexcept
		{
			return min3<T>(r, g, b);
		}

		constexpr T maxrgb() const noexcept
		{
			return max3<T>(r, g, b);
		}

		//static constexpr TColor4<T> min(const TColor4<T>& a, const TColor4<T>& b) noexcept
		//{
		//    return TColor4<T>(
		//        std::min(a.r, b.r),
		//        std::min(a.g, b.g),
		//        std::min(a.b, b.b),
		//        std::min(a.a, b.a));
		//}

		//static constexpr TColor4<T> max(const TColor4<T>& a, const TColor4<T>& b) noexcept
		//{
		//    return TColor4<T>(
		//        std::max(a.r, b.r),
		//        std::max(a.g, b.g),
		//        std::max(a.b, b.b),
		//        std::max(a.a, b.a));
		//}
	};

	template <typename T>
	constexpr TColor4<T> abs(TColor4<T> color)
	{
		return TColor4<T>(
			color.r < 0 ? (T)-1 : color.r > 0 ? 1 : 0,
			color.g < 0 ? (T)-1 : color.g > 0 ? 1 : 0,
			color.b < 0 ? (T)-1 : color.b > 0 ? 1 : 0,
			color.a < 0 ? (T)-1 : color.a > 0 ? 1 : 0);
	}

	template <typename T>
	constexpr TColor4<T> sign(TColor4<T> color)
	{
		return TColor4<T>(
			color.r < 0 ? (T)-1 : color.r > 0 ? 1 : 0,
			color.g < 0 ? (T)-1 : color.g > 0 ? 1 : 0,
			color.b < 0 ? (T)-1 : color.b > 0 ? 1 : 0,
			color.a < 0 ? (T)-1 : color.a > 0 ? 1 : 0);
	}

	template <typename T>
	constexpr TColor4<T> negate(TColor4<T> color)
	{
		return TColor4<T>(-color.r, -color.g, -color.b, -color.a);
	}

	template <typename Floating, typename T>
	constexpr TColor4<T> operator*(const TColor4<T>& lhs, const Floating rhs)
	{
		return TColor4<T>(
			(T)(lhs.r * rhs),
			(T)(lhs.g * rhs),
			(T)(lhs.b * rhs),
			(T)(lhs.a * rhs));
	}

	template <typename Floating, typename T>
	constexpr TColor4<T> operator*(const Floating rhs, const TColor4<T>& lhs)
	{
		return TColor4<T>(
			(T)(lhs * rhs.r),
			(T)(lhs * rhs.g),
			(T)(lhs * rhs.b),
			(T)(lhs * rhs.a));
	}

	template <typename T>
	constexpr TColor4<T> operator/(const TColor4<T>& lhs, const T rhs)
	{
		return TColor4<T>(
			lhs.r / rhs,
			lhs.g / rhs,
			lhs.b / rhs,
			lhs.a / rhs);
	}

	using Color4f = TColor4<float>;
	using Color4d = TColor4<double>;
	using Color4i = TColor4<int>;
	using Color4ui = TColor4<unsigned int>;
	using Color4s = TColor4<short>;
	using Color4us = TColor4<unsigned short>;
	using Color4b = TColor4<char>;
	using Color4ub = TColor4<unsigned char>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TColor4<float>;
	extern template class TColor4<double>;
	extern template class TColor4<int>;
	extern template class TColor4<unsigned int>;
	extern template class TColor4<short>;
	extern template class TColor4<unsigned short>;
	extern template class TColor4<char>;
	extern template class TColor4<unsigned char>;
#endif

	// Scales the color by 255 and then clamps to 0 to 65535
	constexpr Color4i ToColor4i(const Color4f color, float scale = 255.0f, int min_value = 0, int max_value = 65535)
	{
		return color.ToColor4<int, float>(scale, min_value, max_value);
	}

	// Scales the color by 255 and then clamps to 0 to 255
	constexpr Color4ub ToColor4ub(const Color4f color, float scale = 255.99f, int min_value = 0, int max_value = 255)
	{
		return color.ToColor4<unsigned char, float>(scale, (unsigned char)min_value, (unsigned char)max_value);
	}

	// Scales the color by 1/255 and then clamps to 0 to 1
	constexpr Color4f ToColor4f(const Color4ub color, float scale = 1.0f / 255.0f, float min_value = 0.0f, float max_value = 1.0f)
	{
		return color.ToColor4<float>(scale, min_value, max_value);
	}

	// Scales the color by 1/255 and then clamps to 0 to 1
	constexpr Color4f ToColor4f(const Color4i color, float scale = 1.0f / 255.0f, float min_value = 0.0f, float max_value = 1.0f)
	{
		return color.ToColor4<float>(scale, min_value, max_value);
	}

	//////////////////////////////////////////////////////////////////////
	// M A T H   F U N C T I O N S ///////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	//constexpr Color4f sqrt(const Color4f & color) noexcept
	//{
	//	return Color4f(
	//		std::sqrt(color.r),
	//		std::sqrt(color.g),
	//		std::sqrt(color.b),
	//		std::sqrt(color.a)
	//	);
	//}

	//constexpr Color4d sqrt(const Color4d & color) noexcept
	//{
	//	return Color4d(
	//		std::sqrt(color.r),
	//		std::sqrt(color.g),
	//		std::sqrt(color.b),
	//		std::sqrt(color.a)
	//	);
	//}

	//template <typename T>
	//constexpr TColor4<T> SRGBtoRGB(const TColor4<T> & c) noexcept
	//{
	//	return Color4f(
	//		SRGBToLinear<T>(color.r),
	//		SRGBToLinear<T>(color.g),
	//		SRGBToLinear<T>(color.b),
	//		color.a
	//	);
	//}

	//template <typename T>
	//constexpr TColor4<T> RGBtoSRGB(const TColor4<T> & c) noexcept
	//{
	//	return Color4f(
	//		LinearToSRGB<T>(color.r),
	//		LinearToSRGB<T>(color.g),
	//		LinearToSRGB<T>(color.b),
	//		color.a
	//	);
	//}

	//constexpr Color4f SRGBtoRGBf(const Color4f & color) noexcept
	//{
	//	return Color4f(
	//		SRGBToLinear<f32_t>(color.r),
	//		SRGBToLinear<f32_t>(color.g),
	//		SRGBToLinear<f32_t>(color.b),
	//		color.a
	//	);
	//}

	//constexpr Color4d SRGBtoRGBd(const Color4d & color) noexcept
	//{
	//	return Color4d(
	//		SRGBToLinear<f64_t>(color.r),
	//		SRGBToLinear<f64_t>(color.g),
	//		SRGBToLinear<f64_t>(color.b),
	//		color.a
	//	);
	//}

	//constexpr Color4f RGBtoSRGBf(const Color4f & color) noexcept
	//{
	//	return Color4f(
	//		LinearToSRGB<f32_t>(color.r),
	//		LinearToSRGB<f32_t>(color.g),
	//		LinearToSRGB<f32_t>(color.b),
	//		color.a
	//	);
	//}

	//constexpr Color4d RGBtoSRGBf(const Color4d & color) noexcept
	//{
	//	return Color4d(
	//		LinearToSRGB<f64_t>(color.r),
	//		LinearToSRGB<f64_t>(color.g),
	//		LinearToSRGB<f64_t>(color.b),
	//		color.a
	//	);
	//}
} // namespace Fluxions

#endif
