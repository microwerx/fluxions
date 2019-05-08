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
#include "pch.h"
#include <algorithm>
#include <fluxions_bbox.hpp>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace Fluxions
{

BBox::BBox()
	: minBounds(FLT_MAX, FLT_MAX, FLT_MAX),
	  maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{
}

BBox::~BBox()
{
}

void BBox::Reset()
{
	maxBounds = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	minBounds = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
}

void BBox::operator+=(const Vector3f &point)
{
	minBounds.x = std::min(point.x, minBounds.x);
	minBounds.y = std::min(point.y, minBounds.y);
	minBounds.z = std::min(point.z, minBounds.z);
	maxBounds.x = std::max(point.x, maxBounds.x);
	maxBounds.y = std::max(point.y, maxBounds.y);
	maxBounds.z = std::max(point.z, maxBounds.z);
}

bool BBox::IsPointInside(const Vector3f &point) const
{
	if (point.x < minBounds.x || point.x > maxBounds.x)
		return false;
	if (point.y < minBounds.y || point.y > maxBounds.y)
		return false;
	if (point.z < minBounds.z || point.z > maxBounds.z)
		return false;
	return true;
}

Vector3f BBox::Center() const
{
	return Vector3f(
		(maxBounds.x - minBounds.x) / 2 + minBounds.x,
		(maxBounds.y - minBounds.y) / 2 + minBounds.y,
		(maxBounds.z - minBounds.z) / 2 + minBounds.z);
}
} // namespace Fluxions
