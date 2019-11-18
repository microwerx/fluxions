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
#ifndef FLUXIONS_GTE_COLOR3_HPP
#define FLUXIONS_GTE_COLOR3_HPP

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_vector3.hpp>

namespace Fluxions
{
	template <typename T>
	class TColor4;
	template <typename T>
	class TVector3;
	template <typename T>
	class TVector4;

	template <typename T>
	class TColor3 : public TCommonColor<T>
	{
	public:
		T r, g, b;

		using value_type = typename TCommonColor<T>::value_type;

		static float to_float_factor;
		static float from_float_factor;

		static value_type min_value;
		static value_type max_value;

		static unsigned int gl_type; // UNSIGNED_BYTE, FLOAT, etc.
		static unsigned int gl_size; // 3 for RGB, 4 for RGBA

		using iterator = TCommonIterator<T>;
		using const_iterator = TCommonIterator<const T>;

		constexpr unsigned size() const noexcept { return 3; }
		constexpr iterator begin() noexcept { return iterator(&r); }
		constexpr iterator end() noexcept { return iterator(&r + size()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(&r); }
		constexpr const_iterator cend() const noexcept { return const_iterator(&r + size()); }

		constexpr T* ptr() noexcept { return &r; }
		constexpr const T* const_ptr() const noexcept { return &r; }

		constexpr T& operator[](unsigned index) noexcept { return (&r)[index]; }
		constexpr const T& operator[](unsigned index) const noexcept { return (&r)[index]; }

		constexpr TColor3()
			: r(0), g(0), b(0) {}

		constexpr TColor3(const T value)
			: r(value), g(value), b(value) {}

		constexpr TColor3(const T newR, const T newG, const T newB)
			: r(newR), g(newG), b(newB) {}

		constexpr TColor3(const TColor3<T>& color)
			: r(color.r), g(color.g), b(color.b) {}

		template <typename OtherType>
		constexpr operator TColor4<OtherType>() const {
			return TColor4<OtherType>((OtherType)r, (OtherType)g, (OtherType)b, (OtherType)0);
		}

		template <typename OtherType>
		constexpr operator TColor3<OtherType>() const {
			return TColor3<OtherType>((OtherType)r, (OtherType)g, (OtherType)b);
		}

		template <typename OtherType>
		const TColor3<T>& operator=(const TColor3<OtherType>& color) {
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator=(const TColor4<OtherType>& color) {
			r = (T)color.r;
			g = (T)color.g;
			b = (T)color.b;
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator=(const TVector3<OtherType>& color) {
			r = (T)color.x;
			g = (T)color.y;
			b = (T)color.z;
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator=(const TVector4<OtherType>& color) {
			r = (T)color.x;
			g = (T)color.t;
			b = (T)color.z;
			return *this;
		}

		const TColor3<T>& operator=(const T value) {
			r = value;
			g = value;
			b = value;
			return *this;
		}

		TColor3<T>& reset(T r_, T g_, T b_) {
			r = r_;
			g = g_;
			b = b_;
			return *this;
		}

		TColor3<T>& reset(const T value) {
			r = value;
			g = value;
			b = value;
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator+=(const TColor3<OtherType>& color) {
			r = (T)(r + color.r);
			g = (T)(g + color.g);
			b = (T)(b + color.b);
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator-=(const TColor3<OtherType>& color) {
			r = (T)(r - color.r);
			g = (T)(g - color.g);
			b = (T)(b - color.b);
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator*=(const TColor3<OtherType>& color) {
			r = (T)(r * color.r);
			g = (T)(g * color.g);
			b = (T)(b * color.b);
			return *this;
		}

		template <typename OtherType>
		const TColor3<T>& operator/=(const TColor3<OtherType>& color) {
			r = (T)(r / color.r);
			g = (T)(g / color.g);
			b = (T)(b / color.b);
			return *this;
		}

		const TColor3<T>& operator+=(const T value) {
			r = (T)(r + value);
			g = (T)(g + value);
			b = (T)(b + value);
			return *this;
		}

		const TColor3<T>& operator-=(const T value) {
			r = (T)(r - value);
			g = (T)(g - value);
			b = (T)(b - value);
			return *this;
		}

		const TColor3<T>& operator*=(const T value) {
			r = (T)(r * value);
			g = (T)(g * value);
			b = (T)(b * value);
			return *this;
		}

		const TColor3<T>& operator/=(const T value) {
			r = (T)(r / value);
			g = (T)(g / value);
			b = (T)(b / value);
			return *this;
		}

		template <typename OtherType>
		constexpr TColor3<T> operator+(const TColor3<OtherType>& color) {
			return TColor3<T>(
				(T)(r + color.r),
				(T)(g + color.g),
				(T)(b + color.b));
		}

		template <typename OtherType>
		constexpr TColor3<T> operator-(const TColor3<OtherType>& color) {
			return TColor3<T>(
				(T)(r - color.r),
				(T)(g - color.g),
				(T)(b - color.b));
		}

		template <typename OtherType>
		constexpr TColor3<T> operator*(const TColor3<OtherType>& color) {
			return TColor3<T>(
				(T)(r * color.r),
				(T)(g * color.g),
				(T)(b * color.b));
		}

		template <typename OtherType>
		constexpr TColor3<T> operator*(const OtherType& value) {
			return TColor3<T>(
				(T)(value * r),
				(T)(value * g),
				(T)(value * b));
		}

		template <typename OtherType>
		constexpr TColor3<T> operator/(const TColor3<OtherType>& color) {
			return TColor3<T>(
				(T)(r / color.r),
				(T)(g / color.g),
				(T)(b / color.b));
		}

		template <typename OtherType>
		constexpr TColor3<T> pow(const OtherType power) {
			return TColor3<T>(
				(T)std::pow(r, power),
				(T)std::pow(g, power),
				(T)std::pow(b, power));
		}

		template <typename OtherType>
		constexpr TColor3<OtherType> to_other_type(const OtherType min_value, const OtherType max_value) {
			return TColor3<OtherType>(
				remap_value_min_max<T, OtherType>(r, min_value, max_value),
				remap_value_min_max<T, OtherType>(g, min_value, max_value),
				remap_value_min_max<T, OtherType>(b, min_value, max_value));
		}

		template <typename OtherType>
		constexpr TColor3<OtherType> to_other_type(const OtherType min1, const OtherType max1, const OtherType min2, const OtherType max2) {
			return TColor3<OtherType>(
				remap_value_min_max<T, OtherType>(r, min1, max1, min2, max2),
				remap_value_min_max<T, OtherType>(g, min1, max1, min2, max2),
				remap_value_min_max<T, OtherType>(b, min1, max1, min2, max2));
		}

		template <typename OtherType>
		constexpr TColor3<OtherType> Remap(const T src_min_value, const T src_max_value, const OtherType dst_min_value, const OtherType dst_max_value) {
			return TColor3<OtherType>(
				remap_value_min_max<T, OtherType>(r, src_min_value, src_max_value, dst_min_value, dst_max_value),
				remap_value_min_max<T, OtherType>(g, src_min_value, src_max_value, dst_min_value, dst_max_value),
				remap_value_min_max<T, OtherType>(b, src_min_value, src_max_value, dst_min_value, dst_max_value));
		}

		constexpr TColor3<T> Clamp(T minValue, T maxValue) const {
			return TColor3<T>(
				Fluxions::clamp<T>(r, minValue, maxValue),
				Fluxions::clamp<T>(g, minValue, maxValue),
				Fluxions::clamp<T>(b, minValue, maxValue));
		}

		constexpr TColor3<T> ScaleClamp(T scale, T minValue, T maxValue) const {
			return TColor3<T>(
				Fluxions::clamp<T>(r * scale, minValue, maxValue),
				Fluxions::clamp<T>(g * scale, minValue, maxValue),
				Fluxions::clamp<T>(b * scale, minValue, maxValue));
		}

		// constexpr TColor3<float> ToColor3f() const noexcept
		// {
		//     return TColor3<float>(
		//         r * to_float_factor,
		//         g * to_float_factor,
		//         b * to_float_factor);
		// }

		// constexpr TColor3<T> &FromColor3f(const TColor3<T> color)
		// {
		//     r = (type)(from_float_factor * color.r);
		//     g = (type)(from_float_factor * color.g);
		//     b = (type)(from_float_factor * color.b);
		//     return *this;
		// }

		constexpr TColor3<T>& clamp() noexcept {
			r = Fluxions::clamp(r, min_value, max_value);
			g = Fluxions::clamp(r, min_value, max_value);
			b = Fluxions::clamp(r, min_value, max_value);
			return *this;
		}

		template <typename OtherType, typename ScaleType>
		constexpr TColor3<OtherType> ToColor3(const ScaleType scale, const OtherType min_value, const OtherType max_value) const {
			return TColor3<OtherType>(
				(OtherType)Fluxions::clamp<OtherType>(OtherType(r * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(g * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(b * scale), min_value, max_value));
		}

		template <typename OtherType, typename ScaleType>
		constexpr TColor4<OtherType> ToColor4(const ScaleType scale, const OtherType min_value, const OtherType max_value) const {
			return TColor4<OtherType>(
				(OtherType)Fluxions::clamp<OtherType>(OtherType(r * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(g * scale), min_value, max_value),
				(OtherType)Fluxions::clamp<OtherType>(OtherType(b * scale), min_value, max_value),
				0);
		}

		template <typename OtherType = T>
		constexpr TColor3<OtherType> ToColor3() const {
			return TColor3<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b);
		}

		template <typename OtherType = T>
		constexpr TColor4<OtherType> ToColor4() const {
			return TColor4<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b,
				(OtherType)0);
		}

		template <typename OtherType = T>
		constexpr TVector3<OtherType> ToVector3() const {
			return TVector3<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b);
		}

		template <typename OtherType = T>
		constexpr TVector4<OtherType> ToVector4() const {
			return TVector4<OtherType>(
				(OtherType)r,
				(OtherType)g,
				(OtherType)b,
				(OtherType)0);
		}

		//constexpr double Max() noexcept {
		//	return (double)max3(r, g, b);
		//}

		//constexpr double Min() noexcept {
		//	return (double)min3(r, g, b);
		//}

		constexpr double Intensity() const {
			return (r + g + b) / 3.0;
		}

		constexpr double Saturation() const {
			return 1.0 - (1.0 / Intensity()) * min3(r, g, b);
		}

		constexpr double Hue() const {
			return acos(sqrt(0.5 * ((r - g) + (r - b)) / ((r - g) * (r - g) + (r - b) * (g - b))));
		}

		constexpr T minrgb() const noexcept {
			return min3<T>(r, g, b);
		}

		constexpr T maxrgb() const noexcept {
			return max3<T>(r, g, b);
		}

		static constexpr TColor3<T> min(const TColor3<T>& a, const TColor3<T>& b) noexcept {
			return TColor3<T>(
				std::min(a.r, b.r),
				std::min(a.g, b.g),
				std::min(a.b, b.b));
		}

		static constexpr TColor3<T> max(const TColor3<T>& a, const TColor3<T>& b) noexcept {
			return TColor3<T>(
				std::max(a.r, b.r),
				std::max(a.g, b.g),
				std::max(a.b, b.b));
		}
	}; // class TColor3

	template <typename T>
	constexpr TColor3<T> abs(TColor3<T> color) {
		return TColor3<T>(
			color.r < 0 ? (T)-1 : color.r > 0 ? 1 : 0,
			color.g < 0 ? (T)-1 : color.g > 0 ? 1 : 0,
			color.b < 0 ? (T)-1 : color.b > 0 ? 1 : 0);
	}

	template <typename T>
	constexpr TColor3<T> sign(TColor3<T> color) {
		return TColor3<T>(
			color.r < 0 ? (T)-1 : color.r > 0 ? 1 : 0,
			color.g < 0 ? (T)-1 : color.g > 0 ? 1 : 0,
			color.b < 0 ? (T)-1 : color.b > 0 ? 1 : 0);
	}

	template <typename T>
	constexpr TColor3<T> negate(TColor3<T> color) {
		return TColor3<T>(-color.r, -color.g, -color.b);
	}

	template <typename T>
	constexpr TColor3<T> operator*(const TColor3<T>& lhs, const T rhs) {
		return TColor3<T>(
			lhs.r * rhs,
			lhs.g * rhs,
			lhs.b * rhs);
	}

	template <typename T>
	constexpr TColor3<T> operator*(const T lhs, const TColor3<T>& rhs) {
		return TColor3<T>(
			lhs * rhs.r,
			lhs * rhs.g,
			lhs * rhs.b);
	}

	template <typename T>
	constexpr TColor3<T> operator/(const TColor3<T>& lhs, const T rhs) {
		return TColor3<T>(
			lhs.r / rhs,
			lhs.g / rhs,
			lhs.b / rhs);
	}

	template <typename T>
	constexpr bool operator==(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
	}

	template <typename T>
	constexpr bool operator!=(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return !(lhs == rhs);
	}

	template <typename T>
	constexpr bool operator<(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return (lhs.r < rhs.r) && (lhs.g < rhs.g) && (lhs.b < rhs.b);
	}

	template <typename T>
	constexpr bool operator>(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return (lhs.r > rhs.r) && (lhs.g > rhs.g) && (lhs.b > rhs.b);
	}

	template <typename T>
	constexpr bool operator<=(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return (lhs.r <= rhs.r) && (lhs.g <= rhs.g) && (lhs.b <= rhs.b);
	}

	template <typename T>
	constexpr bool operator>=(const TColor3<T>& lhs, const TColor3<T>& rhs) {
		return (lhs.r >= rhs.r) && (lhs.g >= rhs.g) && (lhs.b >= rhs.b);
	}

	using Color3f = TColor3<float>;
	using Color3d = TColor3<double>;
	using Color3i = TColor3<int>;
	using Color3ui = TColor3<unsigned int>;
	using Color3s = TColor3<short>;
	using Color3us = TColor3<unsigned short>;
	using Color3b = TColor3<char>;
	using Color3ub = TColor3<unsigned char>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TColor3<float>;
	extern template class TColor3<double>;
	extern template class TColor3<int>;
	extern template class TColor3<unsigned int>;
	extern template class TColor3<short>;
	extern template class TColor3<unsigned short>;
	extern template class TColor3<char>;
	extern template class TColor3<unsigned char>;
#endif

	// Scales the color by 255 and then clamps to 0 to 65535
	constexpr Color3i ToColor3i(const Color3f color, float scale = 255.0f, int min_value = 0, int max_value = 65535) {
		return color.ToColor3<int, float>(scale, min_value, max_value);
	}

	// Scales the color by 255 and then clamps to 0 to 255
	constexpr Color3ub ToColor3ub(const Color3f color, float scale = 255.99f, int min_value = 0, int max_value = 255) {
		return color.ToColor3<unsigned char, float>(scale, (unsigned char)min_value, (unsigned char)max_value);
	}

	// Scales the color by 1/255 and then clamps to 0 to 1
	constexpr Color3f ToColor3f(const Color3ub color, float scale = 1.0f / 255.0f, float min_value = 0.0f, float max_value = 1.0f) {
		return color.ToColor3<float>(scale, min_value, max_value);
	}

	// Scales the color by 1/255 and then clamps to 0 to 1
	constexpr Color3f ToColor3f(const Color3i color, float scale = 1.0f / 255.0f, float min_value = 0.0f, float max_value = 1.0f) {
		return color.ToColor3<float>(scale, min_value, max_value);
	}

	Color3f HLSToRGBf(float h, float l, float s) noexcept;
	Color3d HLSToRGBd(double h, double l, double s) noexcept;

	template <typename T>
	constexpr TColor3<T> SRGBtoRGB(const TColor3<T>& color) noexcept {
		return TColor3<T>(
			SRGBToLinear<T>(color.r),
			SRGBToLinear<T>(color.g),
			SRGBToLinear<T>(color.b));
	}

	template <typename T>
	constexpr TColor3<T> RGBtoSRGB(const TColor3<T>& color) noexcept {
		return TColor3<T>(
			LinearToSRGB<T>(color.r),
			LinearToSRGB<T>(color.g),
			LinearToSRGB<T>(color.b));
	}

	constexpr Color3f SRGBtoRGBf(const Color3f& color) noexcept {
		return Color3f(
			SRGBToLinear<f32_t>(color.r),
			SRGBToLinear<f32_t>(color.g),
			SRGBToLinear<f32_t>(color.b));
	}

	constexpr Color3d SRGBtoRGBd(const Color3d& color) noexcept {
		return Color3d(
			SRGBToLinear<f64_t>(color.r),
			SRGBToLinear<f64_t>(color.g),
			SRGBToLinear<f64_t>(color.b));
	}

	constexpr Color3f RGBtoSRGBf(const Color3f& color) noexcept {
		return Color3f(
			LinearToSRGB<f32_t>(color.r),
			LinearToSRGB<f32_t>(color.g),
			LinearToSRGB<f32_t>(color.b));
	}

	constexpr Color3d RGBtoSRGBf(const Color3d& color) noexcept {
		return Color3d(
			LinearToSRGB<f64_t>(color.r),
			LinearToSRGB<f64_t>(color.g),
			LinearToSRGB<f64_t>(color.b));
	}
} // namespace Fluxions

#endif
