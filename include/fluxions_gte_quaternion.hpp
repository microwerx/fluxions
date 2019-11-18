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
#ifndef FLUXIONS_GTE_QUATERNION_HPP
#define FLUXIONS_GTE_QUATERNION_HPP

#include <fluxions_gte_matrix3.hpp>

namespace Fluxions
{
using QScalar = float;

// Quaterion operations:
// q = a + bi + cj + dk

template <typename T = QScalar>
class TQuaternion
{
  public:
    T a, b, c, d;

    constexpr T *ptr() noexcept { return &a; }
    constexpr const T *const_ptr() const noexcept { return &a; }

    using type = T;

    TQuaternion()
        : a(0), b(0), c(0), d(0)
    {
    }

    TQuaternion(const T aVal, const T bVal, const T cVal, const T dVal)
        : a(aVal), b(bVal), c(cVal), d(dVal)
    {
    }

    TQuaternion(const TQuaternion &q)
        : a(q.a), b(q.b), c(q.c), d(q.d)
    {
    }

    const TQuaternion<T> &operator=(const TQuaternion<T> &q)
    {
        b = q.b;
        c = q.c;
        d = q.d;
        a = q.a;
        return *this;
    }

    static TQuaternion<T> makeFromAngleAxis(double angleInDegrees, double x, double y, double z);
    static TQuaternion<T> makeFromMatrix3(const TMatrix3<T> &M);

    void toAngleAxis(double &angleInDegrees, double &x, double &y, double &z);
    TMatrix3<T> toMatrix3() const;

    TQuaternion<T> conjugate() const
    {
        return TQuaternion(-b, -c, -d, a);
    }

    T norm() const
    {
        return sqrt(b * b + c * c + d * d + a * a);
    }

    TQuaternion<T> operator*(T alpha) const
    {
        return TQuaternion(alpha * b, alpha * c, alpha * d, alpha * a);
    }

    T dot(const TQuaternion &q) const
    {
        return (b * q.b + c * q.c + d * q.d);
    }

    TQuaternion<T> cross(const TQuaternion<T> &q) const
    {
        return TQuaternion(
            c * q.d - d * q.c,
            d * q.b - b * q.d,
            b * q.c - c * q.b,
            0.);
    }
};

template <typename T>
TQuaternion<T> TQuaternion<T>::makeFromAngleAxis(double angleInDegrees, double x, double y, double z)
{
    double c = cos(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
    double s = sin(0.5 * angleInDegrees * FX_DEGREES_TO_RADIANS);
    return TQuaternion<T>((T)c, (T)(s * x), (T)(s * y), (T)(s * z));
}

template <typename T>
TQuaternion<T> TQuaternion<T>::makeFromMatrix3(const TMatrix3<T> &M)
{
    T trace = M.Trace();
    T r = (T)sqrt(1 + trace);
    T w = (T)(0.5 * r);
    T x = (T)copysign(0.5 * sqrt(1 + M.m11 - M.m22 - M.m33), M.m32 - M.m23);
    T y = (T)copysign(0.5 * sqrt(1 - M.m11 + M.m22 - M.m33), M.m13 - M.m31);
    T z = (T)copysign(0.5 * sqrt(1 - M.m11 - M.m22 + M.m33), M.m21 - M.m12);
    return TQuaternion<T>(w, x, y, z);
}

template <typename T>
void TQuaternion<T>::toAngleAxis(double &angleInDegrees, double &x, double &y, double &z)
{
    angleInDegrees = 2.0 * acos(a) * FX_RADIANS_TO_DEGREES;
    double denominator = sqrt(1 - a * a);
    if (denominator > 0.0)
    {
        x = b / denominator;
        y = c / denominator;
        z = d / denominator;
    }
    else
    {
        x = 1.0;
        y = 0.0;
        z = 0.0;
    }
}

template <typename T>
TMatrix3<T> TQuaternion<T>::toMatrix3() const
{
    return TMatrix3<T>(
        1 - 2 * c * c - 2 * d * d, 2 * b * c - 2 * a * d, 2 * b * d + 2 * a * c,
        2 * b * c + 2 * a * d, 1 - 2 * b * b - 2 * d * d, 2 * c * d - 2 * a * b,
        2 * b * d - 2 * a * c, 2 * c * d + 2 * a * b, 1 - 2 * b * b - 2 * c * c);
}

using Quaternionf = TQuaternion<float>;
using Quaterniond = TQuaternion<double>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TQuaternion<float>;
extern template class TQuaternion<double>;
#endif

} // namespace Fluxions

#endif
