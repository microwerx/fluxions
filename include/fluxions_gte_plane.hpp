#ifndef FLUXIONS_GTE_PLANE_HPP
#define FLUXIONS_GTE_PLANE_HPP

namespace Fluxions {
	template <typename T>
	class TPlane {
	public:
		T a, b, c, d;

		constexpr T* ptr() noexcept { return &a; }
		constexpr const T* const_ptr() const noexcept { return &a; }

		using type = T;

		TPlane<T>();
		~TPlane<T>();
	};

	template <typename T>
	TPlane<T>::TPlane()
		: a(0), b(0), c(0), d(0) {}

	template <typename T>
	TPlane<T>::~TPlane() {}

	using Planef = TPlane<float>;
	using Planed = TPlane<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TPlane<float>;
	extern template class TPlane<double>;
#endif
} // namespace Fluxions

#endif
