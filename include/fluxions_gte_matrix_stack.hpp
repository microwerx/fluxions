// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
template <typename T, int stackDepth = 32>
class Matrix4Stack
{
    void Reset()
    {
        curlevel = 0;
        LoadIdentity();
    }

    void LoadIdentity()
    {
        matrices_[curlevel].LoadIdentity();
    }

    void LoadMatrix(const TMatrix4<T> &&m)
    {
        matrices_[curlevel] = m;
    }

    void MultMatrix(const TMatrix4<T> &&m)
    {
        matrices_[curlevel].MultMatrix(m);
    }

    void PushMatrix()
    {
        if (curlevel > stackDepth)
            return;
        curlevel++;
        matrices_[curlevel] = matrices_[curlevel - 1];
    }

    void PopMatrix()
    {
        if (curlevel == 0)
            return;
        curlevel--;
    }

    template <typename T2>
    void Rotate(T2 angle, T2 x, T2 y, T2 z)
    {
        matrices_[curlevel].Rotate(angle, x, y, z);
    }

    template <typename T2>
    void Translate(T2 x, T2 y, T2 z)
    {
        matrices_[curlevel].Translate(x, y, z);
    }

    template <typename T2>
    void Scale(T2 x, T2 y, T2 z)
    {
        matrices_[curlevel].Scale(x, y, z);
    }

    template <typename T2>
    void Frustum(T2 left, T2 right, T2 bottom, T2 top, T2 near, T2 far)
    {
        matrices_[curlevel].Frustum(left, right, bottom, top, near, far);
    }

    template <typename T2>
    void Perspective(T2 fovy, T2 aspect, T2 near, T2 far)
    {
        matrices_[curlevel].Perspective(fovy, aspect, far);
    }

    template <typename T2>
    void Ortho(T2 left, T2 right, T2 bottom, T2 top, T2 near, T2 far)
    {
        matrices_[curlevel].Ortho(left, right, bottom, top, near, far);
    }

    template <typename T2>
    void Ortho2D(T2 left, T2 right, T2 bottom, T2 top)
    {
        matrices_[curlevel].Ortho2D(left, right, bottom, top);
    }

  private:
    TMatrix4<T> matrices_[stackDepth];
    mutable int curlevel = 0;
};

using Matrix4Stackf = Matrix4Stack<float>;
using Matrix4Stackd = Matrix4Stack<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class Matrix4Stack<float, 32>;
extern template class Matrix4Stack<double, 32>;
#endif
} // namespace Fluxions

#endif
