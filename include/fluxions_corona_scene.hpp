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
#ifndef FLUXIONS_CORONA_SCENE_HPP
#define FLUXIONS_CORONA_SCENE_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{
class CoronaSceneFile
{
  public:
	CoronaSceneFile();
	~CoronaSceneFile();

	/// SetCameraType(newCameraType)
	/// newCameraType = { "perspective" | "cubemap" }
	void SetCameraType(const string &newCameraType);
	string GetCameraType() const { return cameraType; }

	void SetPerspectiveCamera(const Vector3f &origin, const Vector3f &target, const Vector3f &roll, const float horizontalFieldOfViewInDegrees);
	void SetCubeMapCamera(const Vector3f &origin, const Vector3f &target, const Vector3f &roll);

	void WriteSCN(const string &filename, const SimpleSceneGraph &ssg);
	void WriteCubeMapSCN(const string &filename, const SimpleSceneGraph &ssg);
	void WriteCubeMapSCN(const string &filename, const SimpleSceneGraph &ssg, const Vector3f &cameraPosition);
	void WriteSkySCN(const string &filename, const SimpleSceneGraph &ssg);
	bool WriteSphlVizSCN(const string &filename, const SimpleSceneGraph &ssg, int sourceLightIndex, int receivingLightIndex);

	void ClearCache();
	void WriteCache(const SimpleSceneGraph &ssg);
	void WriteMaterials(const SimpleSceneGraph &ssg, bool enableKs);

	bool enableKs = false;

  private:
	// cache
	vector<tuple<string, string, Matrix4f>> geometryGroups;

	string cameraType = "perspective";
	Vector3f cameraOrigin;
	Vector3f cameraTarget;
	Vector3f cameraRoll;
	Matrix4f cameraMatrix;
	float cameraHorizontalFieldOfViewInDegrees;

	void writeCamera(ostream &ostr);
	void writeSun(ostream &ostr, const SimpleSceneGraph &ssg);
	void writeGeometryGroups(ostream &ostr, const SimpleSceneGraph &ssg);
};

class CoronaJob
{
  public:
	enum class Type
	{
		REF,
		REF_CubeMap,
		Sky,
		VIZ,
		GEN
	};

	CoronaJob() {}
	CoronaJob(const string &name, Type jobtype, int arg1 = 0, int arg2 = 0);
	~CoronaJob();

	void EnableHQ() { isHQ = true; }
	void DisableHQ() { isHQ = false; }
	inline bool IsHQ() const { return isHQ; }

	inline void SetMaxRayDepth(int depth) { maxRayDepth = clamp(depth, 1, 50); }
	inline void SetPassLimit(int limit) { passLimit = clamp(limit, 1, 100); }

	inline void SetIgnoreCache(bool bState) { ignoreCache = bState; }

	void EnableHDR() { isHDR = true; }
	void DisableHDR() { isHDR = false; }
	inline bool IsHDR() const { return isHDR; }

	inline bool IsReady() const { return state == State::Ready; }
	inline bool IsRunning() const { return state == State::Running; }
	inline bool IsFinished() const { return state == State::Finished; }

	inline bool IsREF() const { return type == Type::REF; }
	inline bool IsREF_CubeMap() const { return type == Type::REF_CubeMap; }
	inline bool IsSky() const { return type == Type::Sky; }
	inline bool IsGEN() const { return type == Type::GEN; }
	inline bool IsVIZ() const { return type == Type::VIZ; }

	inline void SetImageDimensions(int w, int h)
	{
		imageWidth = clamp(w, 0, 8192);
		imageHeight = clamp(h, 0, 8192);
	}

	inline double GetElapsedTime() const { return elapsedTime; }

	inline const string &GetOutputPath() const
	{
		if (isHQ)
			return hq_output_path_ppm;
		else
			return output_path_ppm;
	}
	inline const string &GetName() const { return scene_name; }

	inline int GetGENLightIndex() const
	{
		if (IsGEN())
			return recvLight;
		return -1;
	}
	inline int GetVIZSendLightIndex() const
	{
		if (IsVIZ())
			return sendLight;
		return -1;
	}
	inline int GetVIZRecvLightIndex() const
	{
		if (IsVIZ())
			return recvLight;
		return -1;
	}

	void Start(CoronaSceneFile &coronaScene, SimpleSceneGraph &ssg);
	void CopySPH(const Sph4f &sph);
	void CopySPHToSph4f(Sph4f &sph);
	const int GetCoronaRetval() const { return lastCoronaRetval; }
	const int GetConvertRetval() const { return lastConvertRetval; }

	string MakeCoronaCommandLine();
	string MakeConvertCommandLine();

	static string MakeREFName(const string &prefix, bool isCubeMap, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	static string MakeVIZName(const string &prefix, int srcLightIndex, int recvLightIndex, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	static string MakeGENName(const string &prefix, int recvLightIndex, bool isHDR = false, bool isHQ = false, bool ks = false, int MaxRayDepth = 5, int PassLimit = 1);
	static string MakeHIERName(const string &prefix, int sendLightIndex, int MaxDegrees);

	inline bool IsJobFinished() const { return finished; }
	inline void MarkJobFinished() { finished = true; }
	inline void MarkJobUnfinished()
	{
		finished = false;
		working = false;
	}
	inline bool IsJobWorking() const { return working; }
	inline void MarkJobWorking() { working = true; }

	string ToString() const;
	void FromString(const string &str);

  private:
	enum class State
	{
		Error = -1,
		Ready = 0,
		Running,
		Finished,
	};
	State state = State::Ready;
	Type type = Type::REF;
	string scene_name;
	string scene_path;
	string output_path_exr;
	string output_path_ppm; // the ppm is created using ImageMagick's convert command
	string output_path_png;
	string hq_output_path_exr;
	string hq_output_path_ppm; // the ppm is created using ImageMagick's convert command
	string conf_path;
	string hq_conf_path;

	float sph[121 * 4];

	int imageWidth = 1280;
	int imageHeight = 720;
	int maxRayDepth = 5;
	int passLimit = 1;
	bool ignoreCache = false;

	double elapsedTime;
	bool finished = false;
	bool working = false;

	bool isHQ = false;
	bool isHDR = false;

	// If GEN, then recvLight is the
	int sendLight = -1;
	int recvLight = -1;

	int lastCoronaRetval = 0;
	int lastConvertRetval = 0;

	bool Run();
};
} // namespace Fluxions

#endif
