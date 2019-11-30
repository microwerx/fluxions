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

#ifndef FLUXIONS_GTE_VECTOR_MATH_HPP
#define FLUXIONS_GTE_VECTOR_MATH_HPP

#include <vector>
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>
#include <fluxions_gte_quaternion.hpp>

// This header defines math between scalars and vector types
// * scalars and vectors
// * vectors and scalars
// * vectors and vectors

namespace Fluxions
{

template <typename ScalarType, typename VectorType>
ScalarType CatmullRomKnot(ScalarType t0,
						  ScalarType alpha,
						  const VectorType& P0,
						  const VectorType& P1) {
	VectorType difference = P1 - P0;
	return std::pow(difference.length(), 0.5f * alpha) + t0;
}

template <typename ScalarType, typename VectorType>
VectorType CatmullRomSegment(ScalarType t,
							 const VectorType& P0,
							 const VectorType& P1,
							 const VectorType& P2,
							 const VectorType& P3,
							 ScalarType alpha = 0.5) {
	ScalarType t0 = 0;
	ScalarType t1 = CatmullRomKnot(t0, alpha, P0, P1);
	ScalarType t2 = CatmullRomKnot(t1, alpha, P1, P2);
	ScalarType t3 = CatmullRomKnot(t2, alpha, P2, P3);

	VectorType A1 = (t1 - t) / (t1 - t0) * P0 + (t - t0) / (t1 - t0) * P1;
	VectorType A2 = (t2 - t) / (t2 - t1) * P1 + (t - t1) / (t2 - t1) * P2;
	VectorType A3 = (t3 - t) / (t3 - t2) * P2 + (t - t2) / (t3 - t2) * P3;
	VectorType B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
	VectorType B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

	t = lerp(t, t1, t2);

	return (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomSpline(const std::vector<VectorType>& controlPoints,
										  std::vector<VectorType>& points,
										  unsigned first = 1,
										  unsigned last = 0,
										  unsigned pointsPerSegment = 60,
										  ScalarType alpha = 0.5) {
	last = (last > 0) ? last : controlPoints.size() - 2;
	const unsigned count = controlPoints.size();
	const ScalarType dt = ScalarType(1) / ScalarType(pointsPerSegment);
	points.resize(count * pointsPerSegment);

	unsigned out = 0;
	for (unsigned cp = first; cp < last; cp++) {
		const VectorType& P0 = controlPoints[(cp - 1) % count];
		const VectorType& P1 = controlPoints[(cp + 0) % count];
		const VectorType& P2 = controlPoints[(cp + 1) % count];
		const VectorType& P3 = controlPoints[(cp + 2) % count];
		ScalarType t = 0;
		for (unsigned i = 0; i < pointsPerSegment; i++) {
			points[out++] = std::move(CatmullRomSegment(t, P0, P1, P2, P3, alpha));
			t += dt;
		}
	}

	return points;
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomOpenSpline(const std::vector<VectorType>& controlPoints,
											  std::vector<VectorType>& points,
											  unsigned pointsPerSegment = 60,
											  ScalarType alpha = 0.5) {
	return CatmullRomSpline(controlPoints, points, 1, controlPoints.size() - 2, pointsPerSegment, alpha);
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomClosedSpline(const std::vector<VectorType>& controlPoints,
												std::vector<VectorType>& points,
												unsigned pointsPerSegment = 60,
												ScalarType alpha = 0.5) {
	return CatmullRomSpline(controlPoints, points, 0, controlPoints.size() - 1, pointsPerSegment, alpha);
}

}
#endif
