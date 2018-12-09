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
#ifndef FLUXIONS_XML_HPP
#define FLUXIONS_XML_HPP

#include <fluxions_gte.hpp>

namespace Fluxions
{
//bool XmlReadBool(std::istream &istr, const std::string &tagName);
//int XmlReadInt(std::istream &istr, const std::string &tagName);
//long long XmlReadInt64(std::istream &istr, const std::string &tagName);
//float XmlReadFloat(std::istream &istr, const std::string &tagName);
//double XmlReadDouble(std::istream &istr, const std::string &tagName);
//string XmlReadString(std::istream &istr, const std::string &tagName);
//Vector2f XmlReadVector2f(std::istream &istr, const std::string &tagName);
//Vector2d XmlReadVector2d(std::istream &istr, const std::string &tagName);
//Vector3f XmlReadVector3f(std::istream &istr, const std::string &tagName);
//Vector3d XmlReadVector3d(std::istream &istr, const std::string &tagName);
//Vector4f XmlReadVector4f(std::istream &istr, const std::string &tagName);
//Vector4d XmlReadVector4d(std::istream &istr, const std::string &tagName);
//Quaternionf XmlReadQuaternionf(std::istream &istr, const std::string &tagName);
//Quaterniond XmlReadQuaterniond(std::istream &istr, const std::string &tagName);
//Matrix4f XmlReadMatrix4f(std::istream &istr, const std::string &tagName);
//Matrix4d XmlReadMatrix4d(std::istream &istr, const std::string &tagName);
//Matrix4f XmlReadAffineMatrix4f(std::istream &istr, const std::string &tagName);
//Matrix4d XmlReadAffineMatrix4d(std::istream &istr, const std::string &tagName);
//SphericalHarmonicf XmlReadSphericalHarmonicf(std::istream &istr, const std::string &tagName);
//SphericalHarmonicd XmlReadSphericalHarmonicd(std::istream &istr, const std::string &tagName);

std::ostream &XmlBool(std::ostream &ostr, const std::string &tagName, bool val, int numtabs = 0);
std::ostream &XmlInt(std::ostream &ostr, const std::string &tagName, int val, int numtabs = 0);
std::ostream &XmlInt64(std::ostream &ostr, const std::string &tagName, long long val, int numtabs = 0);
std::ostream &XmlSize(std::ostream &ostr, const std::string &tagName, size_t val, int numtabs = 0);
std::ostream &XmlFloat(std::ostream &ostr, const std::string &tagName, float val, int numtabs = 0);
std::ostream &XmlDouble(std::ostream &ostr, const std::string &tagName, double val, int numtabs = 0);
std::ostream &XmlString(std::ostream &ostr, const std::string &tagName, const std::string &str, int numtabs = 0);
std::ostream &XmlVector2f(std::ostream &ostr, const std::string &tagName, const Vector2f &v, int numtabs = 0);
std::ostream &XmlVector2d(std::ostream &ostr, const std::string &tagName, const Vector2d &v, int numtabs = 0);
std::ostream &XmlVector3f(std::ostream &ostr, const std::string &tagName, const Vector3f &v, int numtabs = 0);
std::ostream &XmlVector3d(std::ostream &ostr, const std::string &tagName, const Vector3d &v, int numtabs = 0);
std::ostream &XmlVector4f(std::ostream &ostr, const std::string &tagName, const Vector4f &v, int numtabs = 0);
std::ostream &XmlVector4d(std::ostream &ostr, const std::string &tagName, const Vector4d &v, int numtabs = 0);
std::ostream &XmlColor3f(std::ostream &ostr, const std::string &tagName, const Color3f &v, int numtabs = 0);
std::ostream &XmlColor3d(std::ostream &ostr, const std::string &tagName, const Color3d &v, int numtabs = 0);
std::ostream &XmlColor4f(std::ostream &ostr, const std::string &tagName, const Color4f &v, int numtabs = 0);
std::ostream &XmlColor4d(std::ostream &ostr, const std::string &tagName, const Color4d &v, int numtabs = 0);
std::ostream &XmlQuaternionf(std::ostream &ostr, const std::string &tagName, const Quaternionf &q, int numtabs = 0);
std::ostream &XmlQuaterniond(std::ostream &ostr, const std::string &tagName, const Quaterniond &q, int numtabs = 0);
std::ostream &XmlMatrix4f(std::ostream &ostr, const std::string &tagName, const Matrix4f &m, int numtabs = 0);
std::ostream &XmlMatrix4d(std::ostream &ostr, const std::string &tagName, const Matrix4d &m, int numtabs = 0);
std::ostream &XmlAffineMatrix4f(std::ostream &ostr, const std::string &tagName, const Matrix4f &m, int numtabs = 0);
std::ostream &XmlAffineMatrix4d(std::ostream &ostr, const std::string &tagName, const Matrix4d &m, int numtabs = 0);
std::ostream &XmlSphericalHarmonicf(std::ostream &ostr, const std::string &tagName, const SphericalHarmonicf &sph, int numtabs = 0);
std::ostream &XmlSphericalHarmonicd(std::ostream &ostr, const std::string &tagName, const SphericalHarmonicd &sph, int numtabs = 0);

std::ostream &XmlComment(std::ostream &ostr, const std::string &comment, int numtabs = 0);
std::ostream &XmlBeginTag(std::ostream &ostr, const std::string &tagName, int numtabs = 0);
std::ostream &XmlBeginTag(std::ostream &ostr, const std::string &tagName, const std::string &optClassName, int numtabs = 0);
std::ostream &XmlBeginTag(std::ostream &ostr, const std::string &tagName, const std::string &attributeName, const std::string &value, int numtabs = 0);
std::ostream &XmlEndTag(std::ostream &ostr, const std::string &tagName, int numtabs = 0);

std::ostream &XmlCoronaMapTexture(std::ostream &ostr, const std::string &tagName, const std::string &mapName, const std::string &imagePath, int numtabs = 0, const float gamma = 2.2f);
} // namespace Fluxions

#endif