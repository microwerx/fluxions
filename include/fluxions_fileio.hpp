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

#include <fluxions_gte.hpp>
#include <fluxions_stdcxx.hpp>
#include <fluxions_types.hpp>

namespace Fluxions
{
enum class PathType
{
    DoesNotExist,
    Directory,
    File,
    Other
};

std::string ReadTextFile(const std::string &filename);

using TimeValue = time_t;

struct FilePathInfo
{
  private:
    /// <summary>The drive letter for this file. Not applicable for POSIX.</summary>
    // std::string  drive;
  public:
    /// <summary>Original path string for this structure</summary>
    std::string origpath;
    /// <summary>The directory for this file with final slash.</summary>
    std::string dir;
    /// <summary>The full filename with extension.</summary>
    std::string fullfname;
    /// <summary>The filename minus the final extension.</summary>
    std::string fname;
    /// <summary>The extension from the filename.</summary>
    std::string ext;
    /// <summary>The full path name for the filename.</summary>
    std::string path;
    bool relativePath;

    PathType pathType = PathType::DoesNotExist;
    TimeValue atime;
    TimeValue ctime;

    FilePathInfo();
    FilePathInfo(const std::string &filename);
    void Clear();
    void Set(const std::string &filename);
    std::string getFullPathName(const std::string &filename);
    std::string getCurrentDirectory();
    bool TestIfFileExists(const std::string &filename);
    bool FindFileIfExists(const std::vector<std::string> &pathsToTry, std::string &output);
    void fill_stat_info();
    bool DoesNotExist() const;
    bool Exists() const;
    bool IsDirectory() const;
    bool IsOther() const;
    bool IsFile() const;
    bool IsRelative() const;
    // inline bool DoesNotExist() const { return pathType == PathType::DoesNotExist; }
    // inline bool Exists() const { return pathType != PathType::DoesNotExist; }
    // inline bool IsDirectory() const { return pathType == PathType::Directory; }
    // inline bool IsFile() const { return pathType == PathType::File; }
    // inline bool IsOther() const { return pathType == PathType::Other; }
    // inline bool IsRelative() const { return relativePath == true; }
};

//inline std::string  GetFileDrive(const std::string  &filename) { FilePathInfo fpi(filename); return fpi.drive; }
inline std::string GetFileDir(const std::string &path)
{
    FilePathInfo fpi(path);
    return fpi.dir;
}
inline std::string GetFileNameWithExtension(const std::string &path)
{
    FilePathInfo fpi(path);
    return fpi.fullfname;
}
inline std::string GetFileName(const std::string &path)
{
    FilePathInfo fpi(path);
    return fpi.fname;
}
inline std::string GetFileExtension(const std::string &path)
{
    FilePathInfo fpi(path);
    return fpi.ext;
}

inline bool TestIfFileExists(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ios::binary);
    if (!fin)
        return false;
    fin.close();
    return true;
}

std::string FindPathIfExists(const std::string &path, const std::vector<std::string> pathsToTry);
std::string NormalizePathName(const std::string &basepath, const std::string &path);
inline std::string NormalizePathName(const std::string &path) { return NormalizePathName("", path); }
PathType GetPathType(const std::string &path);
TimeValue GetPathAccessTime(const std::string &path);
TimeValue GetPathCreationTime(const std::string &path);
inline bool IsPathDirectory(const std::string &fname) { return GetPathType(fname) == PathType::Directory; }
inline bool IsPathFile(const std::string &fname) { return GetPathType(fname) == PathType::File; }
inline bool IsPathValid(const std::string &path)
{
    auto pt = GetPathType(path);
    return pt == PathType::Directory || pt == PathType::File;
}
inline bool IsPathWeird(const std::string &fname) { return GetPathType(fname) == PathType::Other; }

bool ReadBool(std::istream &istr);
int ReadInt(std::istream &istr);
long long ReadInt64(std::istream &istr);
float ReadFloat(std::istream &istr);
double ReadDouble(std::istream &istr);
std::string ReadString(std::istream &istr);
Vector2f ReadVector2f(std::istream &istr);
Vector2d ReadVector2d(std::istream &istr);
Vector3f ReadVector3f(std::istream &istr);
Vector3d ReadVector3d(std::istream &istr);
Vector4f ReadVector4f(std::istream &istr);
Vector4d ReadVector4d(std::istream &istr);
Color3f ReadColor3f(std::istream &istr);
Color3d ReadColor3d(std::istream &istr);
Color4f ReadColor4f(std::istream &istr);
Color4d ReadColor4d(std::istream &istr);
Quaternionf ReadQuaternionf(std::istream &istr);
Quaterniond ReadQuaterniond(std::istream &istr);
Matrix4f ReadMatrix4f(std::istream &istr);
Matrix4d ReadMatrix4d(std::istream &istr);
Matrix4f ReadAffineMatrix4f(std::istream &istr);
Matrix4d ReadAffineMatrix4d(std::istream &istr);
SphericalHarmonicf ReadSphericalHarmonicf(std::istream &istr);
SphericalHarmonicd ReadSphericalHarmonicd(std::istream &istr);

std::ostream &WriteBool(std::ostream &ostr, bool val);
std::ostream &WriteInt(std::ostream &ostr, int val);
std::ostream &WriteInt64(std::ostream &ostr, long long val);
std::ostream &WriteFloat(std::ostream &ostr, float val);
std::ostream &WriteDouble(std::ostream &ostr, double val);
std::ostream &WriteString(std::ostream &ostr, const std::string &str);
std::ostream &WriteVector2f(std::ostream &ostr, const Vector2f &v);
std::ostream &WriteVector2d(std::ostream &ostr, const Vector2d &v);
std::ostream &WriteVector3f(std::ostream &ostr, const Vector3f &v);
std::ostream &WriteVector3d(std::ostream &ostr, const Vector3d &v);
std::ostream &WriteVector4f(std::ostream &ostr, const Vector4f &v);
std::ostream &WriteVector4d(std::ostream &ostr, const Vector4d &v);
std::ostream &WriteColor3f(std::ostream &ostr, const Color3f &v);
std::ostream &WriteColor3d(std::ostream &ostr, const Color3d &v);
std::ostream &WriteColor4f(std::ostream &ostr, const Color4f &v);
std::ostream &WriteColor4d(std::ostream &ostr, const Color4d &v);
std::ostream &WriteQuaternionf(std::ostream &ostr, const Quaternionf &q);
std::ostream &WriteQuaterniond(std::ostream &ostr, const Quaterniond &q);
std::ostream &WriteMatrix4f(std::ostream &ostr, const Matrix4f &m);
std::ostream &WriteMatrix4d(std::ostream &ostr, const Matrix4d &m);
std::ostream &WriteAffineMatrix4f(std::ostream &ostr, const Matrix4f &m);
std::ostream &WriteAffineMatrix4d(std::ostream &ostr, const Matrix4d &m);
std::ostream &WriteSphericalHarmonicf(std::ostream &ostr, const SphericalHarmonicf &sph);
std::ostream &WriteSphericalHarmonicd(std::ostream &ostr, const SphericalHarmonicd &sph);
} // namespace Fluxions

#endif
