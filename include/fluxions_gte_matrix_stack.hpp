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
#ifndef FLUXIONS_GTE_MATRIX_STACK_HPP
#define FLUXIONS_GTE_MATRIX_STACK_HPP

#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
	template <typename T, unsigned stackDepth = 32>
	class Matrix4Stack
	{
	public:
		void Reset()
		{
			curlevel = 0;
			LoadIdentity();
		}

		void LoadIdentity()
		{
			matrices_[curlevel].LoadIdentity();
		}

		void LoadMatrix(const TMatrix4<T>&& m)
		{
			matrices_[curlevel] = m;
		}

		void MultMatrix(const TMatrix4<T>&& m)
		{
			matrices_[curlevel].MultMatrix(m);
		}

		void PushMatrix()
		{
			if (curlevel > stackDepth)
				return;
			curlevel++;
			matrices_[curlevel].operator=(matrices_[curlevel - 1]);
		}

		void PopMatrix()
		{
			if (curlevel == 0)
				return;
			curlevel--;
		}

		void Rotate(T angle, T x, T y, T z)
		{
			matrices_[curlevel].Rotate(angle, x, y, z);
		}

		void Translate(T x, T y, T z)
		{
			matrices_[curlevel].Translate(x, y, z);
		}

		void Scale(T x, T y, T z)
		{
			matrices_[curlevel].Scale(x, y, z);
		}

		void Frustum(T left, T right, T bottom, T top, T near, T far)
		{
			matrices_[curlevel].Frustum(left, right, bottom, top, near, far);
		}

		void Perspective(T fovy, T aspect, T near, T far)
		{
			matrices_[curlevel].PerspectiveY(fovy, aspect, near, far);
		}

		void Ortho(T left, T right, T bottom, T top, T near, T far)
		{
			matrices_[curlevel].Ortho(left, right, bottom, top, near, far);
		}

		void Ortho2D(T left, T right, T bottom, T top)
		{
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
