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
#include "stdafx.h"
#include <fluxions_fileio.hpp>

#ifdef WIN32

#elif _POSIX_VERSION
#include <unistd.h> // for realpath()
#include <libgen.h> // for dirname() and basename()
#endif

#include <string>
#include <vector>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <memory.h> // for free()

#ifdef USING_ERRNO
#include <cerrno>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

namespace Fluxions
{
using namespace std;

// const string BlankString;

#ifdef _POSIX_VERSION
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

FilePathInfo::FilePathInfo()
{
}

FilePathInfo::FilePathInfo(const string &filename)
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
}

void FilePathInfo::Set(const string &_path)
{
	Clear();

	regex path_replace("[/\\]+", regex::awk);
	string p = regex_replace(_path, path_replace, "/");
	origpath = p;

	// updated to use realpath on POSIX
#ifdef _POSIX_VERSION
	fname = "";
	drive = "";
	dir = "";
	ext = "";
	path = "";
	folderpath = "";
	fullfname = "";

	path = getFullPathName(origpath);
	char *pathcopy = new char[path.size() + 1];
	memcpy(pathcopy, &path[0], path.size());
	char *dirnamePtr = dirname(pathcopy);
	if (dirnamePtr)
	{
		path = dirnamePtr;
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

	drive = "";
	dir = path;

	string::size_type idx = fullfname.find_last_of(".");
	if (idx != string::npos)
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

	string drive = driveStr;
	dir = dirStr;
	string testpath = drive + dir;
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

string FilePathInfo::getFullPathName(const string &filename)
{
	string outputStr;
#ifdef _POSIX_VERSION
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
	regex path_replace("[/\\]+", regex::awk);
	string p = regex_replace(outputStr, path_replace, "/");

	return p;
}

string FilePathInfo::getCurrentDirectory()
{
	string output;
#ifdef _POSIX_VERSION
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
	regex path_replace("[/\\]+", regex::awk);
	string p = regex_replace(output, path_replace, "/");

	return p;
}

bool FilePathInfo::TestIfFileExists(const string &filename)
{
	struct _stat Stat;
	int retval = _stat(filename.c_str(), &Stat);
	if (retval == 0)
		return true;
	else
		return false;
}

string FilePathInfo::FindFileIfExists(const vector<string> &pathsToTry)
{
	string output;

	// Is there a file name to test?
	if (path.empty())
		return output;

	if (TestIfFileExists(path))
	{
		return path;
	}
	else
	{
		for (auto testPathIt : pathsToTry)
		{
			char backChar = testPathIt.back();
			string testPath = testPathIt;
			if (backChar != '/' && backChar != '\\')
				testPath = testPath + string("/") + fullfname;
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
			}
		}
	}

	return output;
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
	struct _stat Stat;
#ifdef USING_ERRNO
	errno_t errno;
	_set_errno(0);
#endif
	string testpath;
	if (path.back() == '/' || path.back() == '\\')
		testpath = path.substr(0, path.size() - 1);
	else
		testpath = path;
	int retval = _stat(testpath.c_str(), &Stat);
	if (retval != 0)
	{
		pathType = PathType::DoesNotExist;
	}
	else
	{
		if (Stat.st_mode & _S_IFDIR)
			pathType = PathType::Directory;
		else if (Stat.st_mode & _S_IFREG)
			pathType = PathType::File;
		else
			pathType = PathType::Other;
		ctime = Stat.st_ctime;
		atime = Stat.st_atime;
	}
}

string ReadTextFile(const string &filename)
{
	ifstream fin(filename.c_str());

	if (!fin)
		return "";

	string str;
	fin.seekg(0, ios::end);
	size_t size = (size_t)fin.tellg();
	str.resize(size);
	fin.seekg(0, ios::beg);
	fin.read(&str[0], size);

	fin.close();
	return str;
}

vector<FXubyte> ReadBinaryFile(const string &filename)
{
	vector<FXubyte> buffer;
	ifstream fin(filename.c_str(), ios::binary);

	if (!fin)
		return buffer;

	string str;
	fin.seekg(0, ios::end);
	size_t size = (size_t)fin.tellg();
	buffer.resize(size);
	fin.seekg(0, ios::beg);
	fin.read((char *)&buffer[0], size);

	fin.close();
	return buffer;
}

string FindPathIfExists(const string &path, const vector<string> pathsToTry)
{
	string output;

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
			string testPath = testPathIt + fpi.fullfname;
			if (TestIfFileExists(testPath))
			{
				output = testPath;
				break;
			}
		}
	}

	return output;
}

string NormalizePathName(const string &basepath, const string &path)
{
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

PathType GetPathType(const string &path)
{
	struct _stat Stat;
	int retval = _stat(path.c_str(), &Stat);
	if (retval != 0)
	{
		// do not print errors...
		return PathType::DoesNotExist;
	}
	if (Stat.st_mode & _S_IFDIR)
		return PathType::Directory;
	else if (Stat.st_mode & _S_IFREG)
		return PathType::File;
	return PathType::Other;
}

TimeValue GetPathCreationTime(const string &path)
{
	struct _stat Stat;
#ifdef USING_ERRNO
	errno_t errno;
	_set_errno(0);
#endif
	int retval = _stat(path.c_str(), &Stat);
	if (retval != 0)
	{
#ifdef USING_ERRNO
		switch (errno)
		{
		case ENOENT:
			cerr << __func__ << ": file not found" << endl;
			break;
		case EINVAL:
			cerr << __func__ << ": invalid parameter to _stat()" << endl;
			break;
		default:
			cerr << __func__ << ": unknown error in _stat()" << endl;
		}
#endif
	}
	else
	{
		return Stat.st_ctime;
	}
	return 0;
}

TimeValue GetPathAccessTime(const string &path)
{
	struct _stat Stat;
#ifdef USING_ERRNO
	errno_t errno;
	_set_errno(0);
#endif
	int retval = _stat(path.c_str(), &Stat);
	if (retval != 0)
	{
#ifdef USING_ERRNO
		switch (errno)
		{
		case ENOENT:
			cerr << __func__ << ": file not found" << endl;
			break;
		case EINVAL:
			cerr << __func__ << ": invalid parameter to _stat()" << endl;
			break;
		default:
			cerr << __func__ << ": unknown error in _stat()" << endl;
		}
#endif
	}
	else
	{
		return Stat.st_atime;
	}
	return 0;
}

// Data type IOSTREAM Utilities
bool ReadBool(istream &istr)
{
	string str;
	istr >> str;
	if (str == "true")
		return true;
	return false;
}

int ReadInt(istream &istr)
{
	int ival;
	istr >> ival;
	return ival;
}

long long ReadInt64(istream &istr)
{
	long long ival;
	istr >> ival;
	return ival;
}

float ReadFloat(istream &istr)
{
	double fval;
	istr >> fval;
	return (float)fval;
}

double ReadDouble(istream &istr)
{
	double fval;
	istr >> fval;
	return fval;
}

string ReadString(istream &istr)
{
	string str;
	char c;
	char lastC = 0;
	bool isQuotes = false;

	istr >> ws;
	while (1)
	{
		c = 0;
		istr >> noskipws >> c;
		istr >> skipws;
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

Vector2f ReadVector2f(istream &istr)
{
	double x, y;
	istr >> x;
	istr >> y;
	return Vector2f((float)x, (float)y);
}

Vector2d ReadVector2d(istream &istr)
{
	double x, y;
	istr >> x;
	istr >> y;
	return Vector2d(x, y);
}

Vector3f ReadVector3f(istream &istr)
{
	double x, y, z;
	istr >> x;
	istr >> y;
	istr >> z;
	return Vector3f((float)x, (float)y, (float)z);
}

Vector3d ReadVector3d(istream &istr)
{
	double x, y, z;
	istr >> x;
	istr >> y;
	istr >> z;
	return Vector3d(x, y, z);
}

Vector4f ReadVector4f(istream &istr)
{
	double x, y, z, w;
	istr >> x;
	istr >> y;
	istr >> z;
	istr >> w;
	return Vector4f((float)x, (float)y, (float)z, (float)w);
}

Vector4d ReadVector4d(istream &istr)
{
	double x, y, z, w;
	istr >> x;
	istr >> y;
	istr >> z;
	istr >> w;
	return Vector4d(x, y, z, w);
}

Quaternionf ReadQuaternionf(istream &istr)
{
	double a, b, c, d;
	istr >> a;
	istr >> b;
	istr >> c;
	istr >> d;
	return Quaternionf((float)a, (float)b, (float)c, (float)d);
}

Quaterniond ReadQuaterniond(istream &istr)
{
	double a, b, c, d;
	istr >> a;
	istr >> b;
	istr >> c;
	istr >> d;
	return Quaterniond(a, b, c, d);
}

Matrix4f ReadMatrix4f(istream &istr)
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

Matrix4d ReadMatrix4d(istream &istr)
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

Matrix4f ReadAffineMatrix4f(istream &istr)
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

Matrix4d ReadAffineMatrix4d(istream &istr)
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

SphericalHarmonicf ReadSphericalHarmonicf(istream &istr)
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
		cerr << __FUNCTION__ << "(): invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10" << endl;
	}
	return sph;
}

SphericalHarmonicd ReadSphericalHarmonicd(istream &istr)
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
		cerr << __FUNCTION__ << "(): invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10" << endl;
	}
	return sph;
}

// WRITING ROUTINES

ostream &WriteBool(ostream &ostr, bool val)
{
	if (val == true)
		ostr << "true ";
	else
		ostr << "false ";
	return ostr;
}

ostream &WriteInt(ostream &ostr, int val)
{
	return ostr << val << " ";
}

ostream &WriteInt64(ostream &ostr, long long val)
{
	return ostr << val << " ";
}

ostream &WriteFloat(ostream &ostr, float val)
{
	return ostr << val << " ";
}

ostream &WriteDouble(ostream &ostr, double val)
{
	return ostr << val << " ";
}

ostream &WriteString(ostream &ostr, const string &str)
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

ostream &WriteVector2f(ostream &ostr, const Vector2f &v)
{
	return ostr << v.x << " " << v.y << " ";
}

ostream &WriteVector2d(ostream &ostr, const Vector2d &v)
{
	return ostr << v.x << " " << v.y << " ";
}

ostream &WriteVector3f(ostream &ostr, const Vector3f &v)
{
	return ostr << v.x << " " << v.y << " " << v.z << " ";
}

ostream &WriteVector3d(ostream &ostr, const Vector3d &v)
{
	return ostr << v.x << " " << v.y << " " << v.z << " ";
}

ostream &WriteVector4f(ostream &ostr, const Vector4f &v)
{
	return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
}

ostream &WriteVector4d(ostream &ostr, const Vector4d &v)
{
	return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
}

ostream &WriteQuaternionf(ostream &ostr, const Quaternionf &q)
{
	return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
}

ostream &WriteQuaterniond(ostream &ostr, const Quaterniond &q)
{
	return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
}

ostream &WriteMatrix4f(ostream &ostr, const Matrix4f &m)
{
	ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
	ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
	ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
	ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
	return ostr;
}

ostream &WriteMatrix4d(ostream &ostr, const Matrix4d &m)
{
	ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
	ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
	ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
	ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
	return ostr;
}

ostream &WriteAffineMatrix4f(ostream &ostr, const Matrix4f &m)
{
	ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
	ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
	ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
	return ostr;
}

ostream &WriteAffineMatrix4d(ostream &ostr, const Matrix4d &m)
{
	ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
	ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
	ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
	return ostr;
}

ostream &WriteSphericalHarmonicf(ostream &ostr, const SphericalHarmonicf &sph)
{
	ostr << sph.GetMaxDegree() << " ";
	for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
	{
		ostr << sph[i] << " ";
	}
	return ostr;
}

ostream &WriteSphericalHarmonicd(ostream &ostr, const SphericalHarmonicd &sph)
{
	ostr << sph.GetMaxDegree() << " ";
	for (size_t i = 0; i < sph.getMaxCoefficients(); i++)
	{
		ostr << sph[i] << " ";
	}
	return ostr;
}
} // namespace Fluxions
