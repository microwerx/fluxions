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
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include "stdafx.h"
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{
// template specializations
template class TVector4<float>;
template class TVector4<double>;
template class TVector4<signed char>;
template class TVector4<unsigned char>;
template class TVector4<int>;
template class TVector4<unsigned int>;
template class TVector4<short>;
template class TVector4<unsigned short>;
} // namespace Fluxions
