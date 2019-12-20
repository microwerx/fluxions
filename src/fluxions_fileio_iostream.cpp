#include "pch.hpp"
#include <hatchetfish.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions
{
	// Data type IOSTREAM Utilities
	bool ReadBool(std::istream& istr) {
		std::string str;
		istr >> str;
		if (str == "true")
			return true;
		return false;
	}

	int ReadInt(std::istream& istr) {
		int ival;
		istr >> ival;
		return ival;
	}

	long long ReadInt64(std::istream& istr) {
		long long ival;
		istr >> ival;
		return ival;
	}

	float ReadFloat(std::istream& istr) {
		double fval;
		istr >> fval;
		return (float)fval;
	}

	double ReadDouble(std::istream& istr) {
		double fval;
		istr >> fval;
		return fval;
	}

	std::string ReadString(std::istream& istr) {
		std::string str;
		char c;
		char lastC = 0;
		bool isQuotes = false;

		istr >> std::ws;
		while (1) {
			c = 0;
			istr >> std::noskipws >> c;
			istr >> std::skipws;
			if (!isQuotes && str.empty() && c == '\"') {
				isQuotes = true;
			}
			else if (isQuotes && c == '\"' && lastC != '\\') {
				return str;
			}
			else if (c == '\"' && lastC == '\\') {
				str.back() = c;
			}
			else if (!isQuotes && iswspace(c)) {
				return str;
			}
			else {
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

	Vector2f ReadVector2f(std::istream& istr) {
		double x, y;
		istr >> x;
		istr >> y;
		return Vector2f((float)x, (float)y);
	}

	Vector2d ReadVector2d(std::istream& istr) {
		double x, y;
		istr >> x;
		istr >> y;
		return Vector2d(x, y);
	}

	Vector3f ReadVector3f(std::istream& istr) {
		double x, y, z;
		istr >> x;
		istr >> y;
		istr >> z;
		return Vector3f((float)x, (float)y, (float)z);
	}

	Vector3d ReadVector3d(std::istream& istr) {
		double x, y, z;
		istr >> x;
		istr >> y;
		istr >> z;
		return Vector3d(x, y, z);
	}

	Vector4f ReadVector4f(std::istream& istr) {
		double x, y, z, w;
		istr >> x;
		istr >> y;
		istr >> z;
		istr >> w;
		return Vector4f((float)x, (float)y, (float)z, (float)w);
	}

	Vector4d ReadVector4d(std::istream& istr) {
		double x, y, z, w;
		istr >> x;
		istr >> y;
		istr >> z;
		istr >> w;
		return Vector4d(x, y, z, w);
	}

	Color3f ReadColor3f(std::istream& istr) {
		double r, g, b;
		istr >> r;
		istr >> g;
		istr >> b;
		return Color3f((float)r, (float)g, (float)b);
	}

	Color3d ReadColor3d(std::istream& istr) {
		double r, g, b;
		istr >> r;
		istr >> g;
		istr >> b;
		return Color3d(r, g, b);
	}

	Color4f ReadColor4f(std::istream& istr) {
		double r, g, b, a;
		istr >> r;
		istr >> g;
		istr >> b;
		istr >> a;
		return Color4f((float)r, (float)g, (float)b, (float)a);
	}

	Color4d ReadColor4d(std::istream& istr) {
		double r, g, b, a;
		istr >> r;
		istr >> g;
		istr >> b;
		istr >> a;
		return Color4d(r, g, b, a);
	}

	Quaternionf ReadQuaternionf(std::istream& istr) {
		double a, b, c, d;
		istr >> a;
		istr >> b;
		istr >> c;
		istr >> d;
		return Quaternionf((float)a, (float)b, (float)c, (float)d);
	}

	Quaterniond ReadQuaterniond(std::istream& istr) {
		double a, b, c, d;
		istr >> a;
		istr >> b;
		istr >> c;
		istr >> d;
		return Quaterniond(a, b, c, d);
	}

	Matrix4f ReadMatrix4f(std::istream& istr) {
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

	Matrix4d ReadMatrix4d(std::istream& istr) {
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

	Matrix4f ReadAffineMatrix4f(std::istream& istr) {
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

	Matrix4d ReadAffineMatrix4d(std::istream& istr) {
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

	SphericalHarmonicf ReadSphericalHarmonicf(std::istream& istr) {
		SphericalHarmonicf sph;
		int maxDegree = ReadInt(istr);
		if (maxDegree >= 0 && maxDegree <= 10) {
			sph.resize(maxDegree);
			for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
				sph[i] = ReadFloat(istr);
			}
		}
		else {
			HFLOGERROR("invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10");
		}
		return sph;
	}

	SphericalHarmonicd ReadSphericalHarmonicd(std::istream& istr) {
		SphericalHarmonicd sph;
		int maxDegree = ReadInt(istr);
		if (maxDegree >= 0 && maxDegree <= 10) {
			sph.resize(maxDegree);
			for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
				sph[i] = ReadFloat(istr);
			}
		}
		else {
			HFLOGERROR("invalid number of bands of spherical harmonics. Must satisfy condition that 0 <= maxDegree <= 10");
		}
		return sph;
	}

	// WRITING ROUTINES

	std::ostream& WriteBool(std::ostream& ostr, bool val) {
		if (val == true)
			ostr << "true ";
		else
			ostr << "false ";
		return ostr;
	}

	std::ostream& WriteInt(std::ostream& ostr, int val) {
		return ostr << val << " ";
	}

	std::ostream& WriteInt64(std::ostream& ostr, long long val) {
		return ostr << val << " ";
	}

	std::ostream& WriteFloat(std::ostream& ostr, float val) {
		return ostr << val << " ";
	}

	std::ostream& WriteDouble(std::ostream& ostr, double val) {
		return ostr << val << " ";
	}

	std::ostream& WriteString(std::ostream& ostr, const std::string& str) {
		ostr << "\"";
		for (auto it = str.begin(); it != str.end(); it++) {
			char c = *it;
			if (c == '\"')
				ostr << '\\' << '\"';
			else
				ostr << c;
		}
		ostr << "\" ";
		return ostr;
	}

	std::ostream& WriteVector2f(std::ostream& ostr, const Vector2f& v) {
		return ostr << v.x << " " << v.y << " ";
	}

	std::ostream& WriteVector2d(std::ostream& ostr, const Vector2d& v) {
		return ostr << v.x << " " << v.y << " ";
	}

	std::ostream& WriteVector3f(std::ostream& ostr, const Vector3f& v) {
		return ostr << v.x << " " << v.y << " " << v.z << " ";
	}

	std::ostream& WriteVector3d(std::ostream& ostr, const Vector3d& v) {
		return ostr << v.x << " " << v.y << " " << v.z << " ";
	}

	std::ostream& WriteVector4f(std::ostream& ostr, const Vector4f& v) {
		return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
	}

	std::ostream& WriteVector4d(std::ostream& ostr, const Vector4d& v) {
		return ostr << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
	}

	std::ostream& WriteColor3f(std::ostream& ostr, const Color3f& v) {
		return ostr << v.r << " " << v.g << " " << v.b << " ";
	}

	std::ostream& WriteColor3d(std::ostream& ostr, const Color3d& v) {
		return ostr << v.r << " " << v.g << " " << v.b << " ";
	}

	std::ostream& WriteColor4f(std::ostream& ostr, const Color4f& v) {
		return ostr << v.r << " " << v.g << " " << v.b << " " << v.a << " ";
	}

	std::ostream& WriteColor4d(std::ostream& ostr, const Color4d& v) {
		return ostr << v.r << " " << v.g << " " << v.b << " " << v.a << " ";
	}

	std::ostream& WriteQuaternionf(std::ostream& ostr, const Quaternionf& q) {
		return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
	}

	std::ostream& WriteQuaterniond(std::ostream& ostr, const Quaterniond& q) {
		return ostr << q.a << " " << q.b << " " << q.c << " " << q.d << " ";
	}

	std::ostream& WriteMatrix4f(std::ostream& ostr, const Matrix4f& m) {
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
		ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
		return ostr;
	}

	std::ostream& WriteMatrix4d(std::ostream& ostr, const Matrix4d& m) {
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
		ostr << m.m41 << " " << m.m42 << " " << m.m43 << " " << m.m44 << " ";
		return ostr;
	}

	std::ostream& WriteAffineMatrix4f(std::ostream& ostr, const Matrix4f& m) {
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
		return ostr;
	}

	std::ostream& WriteAffineMatrix4d(std::ostream& ostr, const Matrix4d& m) {
		ostr << m.m11 << " " << m.m12 << " " << m.m13 << " " << m.m14 << " ";
		ostr << m.m21 << " " << m.m22 << " " << m.m23 << " " << m.m24 << " ";
		ostr << m.m31 << " " << m.m32 << " " << m.m33 << " " << m.m34 << " ";
		return ostr;
	}

	std::ostream& WriteSphericalHarmonicf(std::ostream& ostr, const SphericalHarmonicf& sph) {
		ostr << sph.GetMaxDegree() << " ";
		for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
			ostr << sph[i] << " ";
		}
		return ostr;
	}

	std::ostream& WriteSphericalHarmonicd(std::ostream& ostr, const SphericalHarmonicd& sph) {
		ostr << sph.GetMaxDegree() << " ";
		for (unsigned i = 0; i < sph.getMaxCoefficients(); i++) {
			ostr << sph[i] << " ";
		}
		return ostr;
	}
}