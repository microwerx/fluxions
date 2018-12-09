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
#ifndef FLUXIONS_GTE_COLOR_MATH_HPP
#define FLUXIONS_GTE_COLOR_MATH_HPP

#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>

namespace Fluxions
{
template <typename T>
constexpr TColor3<float> ToColor3f(TColor3<T> &src) noexcept
{
    return TColor3<float>(
        (float)src.r * TColor3<T>::to_float_factor,
        (float)src.g * TColor3<T>::to_float_factor,
        (float)src.b * TColor3<T>::to_float_factor);
}

template <typename T>
constexpr TColor3<float> ToColor3f(TColor4<T> &src) noexcept
{
    return TColor3<float>(
        (float)src.r * TColor4<T>::to_float_factor,
        (float)src.g * TColor4<T>::to_float_factor,
        (float)src.b * TColor4<T>::to_float_factor);
}

template <typename T>
constexpr void FromColor3f(TColor3<T> &dst, const TColor3<float> &src)
{
    dst.r = (typename TColor3<T>::type)(TColor4<T>::from_float_factor * (float)src.r);
    dst.g = (typename TColor3<T>::type)(TColor4<T>::from_float_factor * (float)src.g);
    dst.b = (typename TColor3<T>::type)(TColor4<T>::from_float_factor * (float)src.b);
}

template <typename T>
constexpr void FromColor3f(TColor4<T> &dst, const TColor3<float> &src)
{
    dst.r = (typename TColor4<T>::type)(TColor4<T>::from_float_factor * (float)src.r);
    dst.g = (typename TColor4<T>::type)(TColor4<T>::from_float_factor * (float)src.g);
    dst.b = (typename TColor4<T>::type)(TColor4<T>::from_float_factor * (float)src.b);
}

} // namespace Fluxions
#endif