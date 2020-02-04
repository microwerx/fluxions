#ifndef FLUXIONS_GTE_COLOR_MATH_HPP
#define FLUXIONS_GTE_COLOR_MATH_HPP

#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>

namespace Fluxions {
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
		constexpr bool is_int = std::is_integral_v<T>;
		constexpr bool is_flt = std::is_floating_point_v<T>;
		if constexpr (is_int && sizeof(T) == 1)
			return T(255);
		else if constexpr (is_int && sizeof(T) >= 2)
			return T(65535);
		else if constexpr (is_flt)
			return T(1);
		else
			return T(0);
	}

	template<typename T>
	constexpr float color_to_float_factor() {
		bool is_int = std::is_integral_v<T>;
		bool is_flt = std::is_floating_point_v<T>;
		if (is_int && sizeof(T) == 1)
			return float(1.0 / 255.0);
		else if (is_int && sizeof(T) >= 2)
			return float(1.0 / 65535.0);
		else if (is_flt)
			return 1.0f;
		else
			return 0.0f;
	}

	template<typename T>
	constexpr float color_from_float_factor() {
		bool is_int = std::is_integral_v<T>;
		bool is_flt = std::is_floating_point_v<T>;
		if (is_int && sizeof(T) == 1)
			return float(255.99);
		else if (is_int && sizeof(T) >= 2)
			return float(65535.99);
		else if (is_flt)
			return 1.0f;
		else
			return 0.0f;
	}

	template <typename T>
	constexpr TColor3<float> ToColor3f(TColor3<T>& src) noexcept {
		return TColor3<float>(
			(float)src[0] * color_to_float_factor<T>(),
			(float)src[1] * color_to_float_factor<T>(),
			(float)src[2] * color_to_float_factor<T>());
	}

	template <typename T>
	constexpr TColor3<float> ToColor3f(TColor4<T>& src) noexcept {
		return TColor3<float>(
			(float)src[0] * color_to_float_factor<T>(),
			(float)src[1] * color_to_float_factor<T>(),
			(float)src[2] * color_to_float_factor<T>());
	}

	template <typename T>
	constexpr TColor3<T>& FromColor3f(TColor3<T>& dst, const TColor3<float>& src) {
		dst[0] = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.r);
		dst[1] = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.g);
		dst[2] = (typename TColor3<T>::value_type)(TColor4<T>::from_float_factor * (float)src.b);
		return dst;
	}

	template <typename T>
	constexpr TColor4<T>& FromColor3f(TColor4<T>& dst, const TColor3<float>& src) {
		dst[0] = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.r);
		dst[1] = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.g);
		dst[2] = (typename TColor4<T>::value_type)(TColor4<T>::from_float_factor * (float)src.b);
		return dst;
	}
} // namespace Fluxions
#endif
