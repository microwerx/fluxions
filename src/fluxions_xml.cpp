#include "fluxions_pch.hpp"
#include <fluxions_xml.hpp>

namespace Fluxions {
	//bool XmlReadBool(std::istream &istr, const std::string &tagName)
	//{

	//}

	//int XmlReadInt(std::istream &istr, const std::string &tagName)
	//{
	//}

	//long long XmlReadInt64(std::istream &istr, const std::string &tagName)
	//{
	//}

	//float XmlReadFloat(std::istream &istr, const std::string &tagName)
	//{
	//}

	//double XmlReadDouble(std::istream &istr, const std::string &tagName)
	//{
	//}

	//string XmlReadString(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector2f XmlReadVector2f(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector2d XmlReadVector2d(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector3f XmlReadVector3f(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector3d XmlReadVector3d(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector4f XmlReadVector4f(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Vector4d XmlReadVector4d(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Quaternionf XmlReadQuaternionf(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Quaterniond XmlReadQuaterniond(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Matrix4f XmlReadMatrix4f(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Matrix4d XmlReadMatrix4d(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Matrix4f XmlReadAffineMatrix4f(std::istream &istr, const std::string &tagName)
	//{
	//}

	//Matrix4d XmlReadAffineMatrix4d(std::istream &istr, const std::string &tagName)
	//{
	//}

	//SphericalHarmonicf XmlReadSphericalHarmonicf(std::istream &istr, const std::string &tagName)
	//{
	//}

	//SphericalHarmonicd XmlReadSphericalHarmonicd(std::istream &istr, const std::string &tagName)
	//{
	//}

	std::ostream& XmlBool(std::ostream& ostr, const std::string& tagName, bool val, int numtabs) {
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

	std::ostream& XmlInt(std::ostream& ostr, const std::string& tagName, int val, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << val;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlInt64(std::ostream& ostr, const std::string& tagName, long long val, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << val;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlSize(std::ostream& ostr, const std::string& tagName, unsigned val, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << val;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlFloat(std::ostream& ostr, const std::string& tagName, float val, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << val;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlDouble(std::ostream& ostr, const std::string& tagName, double val, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << val;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlString(std::ostream& ostr, const std::string& tagName, const std::string& str, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">" << str << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector2f(std::ostream& ostr, const std::string& tagName, const Vector2f& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector2d(std::ostream& ostr, const std::string& tagName, const Vector2d& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector3f(std::ostream& ostr, const std::string& tagName, const Vector3f& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y << " " << v.z;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector3d(std::ostream& ostr, const std::string& tagName, const Vector3d& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y << " " << v.z;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector4f(std::ostream& ostr, const std::string& tagName, const Vector4f& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y << " " << v.z << " " << v.w;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlVector4d(std::ostream& ostr, const std::string& tagName, const Vector4d& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.x << " " << v.y << " " << v.z << " " << v.w;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlColor3f(std::ostream& ostr, const std::string& tagName, const Color3f& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.r << " " << v.g << " " << v.b;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlColor3d(std::ostream& ostr, const std::string& tagName, const Color3d& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.r << " " << v.g << " " << v.b;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlColor4f(std::ostream& ostr, const std::string& tagName, const Color4f& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.r << " " << v.g << " " << v.b << " " << v.a;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlColor4d(std::ostream& ostr, const std::string& tagName, const Color4d& v, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << v.r << " " << v.g << " " << v.b << " " << v.a;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlQuaternionf(std::ostream& ostr, const std::string& tagName, const Quaternionf& q, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << q.a << " " << q.b << " " << q.c << " " << q.d;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlQuaterniond(std::ostream& ostr, const std::string& tagName, const Quaterniond& q, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << q.a << " " << q.b << " " << q.c << " " << q.d;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlMatrix4f(std::ostream& ostr, const std::string& tagName, const Matrix4f& m, int numtabs) {
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

	std::ostream& XmlMatrix4d(std::ostream& ostr, const std::string& tagName, const Matrix4d& m, int numtabs) {
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

	std::ostream& XmlAffineMatrix4f(std::ostream& ostr, const std::string& tagName, const Matrix4f& m, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlAffineMatrix4d(std::ostream& ostr, const std::string& tagName, const Matrix4d& m, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34;
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlSphericalHarmonicf(std::ostream& ostr, const std::string& tagName, const SphericalHarmonicf& sph, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << sph.GetMaxDegree();
		for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
			ostr << " " << sph[i];
		}
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlSphericalHarmonicd(std::ostream& ostr, const std::string& tagName, const SphericalHarmonicd& sph, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		ostr << sph.GetMaxDegree();
		for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
			ostr << " " << sph[i];
		}
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlComment(std::ostream& ostr, const std::string& comment, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<!-- " << comment << " -->" << "\n";
		return ostr;
	}

	std::ostream& XmlBeginTag(std::ostream& ostr, const std::string& tagName, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlBeginTag(std::ostream& ostr, const std::string& tagName, const std::string& className, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << " class=\"" << className << "\">";
		return ostr;
	}

	std::ostream& XmlBeginTag(std::ostream& ostr, const std::string& tagName, const std::string& attributeName, const std::string& value, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "<" << tagName << " " << attributeName << "=\"" << value << "\">";
		return ostr;
	}

	std::ostream& XmlEndTag(std::ostream& ostr, const std::string& tagName, int numtabs) {
		for (int i = 0; i < numtabs; i++)
			ostr << "\t";
		ostr << "</" << tagName << ">";
		return ostr;
	}

	std::ostream& XmlCoronaMapTexture(std::ostream& ostr, const std::string& tagName, const std::string& mapName, const std::string& imagePath, int numtabs, const float gamma) {
		XmlBeginTag(ostr, tagName, "name", mapName, numtabs) << "\n";
		XmlBeginTag(ostr, "map", "Texture", numtabs + 1) << "\n";
		XmlString(ostr, "image", imagePath, numtabs + 2) << "\n";
		XmlFloat(ostr, "gamma", gamma, numtabs + 2) << "\n";
		XmlEndTag(ostr, "map", numtabs + 1) << "\n";
		XmlEndTag(ostr, tagName, numtabs);
		return ostr;
	}
} // namespace Fluxions
