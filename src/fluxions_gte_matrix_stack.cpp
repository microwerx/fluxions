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
#include <fluxions_gte_matrix_stack.hpp>

namespace Fluxions
{
	template class Matrix4Stack<float, 32>;
	template class Matrix4Stack<double, 32>;

	void Matrix4StackTest()
	{
		Matrix4Stackf ms;
		ms.Reset();
		ms.LoadIdentity();
		ms.Frustum(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f);
		ms.Ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
		ms.Ortho2D(0.0f, 1920.0f, 1080.0f, 0.0f);
		ms.PushMatrix();
		ms.PopMatrix();
	}
} // namespace Fluxions