#include "fluxions_ssg_pch.hpp"
#include <fluxions_stdcxx.hpp>
#include <fluxions_ssphh_utilities.hpp>

namespace Fluxions {
	bool SaveSphlOBJ(const std::string& path,
					 const std::string& name,
					 const Color3f Kd,
					 std::vector<Vector3f>& verts,
					 std::vector<Vector3ui>& triangles) {
		std::vector<Vector3f> normals;
		normals.resize(verts.size());
		for (auto& n : normals) {
			n.reset();
		}
		for (auto& tri : triangles) {
			Vector3f a = verts[tri.y] - verts[tri.x];
			Vector3f b = verts[tri.z] - verts[tri.x];
			Vector3f c = a.cross(b).normalize();
			normals[tri.x] += c;
			normals[tri.y] += c;
			normals[tri.z] += c;
		}
		std::ofstream fout;
		fout.open(path + "/" + name + ".obj");
		fout << "mtllib " << name << ".mtl\n";
		// output vertices
		for (const Vector3f& v : verts) {
			fout << "v " << v.x << " " << v.y << " " << v.z << "\n";
		}
		fout << "\n";
		for (Vector3f& n : normals) {
			n.normalize();
			fout << "vn " << n.x << " " << n.y << " " << n.z << "\n";
		}
		fout << "\n\ng " << name << "\n";
		fout << "usemtl " << name << "\n";
		// output faces
		for (const Vector3ui& t : triangles) {
			fout << "f ";
			fout << (t.x + 1) << "//" << (t.x + 1) << " ";
			fout << (t.y + 1) << "//" << (t.y + 1) << " ";
			fout << (t.z + 1) << "//" << (t.z + 1) << "\n";
		}
		fout.close();

		// Write a material
		fout.open(path + "/" + name + ".mtl");
		fout << "\nnewmtl " << name << "\n\n";
		fout << "    Kd " << Kd.r << " " << Kd.g << " " << Kd.b << "\n";
		fout << "    Ks 0.1 0.1 0.1\n";
		fout << "    Ns 1.0\n";
		fout << "    Tr 1.0";
		fout << "    d 0.0";
		fout.close();
		return true;
	} // SaveSphlOBJ

	std::string MakeREFName(const std::string& prefix, bool isCubeMap, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit) {
		std::ostringstream ostr;

		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "REF";
		if (isCubeMap)
			ostr << "_cubemap";
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else {
			ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	} // MakeREFName

	std::string MakeVIZName(const std::string& prefix, int srcLightIndex, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit) {
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "VIZ" << std::setfill('0') << std::setw(2) << srcLightIndex + 1 << std::setw(2) << recvLightIndex + 1;
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else {
			ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	} // MakeVIZName

	std::string MakeGENName(const std::string& prefix, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit) {
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "GEN" << std::setfill('0') << std::setw(2) << recvLightIndex + 1;
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else {
			if (MaxRayDepth >= 0)
				ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			if (PassLimit >= 0)
				ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	} // MakeGENName

	std::string MakeHIERName(const std::string& prefix, int sendLightIndex, int MaxDegrees) {
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "HIER" << std::setfill('0') << std::setw(2) << sendLightIndex + 1;
		if (MaxDegrees >= 0)
			ostr << "_" << MaxDegrees;
		return ostr.str();
	} // MakeHIERName
} // namespace Fluxions
