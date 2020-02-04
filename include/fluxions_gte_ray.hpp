#ifndef FLUXIONS_GTE_RAY_HPP
#define FLUXIONS_GTE_RAY_HPP

#include <fluxions_gte_vector3.hpp>

namespace Fluxions {
	template <typename T>
	class TRay {
	public:
		TVector3<T> origin;
		TVector3<T> direction;

		TRay() {}
		TRay(const TVector3<T>& newOrigin, const TVector3<T>& newDirection) {
			origin = newOrigin;
			direction = newDirection.unit();
		}
		~TRay() {}

		constexpr TRay<T>& operator=(const TRay<T>& ray) noexcept {
			origin = ray.origin;
			direction = ray.direction;
			return *this;
		}

		// casting operator
		template <typename U>
		operator TRay<U>() const {
			return TRay<U>(origin, direction);
		}

		// set this ray to be a new ray. newDirection is normalized.
		constexpr auto set(const TVector3<T>& newOrigin, const TVector3<T>& newDirection) noexcept {
			origin = newOrigin;
			direction = newDirection.unit();

			return *this;
		}

		// set this ray to be a new ray. newDirection is not normalized.
		constexpr auto set_unnormalized(const TVector3<T>& newOrigin, const TVector3<T>& newDirection) noexcept {
			origin = newOrigin;
			direction = newDirection;

			return *this;
		}

		// return new std::vector pointing t units along direction from origin.
		constexpr auto getPointAtParameter(const T t) const noexcept {
			return origin + t * direction;
		}
	};

	using Rayf = TRay<float>;
	using Rayd = TRay<double>;
	using Rayi = TRay<int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TRay<float>;
	extern template class TRay<double>;
	extern template class TRay<int>;
#endif
} // namespace Fluxions

#endif
