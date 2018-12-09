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

#ifndef FLUXIONS_BBOX_HPP
#define FLUXIONS_BBOX_HPP

#include <float.h>
#include <numeric>
#include <fluxions_gte.hpp>
#include <fluxions_gte_vector3.hpp>

namespace Fluxions
{
class BBox
{
  public:
	BBox();
	~BBox();

	void Reset();
	void operator+=(const Vector3f &point);
	bool IsPointInside(const Vector3f &point) const;
	float MaxXSize() const { return maxBounds.x - minBounds.x; }
	float MaxYSize() const { return maxBounds.y - minBounds.y; }
	float MaxZSize() const { return maxBounds.z - minBounds.z; }
	float MaxUniformScale() const { return max3(MaxXSize(), MaxYSize(), MaxZSize()); }
	Vector3f Center() const;

	Vector3f minBounds;
	Vector3f maxBounds;
};
} // namespace Fluxions

#endif
