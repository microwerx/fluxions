#ifndef FLUXIONS_SSPHH_UTILITIES_HPP
#define FLUXIONS_SSPHH_UTILITIES_HPP

#include <string>
#include <vector>
#include <fluxions_gte_color_math.hpp>

namespace Fluxions {
	bool SaveSphlOBJ(const std::string& path,
					 const std::string& name,
					 const Color3f Kd,
					 std::vector<Vector3f>& verts,
					 std::vector<Vector3ui>& triangles);

	std::string MakeREFName(const std::string& prefix, bool isCubeMap, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	std::string MakeVIZName(const std::string& prefix, int srcLightIndex, int recvLightIndex, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	std::string MakeGENName(const std::string& prefix, int recvLightIndex, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	std::string MakeHIERName(const std::string& prefix, int sendLightIndex, int MaxDegrees);
} // namespace Fluxions

#endif
