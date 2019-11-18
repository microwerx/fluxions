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

#ifndef FLUXIONS_GTE_RECT_HPP
#define FLUXIONS_GTE_RECT_HPP

#define NOMINMAX

#include <algorithm>
#include <fluxions_gte_vector2.hpp>

namespace Fluxions
{
template <typename T>
class TRect
{
  public:
    T x, y, w, h;

    constexpr T *ptr() noexcept { return &x; }
    constexpr const T *const_ptr() const noexcept { return &x; }

    using type = T;

    constexpr T *v() noexcept { return &x; }
    constexpr const T *v() const noexcept { return &x; }

    constexpr TRect() noexcept
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    constexpr TRect(T _x, T _y, T _w, T _h) noexcept
        : x(_x), y(_y), w(_w), h(_h)
    {
    }

    constexpr TRect(const TRect<T> &rect) noexcept
        : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
    {
    }

    // initialize like OpenGL viewport
    constexpr TRect(const T V[4]) noexcept
        : x(V[0]), y(V[1]), w(V[2]), h(V[3])
    {
    }

    // initialize with two points
    constexpr TRect(const TVector2<T> v1, const TVector2<T> v2) noexcept
    {
		x = Fluxions::min2(v1.x, v2.x);
		y = Fluxions::min2(v1.y, v2.y);
		T x2 = Fluxions::max2(v1.x, v2.x);
		T y2 = Fluxions::max2(v1.y, v2.y);
        w = x2 - x;
        h = y2 - y;
    }

    template <typename T2>
    constexpr TVector2<T> &operator=(const TRect<T2> &rect) noexcept
    {
        x = (T)rect.x;
        y = (T)rect.y;
        w = (T)rect.w;
        h = (T)rect.h;
        return *this;
    }

    template <typename Other>
    constexpr operator TRect<Other>() const noexcept
    {
        return TRect<Other>((Other)x, (Other)y, (Other)w, (Other)h);
    }

    // x1 <= x2
    constexpr T x1() const noexcept { return x; }
    // y1 <= y2
    constexpr T y1() const noexcept { return y; }
    // x1 <= x2
    constexpr T x2() const noexcept { return x + w; }
    // y1 <= y2
    constexpr T y2() const noexcept { return y + h; }

    constexpr T width() const noexcept { return w; }
    constexpr T height() const noexcept { return h; }
    constexpr T left() const noexcept { return x; }
    constexpr T right() const noexcept { return x + w; }
    constexpr T top() const noexcept { return y + h; }
    constexpr T bottom() const noexcept { return y; }
    constexpr T area() const noexcept { return w * h; }
    constexpr TVector2<T> lowerLeft() const noexcept { return TVector2<T>(left(), bottom()); }
    constexpr TVector2<T> lowerRight() const noexcept { return TVector2<T>(right(), bottom()); }
    constexpr TVector2<T> upperLeft() const noexcept { return TVector2<T>(left(), top()); }
    constexpr TVector2<T> upperRight() const noexcept { return TVector2<T>(right(), top()); }

    constexpr TVector2<T> middle(bool relative = false) const noexcept { return relative ? TVector2<T>(w / 2, h / 2) : TVector2<T>(y + w / 2, x + h / 2); }
    constexpr TVector2<T> percent(float pctx, float pcty, bool relative = false) const noexcept { return relative ? TVector2<T>((T)(w * pctx), (T)(h * pcty)) : TVector2<T>(x + (T)(w * pctx), y + (T)(h * pcty)); }
    constexpr TVector2<T> percent(const Vector2f pct, bool relative = false) const noexcept { return relative ? TVector2<T>((T)(w * pct.x), (T)(h * pct.y)) : TVector2<T>(x + (T)(w * pct.x), y + (T)(h * pct.y)); }

    constexpr double aspectRatio() const { return (double)w / (double)h; }
    constexpr float aspectRatiof() const { return (float)aspectRatio(); }

    constexpr TRect<T> GetUpperLeftRect(const TVector2<T> splitPoint) const noexcept { return TRect<T>(Clamp(splitPoint), upperLeft()); }
    constexpr TRect<T> GetUpperRightRect(const TVector2<T> splitPoint) const noexcept { return TRect<T>(Clamp(splitPoint), upperRight()); }
    constexpr TRect<T> GetLowerLeftRect(const TVector2<T> splitPoint) const noexcept { return TRect<T>(Clamp(splitPoint), lowerLeft()); }
    constexpr TRect<T> GetLowerRightRect(const TVector2<T> splitPoint) const noexcept { return TRect<T>(Clamp(splitPoint), lowerRight()); }

    enum Quadrant
    {
        UpperLeft = 0,
        UpperRight = 1,
        LowerLeft = 2,
        LowerRight = 3
    };

    constexpr TRect<T> GetQuadrant(Quadrant quadrant, const TVector2<T> splitPoint) const noexcept
    {
        if (quadrant == UpperLeft)
            return GetUpperLeftRect(splitPoint);
        if (quadrant == UpperRight)
            return GetUpperRightRect(splitPoint);
        if (quadrant == LowerLeft)
            return GetLowerLeftRect(splitPoint);
        if (quadrant == LowerRight)
            return GetLowerRightRect(splitPoint);
        return TRect<T>();
    }

    constexpr TRect<T> &SetFromPoints(const TVector2<T> v1, const TVector2<T> v2) noexcept
    {
        x = Fluxions::min2(v1.x, v2.x);
        y = Fluxions::min2(v1.y, v2.y);
        T x2 = Fluxions::max2(v1.x, v2.x);
        T y2 = Fluxions::max2(v1.y, v2.y);
        w = x2 - x;
        h = y2 - y;
        return *this;
    }

    constexpr bool IsInside(const TVector2<T> p) const noexcept
    {
        return (x1() >= p.x && x2() <= p.x) && (y1() >= p.y && y2() <= p.y);
    }

    constexpr TVector2<T> Clamp(const TVector2<T> p) const noexcept
    {
        return TVector2<T>(clamp(p.x, x1(), x2()), clamp(p.y, y1(), y2()));
    }
};

using Rectf = TRect<float>;
using Rectd = TRect<double>;
using Rects = TRect<short>;
using Rectus = TRect<unsigned short>;
using Recti = TRect<int>;
using Rectui = TRect<unsigned>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TRect<float>;
extern template class TRect<double>;
extern template class TRect<short>;
extern template class TRect<unsigned short>;
extern template class TRect<int>;
extern template class TRect<unsigned int>;
#endif
} // namespace Fluxions

#endif