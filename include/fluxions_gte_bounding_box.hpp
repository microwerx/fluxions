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
#ifndef FLUXIONS_GTE_BOUNDING_BOX_HPP
#define FLUXIONS_GTE_BOUNDING_BOX_HPP

#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{
template <typename T>
class TBoundingBox
{
  public:
	TBoundingBox();
	TBoundingBox(const TBoundingBox<T> &bbox);
	~TBoundingBox();
	TBoundingBox<T> &operator=(const TBoundingBox<T> &bbox);

	// resets the bounds to:
	// maxBounds(-1000000000, -1000000000, -1000000000)
	// minBounds( 1000000000,  1000000000,  1000000000)
	void Reset();

	// adds the point (X, y, 0) to the box, expanding it if necessary
	void operator+=(const TVector2<T> &point);
	// adds the point (X, y, z) to the box, expanding it if necessary
	void operator+=(const TVector3<T> &point);
	// adds the point (X, y, z) to the box, expanding it if necessary
	void operator+=(const TVector4<T> &point);

	// returns true if (X, y, 0) is inside the box
	bool IsPointInside(const TVector2<T> &point) const;
	// returns true if (X, y, z) is inside the box
	bool IsPointInside(const TVector3<T> &point) const;
	// returns true if (X, y, z) is inside the box
	bool IsPointInside(const TVector4<T> &point) const;
	const TVector3<T> Size() const { return TVector3<T>(X(), Y(), Z()); }
	const TVector3<T> SizeRounded() const { return TVector3<T>((T)trunc(X() + 0.5f), (T)trunc(Y() + 0.5f), (T)trunc(Z() + 0.5f)); }
	T MinXRounded() const { return (T)trunc(MinX() - 0.5f); }
	T MinYRounded() const { return (T)trunc(MinY() - 0.5f); }
	T MinZRounded() const { return (T)trunc(MinZ() - 0.5f); }
	T MaxXRounded() const { return (T)trunc(MaxX() + 0.5f); }
	T MaxYRounded() const { return (T)trunc(MaxY() + 0.5f); }
	T MaxZRounded() const { return (T)trunc(MaxZ() + 0.5f); }
	const TVector3<T> MinRounded() const { return TVector3<T>(MinXRounded(), MinYRounded(), MinZRounded()); }
	const TVector3<T> MaxRounded() const { return TVector3<T>(MaxXRounded(), MaxYRounded(), MaxZRounded()); }
	// returns the size of box's X dimension
	T X() const { return maxBounds.x - minBounds.x; }
	// returns the size of the box's Y dimension
	T Y() const { return maxBounds.y - minBounds.y; }
	// returns the size of the box's Z dimension
	T Z() const { return maxBounds.z - minBounds.z; }
	// returns the largest size of the box's X, Y, or Z dimensions
	T MaxSize() const { return max3(X(), Y(), Z()); }
	T MaxSizeRounded() const { return (T)(trunc(MaxSize() + 0.5f)); }
	// returns the smallest size of the box's X, Y, or Z dimensions
	T MinSize() const { return min3(X(), Y(), Z()); }
	T MinSizeRounded() const { return (T)(trunc(MinSize() + 0.5f)); }
	// returns the 3D point of the center of the box
	T Radius() const { return (T)(sqrt(0.75f) * MaxSize()); }
	T Diameter() const { return (T)(sqrt(3.0f) * MaxSize()); }
	T RadiusRounded() const { return (T)(trunc(Radius() + 0.5f)); }
	T DiameterRounded() const { return (T)(trunc(Diameter() + 0.5f)); }
	TVector3<T> Center() const;
	// returns the smallest X coordinate of the box
	T MinX() const { return minBounds.x; }
	// returns the smallest y coordinate of the box
	T MinY() const { return minBounds.y; }
	// returns the smallest z coordinate of the box
	T MinZ() const { return minBounds.z; }
	// returns the largest X coordinate of the box
	T MaxX() const { return maxBounds.x; }
	// returns the largest y coordinate of the box
	T MaxY() const { return maxBounds.y; }
	// returns the largest z coordinate of the box
	T MaxZ() const { return maxBounds.z; }

	TVector3<T> minBounds;
	TVector3<T> maxBounds;
};

template <typename T>
TBoundingBox<T>::TBoundingBox()
	: minBounds(1000000000, 1000000000, 1000000000),
	  maxBounds(-1000000000, -1000000000, -1000000000)
{
}

template <typename T>
TBoundingBox<T>::TBoundingBox(const TBoundingBox<T> &bbox)
{
	minBounds = bbox.minBounds;
	maxBounds = bbox.maxBounds;
}

template <typename T>
TBoundingBox<T>::~TBoundingBox()
{
}

template <typename T>
TBoundingBox<T> &TBoundingBox<T>::operator=(const TBoundingBox<T> &bbox)
{
	minBounds = bbox.minBounds;
	maxBounds = bbox.maxBounds;
	return *this;
}

template <typename T>
void TBoundingBox<T>::Reset()
{
	maxBounds = TVector3<int>(-1000000000, -1000000000, -1000000000);
	minBounds = TVector3<int>(1000000000, 1000000000, 1000000000);
}

template <typename T>
void TBoundingBox<T>::operator+=(const TVector2<T> &point)
{
	minBounds.x = std::min(point.x, minBounds.x);
	minBounds.y = std::min(point.y, minBounds.y);
	minBounds.z = std::min((T)0, minBounds.z);
	maxBounds.x = std::max(point.x, maxBounds.x);
	maxBounds.y = std::max(point.y, maxBounds.y);
	maxBounds.z = std::max((T)0, maxBounds.z);
}

template <typename T>
void TBoundingBox<T>::operator+=(const TVector3<T> &point)
{
	minBounds.x = std::min(point.x, minBounds.x);
	minBounds.y = std::min(point.y, minBounds.y);
	minBounds.z = std::min(point.z, minBounds.z);
	maxBounds.x = std::max(point.x, maxBounds.x);
	maxBounds.y = std::max(point.y, maxBounds.y);
	maxBounds.z = std::max(point.z, maxBounds.z);
}

template <typename T>
void TBoundingBox<T>::operator+=(const TVector4<T> &point)
{
	minBounds.x = std::min(point.x, minBounds.x);
	minBounds.y = std::min(point.y, minBounds.y);
	minBounds.z = std::min(point.z, minBounds.z);
	maxBounds.x = std::max(point.x, maxBounds.x);
	maxBounds.y = std::max(point.y, maxBounds.y);
	maxBounds.z = std::max(point.z, maxBounds.z);
}

template <typename T>
bool TBoundingBox<T>::IsPointInside(const TVector2<T> &point) const
{
	if (point.x < minBounds.x || point.x > maxBounds.x)
		return false;
	if (point.y < minBounds.y || point.y > maxBounds.y)
		return false;
	if (0 < minBounds.z || 0 > maxBounds.z)
		return false;
	return true;
}

template <typename T>
bool TBoundingBox<T>::IsPointInside(const TVector3<T> &point) const
{
	if (point.x < minBounds.x || point.x > maxBounds.x)
		return false;
	if (point.y < minBounds.y || point.y > maxBounds.y)
		return false;
	if (point.z < minBounds.z || point.z > maxBounds.z)
		return false;
	return true;
}

template <typename T>
bool TBoundingBox<T>::IsPointInside(const TVector4<T> &point) const
{
	if (point.x < minBounds.x || point.x > maxBounds.x)
		return false;
	if (point.y < minBounds.y || point.y > maxBounds.y)
		return false;
	if (point.z < minBounds.z || point.z > maxBounds.z)
		return false;
	return true;
}

template <typename T>
TVector3<T> TBoundingBox<T>::Center() const
{
	return TVector3<T>(
		(maxBounds.x - minBounds.x) / 2 + minBounds.x,
		(maxBounds.y - minBounds.y) / 2 + minBounds.y,
		(maxBounds.z - minBounds.z) / 2 + minBounds.z);
}

using BoundingBoxf = TBoundingBox<float>;
using BoundingBoxd = TBoundingBox<double>;
using BoundingBoxi = TBoundingBox<int>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TBoundingBox<float>;
extern template class TBoundingBox<double>;
extern template class TBoundingBox<int>;
#endif
} // namespace Fluxions

#endif
