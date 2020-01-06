#ifndef FLUXIONS_FILEIO_IOSTREAM_HPP
#define FLUXIONS_FILEIO_IOSTREAM_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>

namespace Fluxions
{
	bool ReadLines(const std::string& path, std::vector<std::string>& lines, bool noBlanksOrComments);

	bool ReadBool(std::istream& istr);
	int ReadInt(std::istream& istr);
	long long ReadInt64(std::istream& istr);
	float ReadFloat(std::istream& istr);
	double ReadDouble(std::istream& istr);
	std::string ReadString(std::istream& istr);
	Vector2f ReadVector2f(std::istream& istr);
	Vector2d ReadVector2d(std::istream& istr);
	Vector3f ReadVector3f(std::istream& istr);
	Vector3d ReadVector3d(std::istream& istr);
	Vector4f ReadVector4f(std::istream& istr);
	Vector4d ReadVector4d(std::istream& istr);
	Color3f ReadColor3f(std::istream& istr);
	Color3d ReadColor3d(std::istream& istr);
	Color4f ReadColor4f(std::istream& istr);
	Color4d ReadColor4d(std::istream& istr);
	Quaternionf ReadQuaternionf(std::istream& istr);
	Quaterniond ReadQuaterniond(std::istream& istr);
	Matrix4f ReadMatrix4f(std::istream& istr);
	Matrix4d ReadMatrix4d(std::istream& istr);
	Matrix4f ReadAffineMatrix4f(std::istream& istr);
	Matrix4d ReadAffineMatrix4d(std::istream& istr);
	SphericalHarmonicf ReadSphericalHarmonicf(std::istream& istr);
	SphericalHarmonicd ReadSphericalHarmonicd(std::istream& istr);

	std::ostream& WriteBool(std::ostream& ostr, bool val);
	std::ostream& WriteInt(std::ostream& ostr, int val);
	std::ostream& WriteInt64(std::ostream& ostr, long long val);
	std::ostream& WriteFloat(std::ostream& ostr, float val);
	std::ostream& WriteDouble(std::ostream& ostr, double val);
	std::ostream& WriteLabel(std::ostream& ostr, const std::string& str);
	std::ostream& WriteString(std::ostream& ostr, const std::string& str);
	std::ostream& WriteVector2f(std::ostream& ostr, const Vector2f& v);
	std::ostream& WriteVector2d(std::ostream& ostr, const Vector2d& v);
	std::ostream& WriteVector3f(std::ostream& ostr, const Vector3f& v);
	std::ostream& WriteVector3f(std::ostream& ostr, const Vector4f& v);
	std::ostream& WriteVector3d(std::ostream& ostr, const Vector3d& v);
	std::ostream& WriteVector4f(std::ostream& ostr, const Vector4f& v);
	std::ostream& WriteVector4d(std::ostream& ostr, const Vector4d& v);
	std::ostream& WriteColor3f(std::ostream& ostr, const Color3f& v);
	std::ostream& WriteColor3d(std::ostream& ostr, const Color3d& v);
	std::ostream& WriteColor4f(std::ostream& ostr, const Color4f& v);
	std::ostream& WriteColor4d(std::ostream& ostr, const Color4d& v);
	std::ostream& WriteQuaternionf(std::ostream& ostr, const Quaternionf& q);
	std::ostream& WriteQuaterniond(std::ostream& ostr, const Quaterniond& q);
	std::ostream& WriteMatrix4f(std::ostream& ostr, const Matrix4f& m);
	std::ostream& WriteMatrix4d(std::ostream& ostr, const Matrix4d& m);
	std::ostream& WriteAffineMatrix4f(std::ostream& ostr, const Matrix4f& m);
	std::ostream& WriteAffineMatrix4d(std::ostream& ostr, const Matrix4d& m);
	std::ostream& WriteSphericalHarmonicf(std::ostream& ostr, const SphericalHarmonicf& sph);
	std::ostream& WriteSphericalHarmonicd(std::ostream& ostr, const SphericalHarmonicd& sph);
	std::ostream& WriteIndices(std::ostream& os, int i1, int i2, int i3);

	template<typename T>
	std::ostream& WriteBinaryElement(std::ostream& os, T x) {
		return os.write(reinterpret_cast<const char*>(&x), sizeof(T));
	}

	template<typename T>
	std::istream& ReadBinaryElement(std::istream& is, T& x) {
		return is.read(reinterpret_cast<char*>(&x), sizeof(T));
	}

	template <typename T>
	std::ostream& WriteBinaryElement(std::ostream& os, const std::vector<T>& v) {
		return os.write(reinterpret_cast<const char*>(&v[0]), v.size() * sizeof(T));
	}

	template<typename T>
	std::istream& ReadBinaryElement(std::istream& is, std::vector<T>& v, size_t count) {
		v.resize(count);
		return is.read(reinterpret_cast<char*>(&v[0]), count * sizeof(T));
	}

	std::ostream& WriteBinaryString(std::ostream& os, const std::string& str);
	std::ostream& WriteBinaryStringMap(std::ostream& os, const string_string_map& m);

	std::istream& ReadBinaryString(std::istream& is, std::string& str);
	std::istream& ReadBinaryStringMap(std::istream& is, string_string_map& m);
}

#endif

