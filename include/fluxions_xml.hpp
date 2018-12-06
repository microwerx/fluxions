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
	//bool XmlReadBool(istream &istr, const string &tagName);
	//int XmlReadInt(istream &istr, const string &tagName);
	//long long XmlReadInt64(istream &istr, const string &tagName);
	//float XmlReadFloat(istream &istr, const string &tagName);
	//double XmlReadDouble(istream &istr, const string &tagName);
	//string XmlReadString(istream &istr, const string &tagName);
	//Vector2f XmlReadVector2f(istream &istr, const string &tagName);
	//Vector2d XmlReadVector2d(istream &istr, const string &tagName);
	//Vector3f XmlReadVector3f(istream &istr, const string &tagName);
	//Vector3d XmlReadVector3d(istream &istr, const string &tagName);
	//Vector4f XmlReadVector4f(istream &istr, const string &tagName);
	//Vector4d XmlReadVector4d(istream &istr, const string &tagName);
	//Quaternionf XmlReadQuaternionf(istream &istr, const string &tagName);
	//Quaterniond XmlReadQuaterniond(istream &istr, const string &tagName);
	//Matrix4f XmlReadMatrix4f(istream &istr, const string &tagName);
	//Matrix4d XmlReadMatrix4d(istream &istr, const string &tagName);
	//Matrix4f XmlReadAffineMatrix4f(istream &istr, const string &tagName);
	//Matrix4d XmlReadAffineMatrix4d(istream &istr, const string &tagName);
	//SphericalHarmonicf XmlReadSphericalHarmonicf(istream &istr, const string &tagName);
	//SphericalHarmonicd XmlReadSphericalHarmonicd(istream &istr, const string &tagName);

	ostream & XmlBool(ostream &ostr, const string &tagName, bool val, int numtabs = 0);
	ostream & XmlInt(ostream &ostr, const string &tagName, int val, int numtabs = 0);
	ostream & XmlInt64(ostream &ostr, const string &tagName, long long val, int numtabs = 0);
	ostream & XmlSize(ostream &ostr, const string &tagName, size_t val, int numtabs = 0);
	ostream & XmlFloat(ostream &ostr, const string &tagName, float val, int numtabs = 0);
	ostream & XmlDouble(ostream &ostr, const string &tagName, double val, int numtabs = 0);
	ostream & XmlString(ostream &ostr, const string &tagName, const string &str, int numtabs = 0);
	ostream & XmlVector2f(ostream &ostr, const string &tagName, const Vector2f &v, int numtabs = 0);
	ostream & XmlVector2d(ostream &ostr, const string &tagName, const Vector2d &v, int numtabs = 0);
	ostream & XmlVector3f(ostream &ostr, const string &tagName, const Vector3f &v, int numtabs = 0);
	ostream & XmlVector3d(ostream &ostr, const string &tagName, const Vector3d &v, int numtabs = 0);
	ostream & XmlVector4f(ostream &ostr, const string &tagName, const Vector4f &v, int numtabs = 0);
	ostream & XmlVector4d(ostream &ostr, const string &tagName, const Vector4d &v, int numtabs = 0);
	ostream & XmlQuaternionf(ostream &ostr, const string &tagName, const Quaternionf &q, int numtabs = 0);
	ostream & XmlQuaterniond(ostream &ostr, const string &tagName, const Quaterniond &q, int numtabs = 0);
	ostream & XmlMatrix4f(ostream &ostr, const string &tagName, const Matrix4f & m, int numtabs = 0);
	ostream & XmlMatrix4d(ostream &ostr, const string &tagName, const Matrix4d & m, int numtabs = 0);
	ostream & XmlAffineMatrix4f(ostream &ostr, const string &tagName, const Matrix4f &m, int numtabs = 0);
	ostream & XmlAffineMatrix4d(ostream &ostr, const string &tagName, const Matrix4d &m, int numtabs = 0);
	ostream & XmlSphericalHarmonicf(ostream &ostr, const string &tagName, const SphericalHarmonicf &sph, int numtabs = 0);
	ostream & XmlSphericalHarmonicd(ostream &ostr, const string &tagName, const SphericalHarmonicd &sph, int numtabs = 0);

	ostream & XmlComment(ostream &ostr, const string & comment, int numtabs = 0);
	ostream & XmlBeginTag(ostream &ostr, const string &tagName, int numtabs = 0);
	ostream & XmlBeginTag(ostream &ostr, const string &tagName, const string &optClassName, int numtabs = 0);
	ostream & XmlBeginTag(ostream &ostr, const string &tagName, const string &attributeName, const string &value, int numtabs = 0);
	ostream & XmlEndTag(ostream &ostr, const string &tagName, int numtabs = 0);

	ostream & XmlCoronaMapTexture(ostream &ostr, const string &tagName, const string &mapName, const string &imagePath, int numtabs = 0, const float gamma = 2.2f);
}


#endif