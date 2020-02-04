#ifndef FLUXIONS_GTE_MATRIX_STACK_HPP
#define FLUXIONS_GTE_MATRIX_STACK_HPP

#include <fluxions_gte_matrix4.hpp>

namespace Fluxions {
	template <typename T, unsigned stackDepth = 32>
	class Matrix4Stack {
	public:
		void reset() {
			curlevel = 0;
			LoadIdentity();
		}

		void LoadIdentity() {
			matrices_[curlevel].LoadIdentity();
		}

		void LoadMatrix(const TMatrix4<T>&& m) {
			matrices_[curlevel] = m;
		}

		void MultMatrix(const TMatrix4<T>&& m) {
			matrices_[curlevel].MultMatrix(m);
		}

		void PushMatrix() {
			if (curlevel > stackDepth)
				return;
			curlevel++;
			matrices_[curlevel].operator=(matrices_[curlevel - 1]);
		}

		void PopMatrix() {
			if (curlevel == 0)
				return;
			curlevel--;
		}

		void Rotate(T angle, T x, T y, T z) {
			matrices_[curlevel].Rotate(angle, x, y, z);
		}

		void Translate(T x, T y, T z) {
			matrices_[curlevel].Translate(x, y, z);
		}

		void Scale(T x, T y, T z) {
			matrices_[curlevel].Scale(x, y, z);
		}

		void Frustum(T left, T right, T bottom, T top, T znear, T zfar) {
			matrices_[curlevel].Frustum(left, right, bottom, top, znear, zfar);
		}

		void Perspective(T fovy, T aspect, T znear, T zfar) {
			matrices_[curlevel].PerspectiveY(fovy, aspect, znear, zfar);
		}

		void Ortho(T left, T right, T bottom, T top, T znear, T zfar) {
			matrices_[curlevel].Ortho(left, right, bottom, top, znear, zfar);
		}

		void Ortho2D(T left, T right, T bottom, T top) {
			matrices_[curlevel].Ortho2D(left, right, bottom, top);
		}

	private:
		TMatrix4<T> matrices_[stackDepth];
		mutable unsigned curlevel = 0;
	};

	using Matrix4Stackf = Matrix4Stack<float>;
	using Matrix4Stackd = Matrix4Stack<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class Matrix4Stack<float, 32>;
	extern template class Matrix4Stack<double, 32>;
#endif
} // namespace Fluxions

#endif
