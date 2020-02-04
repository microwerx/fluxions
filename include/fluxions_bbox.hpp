#ifndef FLUXIONS_BBOX_HPP
#define FLUXIONS_BBOX_HPP

#include <float.h>
#include <numeric>
#include <fluxions_gte.hpp>
#include <fluxions_gte_vector3.hpp>

namespace Fluxions {
	class BBox {
	public:
		BBox();
		~BBox();

		void reset();
		void operator+=(const Vector3f& point);
		bool IsPointInside(const Vector3f& point) const;
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
