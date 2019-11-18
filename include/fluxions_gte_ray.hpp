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
#ifndef FLUXIONS_GTE_RAY_HPP
#define FLUXIONS_GTE_RAY_HPP

#include <fluxions_gte_vector3.hpp>

namespace Fluxions
{
template <typename T>
class TRay
{
  public:
    TVector3<T> origin;
    TVector3<T> direction;

    TRay() {}
    TRay(const TVector3<T> &newOrigin, const TVector3<T> &newDirection)
    {
        origin = newOrigin;
        direction = newDirection.norm();
    }
    ~TRay() {}

    constexpr TRay<T>& operator=(const TRay<T> &ray) noexcept
    {
        origin = ray.origin;
        direction = ray.direction;
        return *this;
    }

    // casting operator
    template <typename U>
    operator TRay<U>() const
    {
        return TRay<U>(origin, direction);
    }

    // set this ray to be a new ray. newDirection is normalized.
    constexpr auto set(const TVector3<T> &newOrigin, const TVector3<T> &newDirection) noexcept
    {
        origin = newOrigin;
        direction = newDirection.norm();

        return *this;
    }

    // set this ray to be a new ray. newDirection is not normalized.
    constexpr auto set_unnormalized(const TVector3<T> &newOrigin, const TVector3<T> &newDirection) noexcept
    {
        origin = newOrigin;
        direction = newDirection;

        return *this;
    }

    // return new std::vector pointing t units along direction from origin.
    constexpr auto getPointAtParameter(const T t) const noexcept
    {
        return origin + t * direction;
    }
};

using Rayf = TRay<float>;
using Rayd = TRay<double>;
using Rayi = TRay<int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TRay<float>;
extern template class TRay<double>;
extern template class TRay<int>;
#endif
} // namespace Fluxions

#endif
