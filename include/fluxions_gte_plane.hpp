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
#ifndef FLUXIONS_GTE_PLANE_HPP
#define FLUXIONS_GTE_PLANE_HPP

namespace Fluxions
{
template <typename T>
class TPlane
{
  public:
    T a, b, c, d;

    constexpr T *ptr() noexcept { return &a; }
    constexpr const T *const_ptr() const noexcept { return &a; }

    using type = T;

    TPlane<T>();
    ~TPlane<T>();
};

template <typename T>
TPlane<T>::TPlane()
    : a(0), b(0), c(0), d(0)
{
}

template <typename T>
TPlane<T>::~TPlane()
{
}

using Planef = TPlane<float>;
using Planed = TPlane<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TPlane<float>;
extern template class TPlane<double>;
#endif
} // namespace Fluxions

#endif
