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
#ifndef FLUXIONS_FILEIO_HPP
#define FLUXIONS_FILEIO_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_types.hpp>
#include <fluxions_gte.hpp>

namespace Fluxions
{
enum class PathType
{
	DoesNotExist,
	Directory,
	File,
	Other
};

string ReadTextFile(const string &filename);

#ifdef _TIME64_T_DEFINED
using TimeValue = __time64_t;
#else
using TimeValue = long long;
#endif

struct FilePathInfo
{
  private:
	/// <summary>The drive letter for this file. Not applicable for POSIX.</summary>
	// string drive;
  public:
	/// <summary>Original path string for this structure</summary>
	string origpath;
	/// <summary>The directory for this file with final slash.</summary>
	string dir;
	/// <summary>The full filename with extension.</summary>
	string fullfname;
	/// <summary>The filename minus the final extension.</summary>
	string fname;
	/// <summary>The extension from the filename.</summary>
	string ext;
	/// <summary>The full path name for the filename.</summary>
	string path;

	PathType pathType = PathType::DoesNotExist;
	TimeValue atime;
	TimeValue ctime;

	FilePathInfo();
	FilePathInfo(const string &filename);
	void Clear();
	void Set(const string &filename);
	string getFullPathName(const string &filename);
	string getCurrentDirectory();
	bool TestIfFileExists(const string &filename);
	string FindFileIfExists(const vector<string> &pathsToTry);
	void fill_stat_info();
	bool DoesNotExist() const { return pathType == PathType::DoesNotExist; }
	bool Exists() const { return pathType != PathType::DoesNotExist; }
	bool IsDirectory() const { return pathType == PathType::Directory; }
	bool IsFile() const { return pathType == PathType::File; }
};

//inline string GetFileDrive(const string &filename) { FilePathInfo fpi(filename); return fpi.drive; }
inline string GetFileDir(const string &path)
{
	FilePathInfo fpi(path);
	return fpi.dir;
}
inline string GetFileNameWithExtension(const string &path)
{
	FilePathInfo fpi(path);
	return fpi.fullfname;
}
inline string GetFileName(const string &path)
{
	FilePathInfo fpi(path);
	return fpi.fname;
}
inline string GetFileExtension(const string &path)
{
	FilePathInfo fpi(path);
	return fpi.ext;
}

inline bool TestIfFileExists(const string &filename)
{
	ifstream fin(filename.c_str(), ios::binary);
	if (!fin)
		return false;
	fin.close();
	return true;
}

string FindPathIfExists(const string &path, const vector<string> pathsToTry);
string NormalizePathName(const string &basepath, const string &path);
inline string NormalizePathName(const string &path) { return NormalizePathName("", path); }
PathType GetPathType(const string &path);
TimeValue GetPathAccessTime(const string &path);
TimeValue GetPathCreationTime(const string &path);
inline bool IsPathDirectory(const string &fname) { return GetPathType(fname) == PathType::Directory; }
inline bool IsPathFile(const string &fname) { return GetPathType(fname) == PathType::File; }
inline bool IsPathValid(const string &path)
{
	auto pt = GetPathType(path);
	return pt == PathType::Directory || pt == PathType::File;
}
inline bool IsPathWeird(const string &fname) { return GetPathType(fname) == PathType::Other; }

bool ReadBool(istream &istr);
int ReadInt(istream &istr);
long long ReadInt64(istream &istr);
float ReadFloat(istream &istr);
double ReadDouble(istream &istr);
string ReadString(istream &istr);
Vector2f ReadVector2f(istream &istr);
Vector2d ReadVector2d(istream &istr);
Vector3f ReadVector3f(istream &istr);
Vector3d ReadVector3d(istream &istr);
Vector4f ReadVector4f(istream &istr);
Vector4d ReadVector4d(istream &istr);
Quaternionf ReadQuaternionf(istream &istr);
Quaterniond ReadQuaterniond(istream &istr);
Matrix4f ReadMatrix4f(istream &istr);
Matrix4d ReadMatrix4d(istream &istr);
Matrix4f ReadAffineMatrix4f(istream &istr);
Matrix4d ReadAffineMatrix4d(istream &istr);
SphericalHarmonicf ReadSphericalHarmonicf(istream &istr);
SphericalHarmonicd ReadSphericalHarmonicd(istream &istr);

ostream &WriteBool(ostream &ostr, bool val);
ostream &WriteInt(ostream &ostr, int val);
ostream &WriteInt64(ostream &ostr, long long val);
ostream &WriteFloat(ostream &ostr, float val);
ostream &WriteDouble(ostream &ostr, double val);
ostream &WriteString(ostream &ostr, const string &str);
ostream &WriteVector2f(ostream &ostr, const Vector2f &v);
ostream &WriteVector2d(ostream &ostr, const Vector2d &v);
ostream &WriteVector3f(ostream &ostr, const Vector3f &v);
ostream &WriteVector3d(ostream &ostr, const Vector3d &v);
ostream &WriteVector4f(ostream &ostr, const Vector4f &v);
ostream &WriteVector4d(ostream &ostr, const Vector4d &v);
ostream &WriteQuaternionf(ostream &ostr, const Quaternionf &q);
ostream &WriteQuaterniond(ostream &ostr, const Quaterniond &q);
ostream &WriteMatrix4f(ostream &ostr, const Matrix4f &m);
ostream &WriteMatrix4d(ostream &ostr, const Matrix4d &m);
ostream &WriteAffineMatrix4f(ostream &ostr, const Matrix4f &m);
ostream &WriteAffineMatrix4d(ostream &ostr, const Matrix4d &m);
ostream &WriteSphericalHarmonicf(ostream &ostr, const SphericalHarmonicf &sph);
ostream &WriteSphericalHarmonicd(ostream &ostr, const SphericalHarmonicd &sph);
} // namespace Fluxions

#endif
