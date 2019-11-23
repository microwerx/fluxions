#ifndef FLUXIONS_GTE_TRANSFORMATION_HPP
#define FLUXIONS_GTE_TRANSFORMATION_HPP

#include <utility>
#include <string>
#include <sstream>
#include <fluxions_gte_quaternion.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{

template <typename T>
class TTransformation {
public:
	using value_type = T;
	using vector_type = TVector3<T>;
	using position_type = TVector3<T>;
	using rotation_type = TQuaternion<T>;

	constexpr TTransformation() noexcept {}

	constexpr TTransformation(const position_type& position, const rotation_type& rotation) noexcept
		: position{ position }, rotation{ rotation } {}

	constexpr TTransformation(position_type&& position, rotation_type&& rotation) noexcept
		: position{ std::move(position) }, rotation{ std::move(rotation) } {}

	constexpr TTransformation<T> operator+(const TTransformation<T>& other) const noexcept {
		return TTransformation<T>(position + other.position, rotation + other.rotation);
	}

	constexpr TTransformation<T> operator-(const TTransformation<T>& other) const noexcept {
		return TTransformation<T>{ position - other.position, rotation - other.rotation	};
	}

	constexpr TTransformation<T> operator*(const T t) const noexcept {
		return TTransformation<T>{ position * t, rotation * t };
	}

	constexpr TMatrix4<T> toMatrix4() const noexcept {
		return TMatrix4<T>{
			rotation.m11(), rotation.m12(), rotation.m13(), position.x,
			rotation.m21(), rotation.m22(), rotation.m23(), position.y,
			rotation.m31(), rotation.m32(), rotation.m33(), position.z,
			0, 0, 0, 1
		};
	}

	//inline vector_type origin() noexcept const {
	//	return vector_type{position};
	//}

	//inline vector_type uaxis() noexcept const {
	//	return vector_type{rotation.m11(), rotation.m21(), rotation.m31()};
	//}

	//inline vector_type vaxis() noexcept const {
	//	return vector_type{rotation.m12(), rotation.m22(), rotation.m32()};
	//}

	//inline vector_type waxis() noexcept const {
	//	return vector_type{rotation.m13(), rotation.m23(), rotation.m33()};
	//}

	inline std::string toString() const noexcept {
		std::ostringstream os;
		os << "{ 'P': { ";
		os << position.x << ", " << position.y << ", " << position.z;
		os << " }, { 'R' : { ";
		os << rotation.a << ", " << rotation.b << ", " << rotation.c << ", " << rotation.d << " }";
		os << " }";
		return os.str();
	}

	position_type position;
	rotation_type rotation;
};

using Transformationf = TTransformation<float>;
using Transformationd = TTransformation<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TTransformation<float>;
extern template class TTransformation<double>;
#endif

void TestGteTransformations() noexcept;

}

#endif