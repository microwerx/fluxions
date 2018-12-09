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
#ifndef FLUXIONS_GTE_HPP
#define FLUXIONS_GTE_HPP

// #include <fluxions_stdcxx.hpp>
#include <fluxions_gte_math.hpp>
#include <fluxions_gte_rect.hpp>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>
#include <fluxions_gte_matrix2.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_matrix_extra.hpp>
#include <fluxions_gte_matrix_stack.hpp>
#include <fluxions_gte_quaternion.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_gte_plane.hpp>
#include <fluxions_gte_bounding_box.hpp>
#include <fluxions_gte_ray.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_color_math.hpp>
#include <fluxions_gte_image.hpp>

namespace Fluxions
{

// Matrix Vector Multiplication

//template <typename T>
//constexpr TVector4<T> operator * (const TMatrix4<T> &M, const TVector4<T> &V)
//{
//	return TVector4<T>(
//		M.m11 * V.X + M.m12 * V.y + M.m13 * V.z + M.m14 * V.w,
//		M.m21 * V.X + M.m22 * V.y + M.m23 * V.z + M.m24 * V.w,
//		M.m31 * V.X + M.m32 * V.y + M.m33 * V.z + M.m34 * V.w,
//		M.m41 * V.X + M.m42 * V.y + M.m43 * V.z + M.m44 * V.w
//		);
//}
} // namespace Fluxions

#endif
