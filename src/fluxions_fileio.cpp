// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#include "pch.hpp"
#include <iostream>
#include <fluxions_fileio.hpp>
#include <fluxions_types.hpp>

#ifdef __APPLE__
#define __unix__ 1
#endif

#ifdef _WIN32
#include <Windows.h>
#elif __unix__
#include <libgen.h> // for dirname() and basename()
#include <unistd.h> // for realpath()
#endif

#include <memory.h> // for free()
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <regex>
#include <string>
#include <vector>

namespace Fluxions
{
#ifdef _WIN32
const int bitIsDirectory = _S_IFDIR;
const int bitIsRegularFile = _S_IFREG;
#elif __unix__
const int bitIsDirectory = S_IFDIR;
const int bitIsRegularFile = S_IFREG;
#endif

// const std::string BlankString;

#ifdef __unix__
#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif
#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif
#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif
#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif
#endif

int stat_with_errno(const std::string &path, void *Stat)
{
#ifdef _WIN32
    _set_errno(0);
#elif __unix__
    errno = 0;
#endif

#ifdef _WIN32
    return _stat(path.c_str(), (struct _stat *)Stat);
#elif __unix__
    return stat(path.c_str(), (struct stat *)Stat);
#endif
}

FilePathInfo::FilePathInfo()
{
}

FilePathInfo::FilePathInfo(const std::string &filename)
{
    Set(filename);
}

void FilePathInfo::Clear()
{
    pathType = PathType::DoesNotExist;
    atime = 0;
    ctime = 0;
    dir.clear();
    origpath.clear();
    path.clear();
    fullfname.clear();
    fname.clear();
    ext.clear();
    relativePath = false;
}

void FilePathInfo::Set(const std::string &_path)
{
    Clear();

    std::regex path_replace("[/\\\\]+");
    std::string p = std::regex_replace(_path, path_replace, "/");
    origpath = p;

    if (origpath[0] == '.' && origpath[1] == '/') relativePath = true;
    if (origpath[0] == '.' && origpath[1] == '.' && origpath[2] == '/') relativePath = true;

    // updated to use realpath on POSIX
#ifdef __unix__
    fname = "";
    dir = "";
    ext = "";
    path = "";
    fullfname = "";

    path = getFullPathName(origpath);
    char *pathcopy = new char[path.size() + 1];
    memcpy(pathcopy, &path[0], path.size());
    char *dirnamePtr = dirname(pathcopy);
    if (dirnamePtr)
    {
        dir = dirnamePtr;
    }
    else
    {
        // check errno for error
        dir = getCurrentDirectory();
    }
    delete[] pathcopy;
    pathcopy = new char[path.size() + 1];
    memcpy(pathcopy, &path[0], path.size());
    char *basenamePtr = basename(pathcopy);
    if (basenamePtr)
    {
        fullfname = basenamePtr;
    }
    else
    {
        // check errno for error
        fullfname = path;
    }
    delete[] pathcopy;

    std::string::size_type idx = fullfname.find_last_of(".");
    if (idx != std::string::npos)
    {
        ext = fullfname.substr(idx + 1);
        fname = fullfname.substr(0, idx);
    }
    else
    {
        ext = "";
        fname = fullfname;
    }
#elif defined(WIN32)
    char driveStr[_MAX_DRIVE] = {0};
    char dirStr[_MAX_DIR] = {0};
    char fnameStr[_MAX_FNAME] = {0};
    char extStr[_MAX_EXT] = {0};

    path = getFullPathName(origpath);

    _splitpath_s(path.c_str(), driveStr, _MAX_DRIVE, dirStr, _MAX_DIR, fnameStr, _MAX_FNAME, extStr, _MAX_EXT);

    std::string drive = driveStr;
    dir = dirStr;
    std::string testpath = drive + dir;
    dir = regex_replace(testpath, path_replace, "/");
    fname = fnameStr;
    ext = extStr;
    fullfname = fname + ext;

    if (dir.empty())
    {
        testpath = getCurrentDirectory();
        dir = regex_replace(testpath, path_replace, "/");
    }

    if (!dir.empty())
    {
        char backChar = dir.back();

        if (backChar != '/' && backChar != '\\')
        {
            dir += "/";
        }
    }

    fullfname = fname + ext;
    path = dir + fullfname;
#endif
    fill_stat_info();
}

std::string FilePathInfo::getFullPathName(const std::string &filename)
{
    std::string outputStr;
#ifdef __unix__
    errno = 0;
    char *pathstr = realpath(filename.c_str(), NULL);
    if (pathstr != NULL)
    {
        outputStr = pathstr;
        free(pathstr);
    }
    else
    {
        // check errno
        // default behavior is to return the filename (it doesn't exist, but that's okay)
        outputStr = filename;
    }
#elif defined(WIN32)
    char pathStr[4096] = {0};
    char **lppPart = {NULL};

    ::GetFullPathNameA(filename.c_str(), 4096, pathStr, lppPart);

    outputStr = pathStr;
#endif
    std::regex path_replace("[/\\\\]+");
    std::string p = std::regex_replace(outputStr, path_replace, "/");

    return p;
}

std::string FilePathInfo::getCurrentDirectory()
{
    std::string output;
#ifdef __unix__
    char *buffer;
    buffer = getcwd(NULL, 0);
    if (buffer == NULL)
    {
        // error
    }
    else
    {
        output = buffer;
        free(buffer);
    }
#elif defined(WIN32)
    char dirStr[_MAX_DIR + 1];
    DWORD result = ::GetCurrentDirectoryA(_MAX_DIR, dirStr);
    if (result > 0)
    {
        output = dirStr;
    }
    else
    {
        output = "";
    }
#endif
    std::regex path_replace("[/\\\\]+");
    std::string p = std::regex_replace(output, path_replace, "/");

    return p;
}

bool FilePathInfo::TestIfFileExists(const std::string &filename)
{
#ifdef _WIN32
    struct _stat Stat;
#elif __unix__
    struct stat Stat;
#endif
    int retval = stat_with_errno(filename.c_str(), &Stat);
    if (retval == 0)
        return true;
    else
        return false;
}

bool FilePathInfo::FindFileIfExists(const std::vector<std::string> &pathsToTry, std::string &output)
{
    // Is there a file name to test?
    if (path.empty())
        return false;

    if (TestIfFileExists(path))
    {
        return true;
    }
    else
    {
        for (auto testPathIt : pathsToTry)
        {
            char backChar = testPathIt.back();
            std::string testPath = testPathIt;
            if (backChar != '/' && backChar != '\\')
                testPath = testPath + std::string("/") + fullfname;
            else
                testPath = testPath + fullfname;

            if (TestIfFileExists(testPath))
            {
                output = testPath;
                break;
            }
            else
            {
                // An error has occurred. The file does not exist.
                return false;
            }
        }
    }

    return true;
}

void FilePathInfo::fill_stat_info()
{
    if (path.empty())
    {
        pathType = PathType::DoesNotExist;
        atime = 0;
        ctime = 0;
        return;
    }
    std::string testpath;
    if (path.back() == '/' || path.back() == '\\')
        testpath = path.substr(0, path.size() - 1);
    else
        testpath = path;

#ifdef _WIN32
    struct _stat Stat;
#define S_IF
#elif __unix__
    struct stat Stat;
#endif
    int retval = stat_with_errno(testpath.c_str(), &Stat);

    if (retval != 0)
    {
        pathType = PathType::DoesNotExist;
    }
    else
    {
        if (Stat.st_mode & bitIsDirectory)
        {
            pathType = PathType::Directory;
            // we need to reset dir because it would contain the parent directory
            dir = testpath;
        }
        else if (Stat.st_mode & bitIsRegularFile)
        {
            pathType = PathType::File;
        }
        else
        {
            pathType = PathType::Other;
        }
        ctime = Stat.st_ctime;
        atime = Stat.st_atime;
    }
}

bool FilePathInfo::DoesNotExist() const
{
    return pathType == PathType::DoesNotExist;
}

bool FilePathInfo::Exists() const
{
    return pathType != PathType::DoesNotExist;
}

bool FilePathInfo::IsDirectory() const {
    return pathType == PathType::Directory;
}

bool FilePathInfo::IsFile() const {
    return pathType == PathType::File;
}

bool FilePathInfo::IsOther() const
{
    return pathType == PathType::Other;
}

bool FilePathInfo::IsRelative() const
{
    return relativePath == true; 
}

std::string ReadTextFile(const std::string &filename)
{
    std::ifstream fin(filename.c_str());

    if (!fin)
        return "";

    std::stringstream sstr;
    sstr << fin.rdbuf();
    fin.close();
    return sstr.str();
    // std::string str;
    // fin.seekg(0, std::ios::end);
    // size_t size = (size_t)fin.tellg();
    // str.resize(size);
    // fin.seekg(0, std::ios::beg);
    // fin.read(&str[0], size);

    // fin.close();
    // return str;
}

std::vector<FXubyte> ReadBinaryFile(const std::string &filename)
{
    std::vector<FXubyte> buffer;
    std::ifstream fin(filename.c_str(), std::ios::binary);

    if (!fin)
        return buffer;

    std::string str;
    fin.seekg(0, std::ios::end);
    size_t size = (size_t)fin.tellg();
    buffer.resize(size);
    fin.seekg(0, std::ios::beg);
    fin.read((char *)&buffer[0], size);

    fin.close();
    return buffer;
}

std::string FindPathIfExists(const std::string &path, const std::vector<std::string> pathsToTry)
{
    std::string output;

    // Is there a file name to test?
    FilePathInfo fpi(path);
    if (fpi.fullfname.empty())
        return output;

    if (TestIfFileExists(path))
    {
        output = path;
    }
    else
    {
        for (auto testPathIt : pathsToTry)
        {
            std::string testPath = testPathIt + "/" + fpi.fullfname;
            if (TestIfFileExists(testPath))
            {
                output = testPath;
                break;
            }
        }
    }

    return output;
}

std::string NormalizePathName(const std::string &basepath, const std::string &path)
{
	static std::string BlankString;
	BlankString = "";

    FilePathInfo p1(basepath + "/" + path);
    FilePathInfo p2(path);

    if (p1.pathType == PathType::DoesNotExist)
    {
        if (p2.pathType == PathType::DoesNotExist)
        {
            return BlankString;
        }
        else
        {
            return p2.origpath;
        }
    }
    else
    {
        return p1.origpath;
    }
}

PathType GetPathType(const std::string &path)
{
#ifdef _WIN32
    struct _stat Stat;
#elif __unix__
    struct stat Stat;
#endif
    int retval = stat_with_errno(path, &Stat);
    if (retval != 0)
    {
        // do not print errors...
        return PathType::DoesNotExist;
    }
    if (Stat.st_mode & bitIsDirectory)
        return PathType::Directory;
    else if (Stat.st_mode & bitIsRegularFile)
        return PathType::File;
    return PathType::Other;
}

TimeValue GetPathCreationTime(const std::string &path)
{
#ifdef _WIN32
    struct _stat Stat;
#elif __unix__
    struct stat Stat;
#endif

    int retval = stat_with_errno(path, &Stat);

    if (retval != 0)
    {
        switch (errno)
        {
        case ENOENT:
            std::cerr << __func__ << ": file not found" << std::endl;
            break;
        case EINVAL:
            std::cerr << __func__ << ": invalid parameter to _stat()" << std::endl;
            break;
        default:
            std::cerr << __func__ << ": unknown error in _stat()" << std::endl;
        }
    }
    else
    {
        return Stat.st_ctime;
    }
    return 0;
}

TimeValue GetPathAccessTime(const std::string &path)
{
#ifdef _WIN32
    struct _stat Stat;
#elif __unix__
    struct stat Stat;
#endif
    int retval = stat_with_errno(path, &Stat);

    if (retval != 0)
    {
        switch (errno)
        {
        case ENOENT:
            std::cerr << __func__ << ": file not found" << std::endl;
            break;
        case EINVAL:
            std::cerr << __func__ << ": invalid parameter to _stat()" << std::endl;
            break;
        default:
            std::cerr << __func__ << ": unknown error in _stat()" << std::endl;
        }
    }
    else
    {
        return Stat.st_atime;
    }
    return 0;
}

// Data type IOSTREAM Utilities
bool ReadBool(std::istream &istr)
{
    std::string str;
    istr >> str;
    if (str == "true")
        return true;
    return false;
}

int ReadInt(std::istream &istr)
{
    int ival;
    istr >> ival;
    return ival;
}

long long ReadInt64(std::istream &istr)
{
    long long ival;
    istr >> ival;
    return ival;
}

float ReadFloat(std::istream &istr)
{
    double fval;
    istr >> fval;
    return (float)fval;
}

double ReadDouble(std::istream &istr)
{
    double fval;
    istr >> fval;
    return fval;
}

std::string ReadString(std::istream &istr)
{
    std::string str;
    char c;
    char lastC = 0;
    bool isQuotes = false;

    istr >> std::ws;
    while (1)
    {
        c = 0;
        istr >> std::noskipws >> c;
        istr >> std::skipws;
        if (!isQuotes && str.empty() && c == '\"')
        {
            isQuotes = true;
        }
        else if (isQuotes && c == '\"' && lastC != '\\')
        {
            return str;
        }
        else if (c == '\"' && lastC == '\\')
        {
            str.back() = c;
        }
        else if (!isQuotes && iswspace(c))
        {
            return str;
        }
        else
        {
            if (c != 0)
                str += c;
            else
                break;
        }
        lastC = c;
        if (!istr)
            break;
    }
    return str;
}

Vector2f ReadVector2f(std::istream &istr)
{
    double x, y;
    istr >> x;
    istr >> y;
    return Vector2f((float)x, (float)y);
}

Vector2d ReadVector2d(std::istream &istr)
{
    double x, y;
    istr >> x;
    istr >> y;
    return Vector2d(x, y);
}

Vector3f ReadVector3f(std::istream &istr)
{
    double x, y, z;
    istr >> x;
    istr >> y;
    istr >> z;
    return Vector3f((float)x, (float)y, (float)z);
}

Vector3d ReadVector3d(std::istream &istr)
{
    double x, y, z;
    istr >> x;
    istr >> y;
    istr >> z;
    return Vector3d(x, y, z);
}

Vector4f ReadVector4f(std::istream &istr)
{
    double x, y, z, w;
    istr >> x;
    istr >> y;
    istr >> z;
    istr >> w;
    return Vector4f((float)x, (float)y, (float)z, (float)w);
}

Vector4d ReadVector4d(std::istream &istr)
{
    double x, y, z, w;
    istr >> x;
    istr >> y;
    istr >> z;
    istr >> w;
    return Vector4d(x, y, z, w);
}

Color3f ReadColor3f(std::istream &istr)
{
    double r, g, b;
    istr >> r;
    istr >> g;
    istr >> b;
    return Color3f((float)r, (float)g, (float)b);
}

Color3d ReadColor3d(std::istream &istr)
{
    double r, g, b;
    istr >> r;
    istr >> g;
    istr >> b;
    return Color3d(r, g, b);
}

Color4f ReadColor4f(std::istream &istr)
{
    double r, g, b, a;
    istr >> r;
    istr >> g;
    istr >> b;
    istr >> a;
    return Color4f((float)r, (float)g, (float)b, (float)a);
}

Color4d ReadColor4d(std::istream &istr)
{
    double r, g, b, a;
    istr >> r;
    istr >> g;
    istr >> b;
    istr >> a;
    return Color4d(r, g, b, a);
}

Quaternionf ReadQuaternionf(std::istream &istr)
{
    double a, b, c, d;
    istr >> a;
    istr >> b;
    istr >> c;
    istr >> d;
    return Quaternionf((float)a, (float)b, (float)c, (float)d);
}

Quaterniond ReadQuaterniond(std::istream &istr)
{
    double a, b, c, d;
    istr >> a;
    istr >> b;
    istr >> c;
    istr >> d;
    return Quaterniond(a, b, c, d);
}

Matrix4f ReadMatrix4f(std::istream &istr)
{
    Matrix4f m;
    m.LoadIdentity();
    istr >> m.m11;
    istr >> m.m12;
    istr >> m.m13;
    istr >> m.m14;
    istr >> m.m21;
    istr >> m.m22;
    istr >> m.m23;
    istr >> m.m24;
    istr >> m.m31;
    istr >> m.m32;
    istr >> m.m33;
    istr >> m.m34;
    istr >> m.m41;
    istr >> m.m42;
    istr >> m.m43;
    istr >> m.m44;
    return m;
}

Matrix4d ReadMatrix4d(std::istream &istr)
{
    Matrix4d m;
    m.LoadIdentity();
    istr >> m.m11;
    istr >> m.m12;
    istr >> m.m13;
    istr >> m.m14;
    istr >> m.m21;
    istr >> m.m22;
    istr >> m.m23;
    istr >> m.m24;
    istr >> m.m31;
    istr >> m.m32;
    istr >> m.m33;
    istr >> m.m34;
    istr >> m.m41;
    istr >> m.m42;
    istr >> m.m43;
    istr >> m.m44;
    return m;
}

Matrix4f ReadAffineMatrix4f(std::istream &istr)
{
    Matrix4f m;
    m.LoadIdentity();
    istr >> m.m11;
    istr >> m.m12;
    istr >> m.m13;
    istr >> m.m14;
    istr >> m.m21;
    istr >> m.m22;
    istr >> m.m23;
    istr >> m.m24;
    istr >> m.m31;
    istr >> m.m32;
    istr >> m.m33;
    istr >> m.m34;
    m.m41 = 0.0f;
    m.m42 = 0.0f;
    m.m43 = 0.0f;
    m.m44 = 1.0f;
    return m;
}

Matrix4d ReadAffineMatrix4d(std::istream &istr)
{
    Matrix4d m;
    m.LoadIdentity();
    istr >> m.m11;
    istr >> m.m12;
    istr >> m.m13;
    istr >> m.m14;
    istr >> m.m21;
    istr >> m.m22;
    istr >> m.m23;
    istr >> m.m24;
    istr >> m.m31;
    istr >> m.m32;
    istr >> m.m33;
    istr >> m.m34;
    m.m41 = 0.0;
    m.m42 = 0.0;
    m.m43 = 0.0;
    m.m44 = 1.0;
    return m;
}

SphericalHarmonicf ReadSphericalHarmonicf(std::istream &istr)
{
    SphericalHarmonicf sph;
    int maxDegree = ReadInt(istr);
    if (maxDegree >= 0 && maxDegree <= 10)
    {
        sph.resize(maxDegree);
        for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
        {
            sph[i] = ReadFloat(istr);
        }
    }
    else
    {
        std::cerr << __FUNCTION__ << "(): invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10" << std::endl;
    }
    return sph;
}

SphericalHarmonicd ReadSphericalHarmonicd(std::istream &istr)
{
    SphericalHarmonicd sph;
    int maxDegree = ReadInt(istr);
    if (maxDegree >= 0 && maxDegree <= 10)
    {
        sph.resize(maxDegree);
        for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
        {
            sph[i] = ReadFloat(istr);
        }
    }
    else
    {
        std::cerr << __FUNCTION__ << "(): invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10" << std::endl;
    }
    return sph;
}

// WRITING ROUTINES

std::ostream &WriteBool(std::ostream &ostr, bool val)
{
    if (val == true)
        ostr << "true ";
    else
        ostr << "false ";
    return ostr;
}

std::ostream &WriteInt(std::ostream &ostr, int val)
{
    return ostr << val << " ";
}

std::ostream &WriteInt64(std::ostream &ostr, long long val)
{
    return ostr << val << " ";
}

std::ostream &WriteFloat(std::ostream &ostr, float val)
{
    return ostr << val << " ";
}

std::ostream &WriteDouble(std::ostream &ostr, double val)
{
    return ostr << val << " ";
}

std::ostream &WriteString(std::ostream &ostr, const std::string &str)
{
    ostr << "\"";
    for (auto it = str.begin(); it != str.end(); it++)
    {
        char c = *it;
        if (c == '\"')
            ostr << '\\' << '\"';
        else
            ostr << c;
    }
    ostr << "\" ";
    return ostr;
}

std::ostream &WriteVector2f(std::ostream &ostr, const Vector2f &v)
{
    return ostr << v.x << " " << v.y << " ";
}

std::ostream &WriteVector2d(std::ostream &ostr, const Vector2d &v)
{
    return ostr << v.x << " " << v.y << " ";
}

std::ostream &WriteVector3f(std::ostream &ostr, const Vector3f &v)
{
    return ostr << v.x << " " << v.y << " " << v.z << " ";
}

std::ostream &WriteVector3d(std::ostream &ostr, const Vector3d &v)
{
    return ostr << v.x << " " << v.y << " " << v.z << " ";
}

std::ostream &WriteVector4f(std::ostream &ostr, const Vector4f &v)
{
    return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
}

std::ostream &WriteVector4d(std::ostream &ostr, const Vector4d &v)
{
    return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
}

std::ostream &WriteColor3f(std::ostream &ostr, const Color3f &v)
{
    return ostr << v.r << " " << v.g << " " << v.b << " ";
}

std::ostream &WriteColor3d(std::ostream &ostr, const Color3d &v)
{
    return ostr << v.r << " " << v.g << " " << v.b << " ";
}

std::ostream &WriteColor4f(std::ostream &ostr, const Color4f &v)
{
    return ostr << v.r << " " << v.g << " " << v.b << " " << v.a << " ";
}

std::ostream &WriteColor4d(std::ostream &ostr, const Color4d &v)
{
    return ostr << v.r << " " << v.g << " " << v.b << " " << v.a << " ";
}

std::ostream &WriteQuaternionf(std::ostream &ostr, const Quaternionf &q)
{
    return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
}

std::ostream &WriteQuaterniond(std::ostream &ostr, const Quaterniond &q)
{
    return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
}

std::ostream &WriteMatrix4f(std::ostream &ostr, const Matrix4f &m)
{
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
    return ostr;
}

std::ostream &WriteMatrix4d(std::ostream &ostr, const Matrix4d &m)
{
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
    return ostr;
}

std::ostream &WriteAffineMatrix4f(std::ostream &ostr, const Matrix4f &m)
{
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    return ostr;
}

std::ostream &WriteAffineMatrix4d(std::ostream &ostr, const Matrix4d &m)
{
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    return ostr;
}

std::ostream &WriteSphericalHarmonicf(std::ostream &ostr, const SphericalHarmonicf &sph)
{
    ostr << sph.GetMaxDegree() << " ";
    for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
    {
        ostr << sph[i] << " ";
    }
    return ostr;
}

std::ostream &WriteSphericalHarmonicd(std::ostream &ostr, const SphericalHarmonicd &sph)
{
    ostr << sph.GetMaxDegree() << " ";
    for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
    {
        ostr << sph[i] << " ";
    }
    return ostr;
}
} // namespace Fluxions
