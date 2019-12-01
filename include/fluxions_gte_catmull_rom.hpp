#ifndef FLUXIONS_CATMULL_ROM_HPP
#define FLUXIONS_CATMULL_ROM_HPP

#include <vector>
#include <algorithm>
#include <fluxions_gte_vector_math.hpp>

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

	t = lerp(t, t1, t2);

	VectorType A1 = (t1 - t) / (t1 - t0) * P0 + (t - t0) / (t1 - t0) * P1;
	VectorType A2 = (t2 - t) / (t2 - t1) * P1 + (t - t1) / (t2 - t1) * P2;
	VectorType A3 = (t3 - t) / (t3 - t2) * P2 + (t - t2) / (t3 - t2) * P3;
	VectorType B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
	VectorType B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

	return (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;
}

template <typename ScalarType, typename VectorType>
VectorType CatmullRomSplinePoint(ScalarType t,
								 const std::vector<VectorType>& controlPoints,
								 const std::vector<ScalarType>& controlAlpha) {
	const unsigned size = (unsigned)controlPoints.size();
	int cp1 = int(t) % size;
	int cp0 = (cp1 - 1) % size;
	int cp2 = (cp1 + 1) % size;
	int cp3 = (cp1 + 2) % size;
	t = t - int(t);
	ScalarType alpha = lerp(t, controlAlpha[cp1], controlAlpha[cp2]);
	return CatmullRomSegment(t,
							 controlPoints[cp0],
							 controlPoints[cp1],
							 controlPoints[cp2],
							 controlPoints[cp3],
							 alpha);
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomSpline(const std::vector<VectorType>& controlPoints,
										  std::vector<VectorType>& points,
										  int first = 1,
										  int last = 0,
										  unsigned pointsPerSegment = 60,
										  ScalarType alpha = 0.5) {
	last = (last > 0) ? last : (unsigned)controlPoints.size() - 2;
	const unsigned count = (unsigned)controlPoints.size();
	const ScalarType dt = ScalarType(1) / ScalarType(pointsPerSegment);
	const unsigned pcount = (last - first) * pointsPerSegment + 1;
	points.resize(pcount);
	std::vector<ScalarType> controlAlpha(controlPoints.size(), alpha);

	unsigned out = 0;
	for (int cp = first; cp < last; cp++) {
		//int cp0 = (cp - 1) % count;
		//int cp1 = (cp) % count;
		//int cp2 = (cp + 1) % count;
		//int cp3 = (cp + 2) % count;
		//const VectorType& P0 = controlPoints[cp0];
		//const VectorType& P1 = controlPoints[cp1];
		//const VectorType& P2 = controlPoints[cp2];
		//const VectorType& P3 = controlPoints[cp3];
		//ScalarType t = 0;
		//for (unsigned i = 0; i < pointsPerSegment; i++) {
		//	points[out++] = std::move(CatmullRomSegment(t, P0, P1, P2, P3, alpha));
		//	t += dt;
		//}
		ScalarType t = 0;
		for (unsigned i = 0; i < pointsPerSegment; i++) {
			points[out++] = CatmullRomSplinePoint(cp + t, controlPoints, controlAlpha);
			t += dt;
		}
	}
	points[out] = controlPoints[last % count];

	return points;
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomOpenSpline(const std::vector<VectorType>& controlPoints,
											  std::vector<VectorType>& points,
											  unsigned pointsPerSegment = 60,
											  ScalarType alpha = 0.5) {
	return CatmullRomSpline(controlPoints, points, 1, (unsigned)controlPoints.size() - 2, pointsPerSegment, alpha);
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomClosedSpline(const std::vector<VectorType>& controlPoints,
												std::vector<VectorType>& points,
												unsigned pointsPerSegment = 60,
												ScalarType alpha = 0.5) {
	return CatmullRomSpline(controlPoints, points, 0, (unsigned)controlPoints.size(), pointsPerSegment, alpha);
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomSpline(const std::vector<VectorType>& controlPoints,
										  const std::vector<ScalarType>& controlAlpha,
										  const std::vector<ScalarType>& curveTime,
										  std::vector<VectorType>& points) {
	points.resize(curveTime.size());
	for (unsigned i = 0; i < points.size(); i++) {
		points[i] = std::move(CatmullRomSplinePoint(curveTime[i],
													controlPoints,
													controlAlpha));
	}
	return points;
}


template <typename ScalarType, typename VectorType>
ScalarType CatmullRomSplineUniformIt(const std::vector<VectorType>& controlPoints,
									 const std::vector<ScalarType>& controlAlpha,
									 const std::vector<ScalarType>& arc,
									 std::vector<ScalarType>& arclength,
									 std::vector<VectorType>& points) {
	const unsigned count = (unsigned)arc.size();
	ScalarType totalLength = 0;
	for (unsigned i = 0; i < count; i++) {
		points[i] = std::move(CatmullRomSplinePoint(arc[i],
													controlPoints,
													controlAlpha));
		if (i > 0) {
			arclength[i] = (points[i] - points[i - 1]).length();
			totalLength += arclength[i];
		}
	}
	arclength[0] = (points[points.size() - 1] - points[0]).length();
	totalLength += arclength[0];
	return totalLength;
}

template <typename ScalarType>
ScalarType variance(ScalarType mean, const std::vector<ScalarType>& v) {
	ScalarType sum = 0;
	for (auto& x : v) {
		sum += std::pow(x - mean, 2);
	}
	return sum / v.size();
}

template <typename ScalarType, typename VectorType>
std::vector<VectorType>& CatmullRomSplineUniform(const std::vector<VectorType>& controlPoints,
												 const std::vector<ScalarType>& controlAlpha,
												 const std::vector<ScalarType>& curveTime,
												 std::vector<VectorType>& points) {
	const unsigned count = (unsigned)curveTime.size();
	std::vector<ScalarType> arclength(count);
	std::vector<ScalarType> arc(curveTime);
	points.resize(count);
	auto t0t1 = std::minmax_element(curveTime.begin(), curveTime.end());
	ScalarType dt = (*t0t1.second - *t0t1.first) / (ScalarType)count;

	ScalarType var = 0;
	for (int relax = 0; relax < 1; relax++) {
		ScalarType S = CatmullRomSplineUniformIt(controlPoints,
												 controlAlpha,
												 arc,
												 arclength,
												 points);
		//ScalarType mean = S / count;
		//var = variance(mean, arclength);
		//for (unsigned i = 1; i < count - 1; i++) {
		//	if (arclength[i] < mean) {

		//		arc[i] += dt;
		//	} if (arclength[i] > mean) {
		//		arc[i] -= dt;
		//	}
		//}
		//dt *= 0.5f;
	}

	return points;
}

}

#endif