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
#include <fluxions_xml.hpp>

namespace Fluxions {
//bool XmlReadBool(istream &istr, const string &tagName)
//{

//}

//int XmlReadInt(istream &istr, const string &tagName)
//{
//}

//long long XmlReadInt64(istream &istr, const string &tagName)
//{
//}

//float XmlReadFloat(istream &istr, const string &tagName)
//{
//}

//double XmlReadDouble(istream &istr, const string &tagName)
//{
//}

//string XmlReadString(istream &istr, const string &tagName)
//{
//}

//Vector2f XmlReadVector2f(istream &istr, const string &tagName)
//{
//}

//Vector2d XmlReadVector2d(istream &istr, const string &tagName)
//{
//}

//Vector3f XmlReadVector3f(istream &istr, const string &tagName)
//{
//}

//Vector3d XmlReadVector3d(istream &istr, const string &tagName)
//{
//}

//Vector4f XmlReadVector4f(istream &istr, const string &tagName)
//{
//}

//Vector4d XmlReadVector4d(istream &istr, const string &tagName)
//{
//}

//Quaternionf XmlReadQuaternionf(istream &istr, const string &tagName)
//{
//}

//Quaterniond XmlReadQuaterniond(istream &istr, const string &tagName)
//{
//}

//Matrix4f XmlReadMatrix4f(istream &istr, const string &tagName)
//{
//}

//Matrix4d XmlReadMatrix4d(istream &istr, const string &tagName)
//{
//}

//Matrix4f XmlReadAffineMatrix4f(istream &istr, const string &tagName)
//{
//}

//Matrix4d XmlReadAffineMatrix4d(istream &istr, const string &tagName)
//{
//}

//SphericalHarmonicf XmlReadSphericalHarmonicf(istream &istr, const string &tagName)
//{
//}

//SphericalHarmonicd XmlReadSphericalHarmonicd(istream &istr, const string &tagName)
//{
//}

ostream& XmlBool(ostream& ostr, const string& tagName, bool val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    if (val == true)
        ostr << "true";
    else
        ostr << "false";
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlInt(ostream& ostr, const string& tagName, int val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << val;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlInt64(ostream& ostr, const string& tagName, long long val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << val;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlSize(ostream& ostr, const string& tagName, size_t val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << val;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlFloat(ostream& ostr, const string& tagName, float val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << val;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlDouble(ostream& ostr, const string& tagName, double val, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << val;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlString(ostream& ostr, const string& tagName, const string& str, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">" << str << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector2f(ostream& ostr, const string& tagName, const Vector2f& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector2d(ostream& ostr, const string& tagName, const Vector2d& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector3f(ostream& ostr, const string& tagName, const Vector3f& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y << " " << v.z;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector3d(ostream& ostr, const string& tagName, const Vector3d& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y << " " << v.z;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector4f(ostream& ostr, const string& tagName, const Vector4f& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y << " " << v.z << " " << v.w;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlVector4d(ostream& ostr, const string& tagName, const Vector4d& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.x << " " << v.y << " " << v.z << " " << v.w;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlColor3f(ostream& ostr, const string& tagName, const Color3f& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.r << " " << v.g << " " << v.b;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlColor3d(ostream& ostr, const string& tagName, const Color3d& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.r << " " << v.g << " " << v.b;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlColor4f(ostream& ostr, const string& tagName, const Color4f& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.r << " " << v.g << " " << v.b << " " << v.a;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlColor4d(ostream& ostr, const string& tagName, const Color4d& v, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << v.r << " " << v.g << " " << v.b << " " << v.a;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlQuaternionf(ostream& ostr, const string& tagName, const Quaternionf& q, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << q.a << " " << q.b << " " << q.c << " " << q.d;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlQuaterniond(ostream& ostr, const string& tagName, const Quaterniond& q, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << q.a << " " << q.b << " " << q.c << " " << q.d;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlMatrix4f(ostream& ostr, const string& tagName, const Matrix4f& m, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlMatrix4d(ostream& ostr, const string& tagName, const Matrix4d& m, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
    ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlAffineMatrix4f(ostream& ostr, const string& tagName, const Matrix4f& m, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlAffineMatrix4d(ostream& ostr, const string& tagName, const Matrix4d& m, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
    ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
    ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34;
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlSphericalHarmonicf(ostream& ostr, const string& tagName, const SphericalHarmonicf& sph, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << sph.GetMaxDegree();
    for (size_t i = 0; i < sph.getMaxCoefficients(); i++) {
        ostr << " " << sph[i];
    }
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlSphericalHarmonicd(ostream& ostr, const string& tagName, const SphericalHarmonicd& sph, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    ostr << sph.GetMaxDegree();
    for (size_t i = 0; i < sph.getMaxCoefficients(); i++) {
        ostr << " " << sph[i];
    }
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlComment(ostream& ostr, const string& comment, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<!-- " << comment << " -->" << endl;
    return ostr;
}

ostream& XmlBeginTag(ostream& ostr, const string& tagName, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << ">";
    return ostr;
}

ostream& XmlBeginTag(ostream& ostr, const string& tagName, const string& className, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << " class=\"" << className << "\">";
    return ostr;
}

ostream& XmlBeginTag(ostream& ostr, const string& tagName, const string& attributeName, const string& value, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "<" << tagName << " " << attributeName << "=\"" << value << "\">";
    return ostr;
}

ostream& XmlEndTag(ostream& ostr, const string& tagName, int numtabs)
{
    for (int i = 0; i < numtabs; i++)
        ostr << "\t";
    ostr << "</" << tagName << ">";
    return ostr;
}

ostream& XmlCoronaMapTexture(ostream& ostr, const string& tagName, const string& mapName, const string& imagePath, int numtabs, const float gamma)
{
    XmlBeginTag(ostr, tagName, "name", mapName, numtabs) << endl;
    XmlBeginTag(ostr, "map", "Texture", numtabs + 1) << endl;
    XmlString(ostr, "image", imagePath, numtabs + 2) << endl;
    XmlFloat(ostr, "gamma", gamma, numtabs + 2) << endl;
    XmlEndTag(ostr, "map", numtabs + 1) << endl;
    XmlEndTag(ostr, tagName, numtabs);
    return ostr;
}
}