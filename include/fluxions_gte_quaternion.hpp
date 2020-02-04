#ifndef FLUXIONS_GTE_QUATERNION_HPP
#define FLUXIONS_GTE_QUATERNION_HPP

#include <fluxions_gte_common_vector.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions {
	using QScalar = float;

	// Quaterion operations:
	// q = a + bi + cj + dk

	template <typename T = QScalar>
	class TQuaternion : public TCommonContainer<T> {
	public:
		T a, b, c, d;

		using value_type = T;
		using value_ref = T&;
		using value_ptr = T*;

		using const_value_type = const T;
		using const_value_ref = const T&;
		using const_value_ptr = const T*;

		inline value_ptr ptr() { return &a; }
		inline const_value_ptr const_ptr() const { return &a; }

		using type = T;

		constexpr TQuaternion() noexcept
			: a(0), b(0), c(0), d(0) {}

		constexpr TQuaternion(const T aVal, const T bVal, const T cVal, const T dVal) noexcept
			: a(aVal), b(bVal), c(cVal), d(dVal) {}

		constexpr TQuaternion(const TQuaternion& q) noexcept
			: a(q.a), b(q.b), c(q.c), d(q.d) {}

		const TQuaternion<T>& operator=(const TQuaternion<T>& q) noexcept {
			b = q.b;
			c = q.c;
			d = q.d;
			a = q.a;
			return *this;
		}

		static TQuaternion<T> makeFromLookDir(Vector3d dirTo, Vector3d rollDir) noexcept;
		static TQuaternion<T> makeFromAngles(double yawInDegrees, double pitchInDegrees, double rollInDegrees) noexcept;
		static TQuaternion<T> makeFromEulerXYZ(double yawInDegrees, double pitchInDegrees, double rollInDegrees) noexcept;
		static TQuaternion<T> makeFromEulerZYX(double yawInDegrees, double pitchInDegrees, double rollInDegrees) noexcept;
		static TQuaternion<T> makeFromEulerZXY(double yawInDegrees, double pitchInDegrees, double rollInDegrees) noexcept;
		static TQuaternion<T> makeFromAzElTwist(double azInDegrees, double elInDegrees, double twistInDegrees) noexcept;
		static TQuaternion<T> makeFromAngleAxis(double angleInDegrees, double x, double y, double z) noexcept;
		static TQuaternion<T> makeFromMatrix3(const TMatrix3<T>& M) noexcept;

		void toAngleAxis(double& angleInDegrees, double& x, double& y, double& z) const noexcept;
		TMatrix3<T> toMatrix3() const noexcept;
		TMatrix4<T> toMatrix4() const noexcept;

		TQuaternion<T> conjugate() const noexcept {
			return TQuaternion(-b, -c, -d, a);
		}

		T length() const noexcept {
			return sqrt(b * b + c * c + d * d + a * a);
		}

		TQuaternion<T> normalized() const noexcept {
			T invlength = T(1) / length();
			return TQuaternion<T>{a* invlength, b* invlength, c* invlength, d* invlength};
		}

		TQuaternion<T> operator-() const noexcept {
			return TQuaternion<T>{-a, -b, -c, -d};
		}

		TQuaternion<T> operator+(const TQuaternion<T>& q) const noexcept {
			return TQuaternion<T>(a + q.a, b + q.b, c + q.c, d + q.d);
		}

		TQuaternion<T> operator-(const TQuaternion<T>& q) const noexcept {
			return TQuaternion<T>(a - q.a, b - q.b, c - q.c, d - q.d);
		}

		// Hamiltonian Product
		TQuaternion<T> operator*(const TQuaternion<T>& q) const noexcept {
			return TQuaternion<T>(a * q.a - b * q.b - c * q.c - d * q.d,
								  a * q.b + b * q.a + c * q.d - d * q.c,
								  a * q.c - b * q.d + c * q.a + d * q.b,
								  a * q.d + b * q.c - c * q.b + d * q.a
								  );
		}

		TQuaternion<T> operator+(T alpha) const noexcept {
			return TQuaternion(a + alpha, b + alpha, c + alpha, d + alpha);
		}

		TQuaternion<T> operator-(T alpha) const noexcept {
			return TQuaternion(a - alpha, b - alpha, c - alpha, d - alpha);
		}

		TQuaternion<T> scale(T alpha) const noexcept {
			return TQuaternion(a * alpha, b * alpha, c * alpha, d * alpha);
		}

		TQuaternion<T> operator*(T alpha) const noexcept {
			return TQuaternion(a * alpha, b * alpha, c * alpha, d * alpha);
		}

		T dot(const TQuaternion& q) const noexcept {
			return (a * q.a + b * q.b + c * q.c + d * q.d);
		}

		TQuaternion<T> cross(const TQuaternion<T>& q) const noexcept {
			return TQuaternion(
				0,
				c * q.d - d * q.c,
				d * q.b - b * q.d,
				b * q.c - c * q.b);
		}

		TQuaternion<T> exp() const noexcept {
			// exp(a) (cos ||v|| + v/||v|| sin ||v||)
			const T magv = std::sqrt(b * b + c * c + d * d);
			const T sinv = std::sin(magv);
			const T cosv = std::cos(magv);
			const T expa = std::exp(a);
			const T C = expa * sinv / magv;
			return TQuaternion{ cosv * expa, C * b, C * c, C * d };
		}

		TQuaternion<T> log() const noexcept {
			// ln ||q|| + v/||v|| arccos(a/||q||)
			const T magq = length();
			const T magv = std::sqrt(b * b + c * c + d * d);
			const T logq = std::log(magq);
			const T acos = std::acos(a / magq);
			const T C = acos / magv;
			return TQuaternion{ logq, C * b, C * c, C * d };
		}

		TQuaternion<T> pow(T p) const noexcept {
			// exp(log(q)*p)
			return (log().scale(p)).exp();
		}

		TQuaternion<T> inverse() const noexcept {
			// q*/||q||^2
			const T magq2 = (a * a + b * b + c * c + d * d);
			return conjugate().scale(T(1) / magq2);
		}

		// theta (Y)
		T yawInDegrees() const noexcept {
			return T(FX_RADIANS_TO_DEGREES) *
				std::atan2(-m31(), std::sqrt(m32() * m32() + m33() * m33()));
			//return T(FX_RADIANS_TO_DEGREES) * std::asin(m13());
		}

		// phi (X)
		T pitchInDegrees() const noexcept {
			return T(FX_RADIANS_TO_DEGREES) * std::atan2(m32(), m33());
			//if (std::abs(std::cos(std::asin(m13()))) == 0) {
			//	return 0;
			//}
			//return T(FX_RADIANS_TO_DEGREES) * std::atan2(-m12(), m11());
		}

		// psi (Z)
		T rollInDegrees() const noexcept {
			//if (std::abs(std::cos(std::asin(m13()))) == 0) {
			//	return T(FX_RADIANS_TO_DEGREES) * std::atan2(m32(), m22());
			//}
			//return T(FX_RADIANS_TO_DEGREES) * std::atan2(-m23(), m33());
			return T(FX_RADIANS_TO_DEGREES) * std::atan2(m21(), m11());
		}

		inline constexpr value_type m11() const noexcept { return 1 - 2 * c * c - 2 * d * d; }
		inline constexpr value_type m12() const noexcept { return 2 * b * c - 2 * a * d; }
		inline constexpr value_type m13() const noexcept { return 2 * b * d + 2 * a * c; }
		inline constexpr value_type m21() const noexcept { return 2 * b * c + 2 * a * d; }
		inline constexpr value_type m22() const noexcept { return 1 - 2 * b * b - 2 * d * d; }
		inline constexpr value_type m23() const noexcept { return 2 * c * d - 2 * a * b; }
		inline constexpr value_type m31() const noexcept { return 2 * b * d - 2 * a * c; }
		inline constexpr value_type m32() const noexcept { return 2 * c * d + 2 * a * b; }
		inline constexpr value_type m33() const noexcept { return 1 - 2 * b * b - 2 * c * c; }
	};

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromLookDir(Vector3d dirTo, Vector3d rollDir) noexcept {
		dirTo = dirTo.unit();
		rollDir = rollDir.unit();
		constexpr Vector3d forward{ 0.0, 0.0, 1.0 };
		constexpr double EPS = 1e-6;
		double cosTheta = forward.dot(dirTo);
		if (std::abs(cosTheta + 1.0f) < EPS) return { T(FX_PI), T(0), T(1), T(0) };
		else if (std::abs(cosTheta - 1.0f) < EPS) return { T(0), T(0), T(0), T(0) };

		double angle = std::acos(cosTheta);
		Vector3d axis = forward.cross(dirTo);
		return TQuaternion<T>::makeFromAngleAxis(angle * FX_RADIANS_TO_DEGREES, axis.x, axis.y, axis.z);
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromAngles(double yawInDegrees, double pitchInDegrees, double rollInDegrees) noexcept {
		TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(pitchInDegrees, 1.0, 0.0, 0.0);
		TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(yawInDegrees, 0.0, 1.0, 0.0);
		TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(rollInDegrees, 0.0, 0.0, 1.0);

		return (qZ * qY * qX).normalized();
		// return (qZ * qX * qY).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromEulerXYZ(double x, double y, double z) noexcept {
		TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(x, 1.0, 0.0, 0.0);
		TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(y, 0.0, 1.0, 0.0);
		TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(z, 0.0, 0.0, 1.0);

		return (qX * qY * qZ).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromEulerZYX(double x, double y, double z) noexcept {
		TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(x, 1.0, 0.0, 0.0);
		TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(y, 0.0, 1.0, 0.0);
		TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(z, 0.0, 0.0, 1.0);

		return (qZ * qY * qX).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromEulerZXY(double x, double y, double z) noexcept {
		TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(x, 1.0, 0.0, 0.0);
		TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(y, 0.0, 1.0, 0.0);
		TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(z, 0.0, 0.0, 1.0);

		return (qZ * qX * qY).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromAzElTwist(double azInDegrees, double elInDegrees, double twistInDegrees) noexcept {
		TQuaternion<T> qX = TQuaternion<T>::makeFromAngleAxis(elInDegrees, 1.0, 0.0, 0.0);
		TQuaternion<T> qY = TQuaternion<T>::makeFromAngleAxis(azInDegrees, 0.0, 1.0, 0.0);
		TQuaternion<T> qZ = TQuaternion<T>::makeFromAngleAxis(twistInDegrees, 0.0, 0.0, 1.0);

		return (qY * qX * qZ).normalized();
		//return (qZ * qX * qY).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromAngleAxis(double angleInDegrees, double x, double y, double z) noexcept {
		double c = cos(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
		double s = sin(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
		return TQuaternion<T>((T)c, (T)(s * x), (T)(s * y), (T)(s * z)).normalized();
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::makeFromMatrix3(const TMatrix3<T>& M) noexcept {
		T trace = M.Trace();
		T r = (T)sqrt(1 + trace);
		T w = (T)(0.5 * r);
		T x = (T)copysign(0.5 * sqrt(1 + M.m11 - M.m22 - M.m33), M.m32 - M.m23);
		T y = (T)copysign(0.5 * sqrt(1 - M.m11 + M.m22 - M.m33), M.m13 - M.m31);
		T z = (T)copysign(0.5 * sqrt(1 - M.m11 - M.m22 + M.m33), M.m21 - M.m12);
		return TQuaternion<T>(w, x, y, z).normalized();
	}

	template <typename T>
	TQuaternion<T> operator*(T a, TQuaternion<T>& q) {
		return TQuaternion<T>{a* q.a, a* q.b, a* q.c, a* q.d};
	}

	template <typename T>
	void TQuaternion<T>::toAngleAxis(double& angleInDegrees, double& x, double& y, double& z) const noexcept {
		angleInDegrees = 2.0 * acos(a) * FX_RADIANS_TO_DEGREES;
		double denominator = sqrt(1 - a * a);
		if (denominator > 0.0) {
			x = b / denominator;
			y = c / denominator;
			z = d / denominator;
		}
		else {
			x = 1.0;
			y = 0.0;
			z = 0.0;
		}
	}

	template <typename T>
	TMatrix3<T> TQuaternion<T>::toMatrix3() const noexcept {
		return TMatrix3<T>(m11(), m12(), m13(),
						   m21(), m22(), m23(),
						   m31(), m32(), m33());
	}


	template <typename T>
	TMatrix4<T> TQuaternion<T>::toMatrix4() const noexcept {
		return TMatrix4<T>(m11(), m12(), m13(), 0,
						   m21(), m22(), m23(), 0,
						   m31(), m32(), m33(), 0,
						   0, 0, 0, 1);
	}

	template <typename T>
	TQuaternion<T> slerp(const TQuaternion<T>& a, const TQuaternion<T>& b, T t) noexcept {
		// Code adapted from Wikipedia article / Ken Shoemake article
		TQuaternion<T> q1 = a.normalized();
		TQuaternion<T> q2 = b.normalized();
		T dot = a.dot(b);

		if (dot < 0) {
			q2 = -q2;
			dot = -dot;
		}

		const T threshold = T(0.9995);
		if (dot > threshold) {
			TQuaternion<T> q = q1 + (q2 - q1).scale(t);
			return q.normalized();
		}

		T theta_0 = acos(dot);
		T theta = theta_0 * t;
		T sin_theta = sin(theta);
		T sin_theta_0 = sin(theta_0);

		T s0 = cos(theta) - dot * sin_theta / sin_theta_0;
		T s1 = sin_theta / sin_theta_0;

		return ((s0 * q1) + (s1 * q2)).normalized();
	}

	template <typename T>
	TQuaternion<T> squad_si(const TQuaternion<T>& q0,
							const TQuaternion<T>& q1,
							const TQuaternion<T>& q2) noexcept {
		const TQuaternion<T> q1inv = q1.inverse();
		const TQuaternion<T> log1 = (q0 * q1inv).log();
		const TQuaternion<T> log2 = (q2 * q1inv).log();
		const TQuaternion<T> tmp = (log1 + log2).scale(-0.25f);
		return tmp.exp() * q1;
	}

	template <typename T>
	TQuaternion<T> QDouble(const TQuaternion<T> p,
						   const TQuaternion<T> q) {
		return q.scale(2 * p.dot(q)) - p;
	}

	template <typename T>
	TQuaternion<T> QBisect(const TQuaternion<T> p,
						   const TQuaternion<T> q) {
		TQuaternion<T> pq = p + q;
		return pq.scale(1.0f / pq.length());
	}

	template <typename T>
	TQuaternion<T> squad_a(const TQuaternion<T>& q0,
						   const TQuaternion<T>& q1,
						   const TQuaternion<T>& q2) {
		//TQuaternion<T> q0inv = q0.inverse();
		//TQuaternion<T> q1inv = q1.inverse();
		//TQuaternion<T> p0 = q0inv * q1;
		//TQuaternion<T> p1 = q1inv * q2;
		//TQuaternion<T> arg = (p0.log() - p1.log()).scale(0.25f);
		//return q1 * arg.exp();
		return QBisect(QDouble(q0, q1), q2);
	}

	template <typename T>
	TQuaternion<T> squad_b(const TQuaternion<T>& q0,
						   const TQuaternion<T>& q1,
						   const TQuaternion<T>& q2) {
		//TQuaternion<T> q0inv = q0.inverse();
		//TQuaternion<T> q1inv = q1.inverse();
		//TQuaternion<T> p0 = q0inv * q1;
		//TQuaternion<T> p1 = q1inv * q2;
		//TQuaternion<T> arg = (p0.log() - p1.log()).scale(-0.25f);
		//return q1 * arg.exp();
		return QDouble(squad_a(q0, q1, q2), q1);
	}

	template <typename T>
	TQuaternion<T> squad(const TQuaternion<T>& q0,
						 const TQuaternion<T>& q1,
						 const TQuaternion<T>& q2,
						 const TQuaternion<T>& q3,
						 T t) noexcept {
		const TQuaternion<T> a = squad_a(q0, q1, q2);
		const TQuaternion<T> b = squad_b(q1, q2, q3);

		const TQuaternion<T> a1 = slerp(q1, a, t);
		const TQuaternion<T> a2 = slerp(a, b, t);
		const TQuaternion<T> a3 = slerp(b, q2, t);
		const TQuaternion<T> b1 = slerp(a1, a2, t);
		const TQuaternion<T> b2 = slerp(a2, a3, t);
		return slerp(b1, b2, t);
		//return slerp(slerp(q1, q2, t), slerp(a, b, t), 2 * t * (1 - t));
	}

	using Quaternionf = TQuaternion<float>;
	using Quaterniond = TQuaternion<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TQuaternion<float>;
	extern template class TQuaternion<double>;
#endif

} // namespace Fluxions

#endif
