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
#include <fluxions_gte_color4.hpp>

namespace Fluxions
{
template <>
float TColor4<float>::to_float_factor = 1.0f;
template <>
float TColor4<float>::from_float_factor = 1.0f;
template <>
float TColor4<double>::to_float_factor = 1.0f;
template <>
float TColor4<double>::from_float_factor = 1.0f;
template <>
float TColor4<unsigned char>::to_float_factor = 255.99f;
template <>
float TColor4<unsigned char>::from_float_factor = 1.0f / 255.00f;
template <>
float TColor4<unsigned int>::to_float_factor = 255.99f;
template <>
float TColor4<unsigned int>::from_float_factor = 1.0f / 255.00f;
template <>
float TColor4<unsigned short>::to_float_factor = 255.99f;
template <>
float TColor4<unsigned short>::from_float_factor = 1.0f / 255.00f;
template <>
float TColor4<char>::to_float_factor = 127.99f;
template <>
float TColor4<char>::from_float_factor = 1.0f / 127.00f;
template <>
float TColor4<int>::to_float_factor = 255.99f;
template <>
float TColor4<int>::from_float_factor = 1.0f / 255.00f;
template <>
float TColor4<short>::to_float_factor = 255.99f;
template <>
float TColor4<short>::from_float_factor = 1.0f / 255.00f;

template <>
TColor4<float>::value_type TColor4<float>::min_value = 0.0f;
template <>
TColor4<float>::value_type TColor4<float>::max_value = 1.0f;
template <>
TColor4<double>::value_type TColor4<double>::min_value = 0.0;
template <>
TColor4<double>::value_type TColor4<double>::max_value = 1.0;
template <>
TColor4<char>::value_type TColor4<char>::min_value = 0;
template <>
TColor4<char>::value_type TColor4<char>::max_value = 127;
template <>
TColor4<short>::value_type TColor4<short>::min_value = 0;
template <>
TColor4<short>::value_type TColor4<short>::max_value = 255;
template <>
TColor4<int>::value_type TColor4<int>::min_value = 0;
template <>
TColor4<int>::value_type TColor4<int>::max_value = 65535;
template <>
TColor4<unsigned char>::value_type TColor4<unsigned char>::min_value = 0;
template <>
TColor4<unsigned char>::value_type TColor4<unsigned char>::max_value = 255;
template <>
TColor4<unsigned short>::value_type TColor4<unsigned short>::min_value = 0;
template <>
TColor4<unsigned short>::value_type TColor4<unsigned short>::max_value = 65535;
template <>
TColor4<unsigned int>::value_type TColor4<unsigned int>::min_value = 0;
template <>
TColor4<unsigned int>::value_type TColor4<unsigned int>::max_value = 65535;

template <>
unsigned int TColor4<float>::gl_type = 0x1406; // GL_FLOAT
template <>
unsigned int TColor4<double>::gl_type = 0x140A; // GL_DOUBLE
template <>
unsigned int TColor4<char>::gl_type = 0x1400; // GL_BYTE
template <>
unsigned int TColor4<unsigned char>::gl_type = 0x1401; // GL_UNSIGNED_BYTE
template <>
unsigned int TColor4<short>::gl_type = 0x1402; // GL_SHORT
template <>
unsigned int TColor4<unsigned short>::gl_type = 0x1403; // GL_UNSIGNED_SHORT
template <>
unsigned int TColor4<int>::gl_type = 0x1404; // GL_INT
template <>
unsigned int TColor4<unsigned int>::gl_type = 0x1405; // GL_UNSIGNED_INT
template <>
unsigned int TColor4<float>::gl_size = 4;
template <>
unsigned int TColor4<double>::gl_size = 4;
template <>
unsigned int TColor4<char>::gl_size = 4;
template <>
unsigned int TColor4<unsigned char>::gl_size = 4;
template <>
unsigned int TColor4<short>::gl_size = 4;
template <>
unsigned int TColor4<unsigned short>::gl_size = 4;
template <>
unsigned int TColor4<int>::gl_size = 4;
template <>
unsigned int TColor4<unsigned int>::gl_size = 4;

template class TColor4<float>;
template class TColor4<double>;
template class TColor4<char>;
template class TColor4<unsigned char>;
template class TColor4<short>;
template class TColor4<unsigned short>;
template class TColor4<int>;
template class TColor4<unsigned int>;

} // namespace Fluxions