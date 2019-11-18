#ifndef FLUXIONS_GTE_COMMON_VECTOR_HPP
#define FLUXIONS_GTE_COMMON_VECTOR_HPP

#include <type_traits>

namespace Fluxions
{
	template <typename T>
	class TCommonContainer {
	public:
		using value_type = T;

		static constexpr T zero = T(0);
		static constexpr T one = T(1);
		static constexpr T minusone = T(-1);
		static constexpr bool is_signed = T(-1) < 0;
		static constexpr bool is_int = std::is_integral_v<T>;
		static constexpr bool is_flt = std::is_floating_point_v<T>;
	};

	template <typename T>
	class TCommonColor : public TCommonContainer<typename T>
	{
	public:
		using TCommonContainer<T>::is_int;
		using TCommonContainer<T>::is_flt;

		static constexpr unsigned value_size = sizeof(T);
		static constexpr int int_max_value =
			is_int && value_size == 1 ? 255 :
			is_int && value_size == 2 ? 65535 : 255;
		static constexpr float to_float_factor =
			is_int ? (1.0f / (float)int_max_value) :
			1.0f;
		static constexpr float from_float_factor = 1.0f / to_float_factor;
		static constexpr T min_value = T(0);
		static constexpr T max_value = is_int ? T(int_max_value) : T(1);
	};

	template <typename T>
	class TCommonIterator {
	public:
		TCommonIterator(T* ptr) : ptr(ptr) {}
		TCommonIterator operator++() { ++ptr; return *this; }
		bool operator !=(const TCommonIterator& other) const { return ptr != other.ptr; }
		const T& operator*() const { return *ptr; }
		T& operator*() { return *ptr; }
	private:
		T* ptr;
	};
}
#endif
