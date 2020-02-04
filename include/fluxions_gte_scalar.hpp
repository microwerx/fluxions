#ifndef FLUXIONS_GTE_SCALAR_HPP
#define FLUXIONS_GTE_SCALAR_HPP

namespace Fluxions {
	template <typename T>
	class TScalar {
	public:
		union {
			T x;
			T v[1];
		};

		TScalar()
			: x(0) {}

		TScalar(const T x_)
			: x(x_) {}

		TScalar(const TScalar<T>& value)
			: x(value.x) {}

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
