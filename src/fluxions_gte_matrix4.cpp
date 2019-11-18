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
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
template class TMatrix4<float>;
template class TMatrix4<double>;

void ___TestMatrix4()
{
	Matrix4f m(5); // = 5.0f;
	m = 5.0;
	m = Matrix4f::MakeCubeMatrix(0);
	m = Matrix4f::MakeFrustum(0.0f, 1.0f, 0.0f, 1.0f, 0.001f, 1000.0f);
	m.Rotate(0.0, 1.0, 0.0, 0.0);
}
} // namespace Fluxions