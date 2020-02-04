#include "fluxions_pch.hpp"
#include <algorithm>
#include <fluxions_bbox.hpp>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace Fluxions {
	BBox::BBox()
		: minBounds(FLT_MAX, FLT_MAX, FLT_MAX),
		maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}

	BBox::~BBox() {}

	void BBox::reset() {
		maxBounds = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		minBounds = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	void BBox::operator+=(const Vector3f& point) {
		minBounds.x = std::min(point.x, minBounds.x);
		minBounds.y = std::min(point.y, minBounds.y);
		minBounds.z = std::min(point.z, minBounds.z);
		maxBounds.x = std::max(point.x, maxBounds.x);
		maxBounds.y = std::max(point.y, maxBounds.y);
		maxBounds.z = std::max(point.z, maxBounds.z);
	}

	bool BBox::IsPointInside(const Vector3f& point) const {
		if (point.x < minBounds.x || point.x > maxBounds.x)
			return false;
		if (point.y < minBounds.y || point.y > maxBounds.y)
			return false;
		if (point.z < minBounds.z || point.z > maxBounds.z)
			return false;
		return true;
	}

	Vector3f BBox::Center() const {
		return Vector3f(
			(maxBounds.x - minBounds.x) / 2 + minBounds.x,
			(maxBounds.y - minBounds.y) / 2 + minBounds.y,
			(maxBounds.z - minBounds.z) / 2 + minBounds.z);
	}
} // namespace Fluxions
