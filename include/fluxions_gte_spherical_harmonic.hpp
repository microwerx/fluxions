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
#ifndef FLUXIONS_GTE_SPHERICAL_HARMONIC_HPP
#define FLUXIONS_GTE_SPHERICAL_HARMONIC_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <fluxions_gte_scalar.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{
using namespace std;

// Some C routines to compute legendre polynomials and spherical harmonics
double legendre_polynomial(int l, int m, double x);
double spherical_harmonic(int l, int m, double theta, double phi);

inline int GetMaxSphCoefficients(int l) { return l * (l + 1) + l + 1; }

template <typename T>
T calc_spherical_harmonic(int max_degree, const T *a, T theta, T phi);

extern template float calc_spherical_harmonic<float>(int max_degree, const float *, float theta, float phi);
extern template double calc_spherical_harmonic<double>(int max_degree, const double *, double theta, double phi);

template <typename VectorType, typename ScalarType>
class TSphericalHarmonic
{
  private:
	int maxCoefficients = 1;
	int maxDegree = 0;
	std::vector<VectorType> coefficients;

  public:
	TSphericalHarmonic()
	{
		resize(0);
	}

	~TSphericalHarmonic()
	{
	}

	void reset() { resize(maxDegree, VectorType(0)); }
	void reset(int maxDegree_) { resize(maxDegree_, VectorType(0)); }
	void reset(int maxDegree_, const VectorType &value) { resize(maxDegree_, value); }
	void resize(int maxDegree_);
	void resize(int maxDegree_, const VectorType &value);
	void readFromString(const string &data);
	void readFromFile(const string &filename);

	// VectorType sum() const { VectorType a = 0; for (auto x : coefficients) a += x; return a; }
	VectorType sum() const { return accumulate(coefficients.begin(), coefficients.end(), VectorType(0)); }

	inline const VectorType *cdata() const { return &coefficients[0]; }
	inline VectorType *data() { return &coefficients[0]; }

	inline TSphericalHarmonic<VectorType, ScalarType> &operator*=(const ScalarType s)
	{
		for (auto &x : coefficients)
		{
			x *= (VectorType)s;
		}
		return *this;
	}

	inline TSphericalHarmonic<VectorType, ScalarType> &operator+=(const TSphericalHarmonic<VectorType, ScalarType> &b)
	{
		if (coefficients.size() != b.coefficients.size())
		{
			hflog.error("%s(): spherical harmonics do not have the same degree! addition not done.", __FUNCTION__);
			return *this;
		}

		for (size_t i = 0; i < coefficients.size(); i++)
		{
			coefficients[i] += b.coefficients[i];
		}

		return *this;
	}

	TSphericalHarmonic<VectorType, ScalarType> &Accumulate(const TSphericalHarmonic<VectorType, ScalarType> &b, const ScalarType c = 1, int maxDegrees = -1)
	{
		int firstIndex = 0;
		int lastDegree = maxDegree;
		if (maxDegrees >= 0)
		{
			lastDegree = min3(maxDegrees, maxDegree, b.maxDegree);
		}
		else
		{
			lastDegree = min(maxDegree, b.maxDegree);
		}
		size_t lastIndex = lastDegree * (lastDegree + 1);

		//if (coefficients.size() != b.coefficients.size())
		//{
		//	hflog.error("%s(): spherical harmonics do not have the same degree! addition not done.", __FUNCTION__);
		//	return *this;
		//}

		for (size_t i = firstIndex; i <= lastIndex; i++)
		{
			coefficients[i] += b.coefficients[i] * (VectorType)c;
		}

		return *this;
	}

	TSphericalHarmonic<VectorType, ScalarType> &Accumulate(const TSphericalHarmonic<VectorType, ScalarType> &b, const TSphericalHarmonic<VectorType, ScalarType> &c, int maxDegrees = -1)
	{
		int firstIndex = 0;
		int lastDegree = maxDegree;
		if (maxDegrees >= 0)
		{
			lastDegree = min3(maxDegrees, maxDegree, b.maxDegree);
		}
		else
		{
			lastDegree = min(maxDegree, b.maxDegree);
		}
		size_t lastIndex = lastDegree * (lastDegree + 1);

		//if (coefficients.size() != b.coefficients.size())
		//{
		//	hflog.error("%s(): spherical harmonics do not have the same degree! addition not done.", __FUNCTION__);
		//	return *this;
		//}

		for (size_t i = firstIndex; i < lastIndex; i++)
		{
			coefficients[i] += b.coefficients[i] * c.coefficients[i];
		}

		return *this;
	}

	inline const VectorType operator[](size_t i) const
	{
		if (i >= 0 && i < maxCoefficients)
			return coefficients[i];
		else
			return VectorType();
	}

	inline VectorType &operator[](size_t i)
	{
		if (i >= 0 && i <= maxCoefficients)
			return coefficients[i];
		else
			throw std::out_of_range("TSphericalHarmonic<VectorType>::operator[](i): maxDegree must be in the range 0 <= maxDegree <= 10");
	}

	constexpr int GetMaxDegree() const
	{
		return maxDegree;
	}

	constexpr int getMaxCoefficients() const
	{
		return maxDegree * (maxDegree + 1) + maxDegree + 1;
	}

	// returns -1 if out of range, or index if in range
	constexpr int getCoefficientIndex(int l, int m) const
	{
		if (l >= 0 && l <= maxDegree && abs(m) <= l)
			return l * (l + 1) + m;
		else
			return -1;
	}

	constexpr auto getCoefficients()
	{
		return coefficients;
	}

	constexpr VectorType getCoefficient(int l, int m) const
	{
		int index = getCoefficientIndex(l, m);
		if (index >= 0)
			return coefficients[index];
		else
			return VectorType();
	}

	constexpr VectorType getCoefficient(int lm) const
	{
		if (lm >= 0 && lm < coefficients.size())
			return coefficients[lm];
		else
			return VectorType();
	}

	constexpr VectorType getCoefficient(size_t lm) const
	{
		if (lm < coefficients.size())
			return coefficients[lm];
		else
			return VectorType();
	}

	inline void setCoefficient(int l, int m, const VectorType x)
	{
		int index = getCoefficientIndex(l, m);
		if (index >= 0)
			coefficients[index] = x;
	}

	inline void setCoefficient(int lm, const VectorType x)
	{
		if (lm >= 0 && lm < coefficients.size())
			coefficients[lm] = x;
	}

	inline void setCoefficient(size_t lm, const VectorType x)
	{
		if (lm < coefficients.size())
			coefficients[lm] = x;
	}

	constexpr VectorType e_m_phi_real(int m, double phi)
	{
		if (m < 0)
			return VectorType(FX_SQRT2 * sin(-m * phi));
		if (m > 0)
			return VectorType(FX_SQRT2 * cos(m * phi));
		return VectorType(1);
	}

	constexpr VectorType K(int l, int m)
	{
		ScalarType fourPi = (ScalarType)12.566370614359172953850573533118;
		ScalarType invFourPi = (ScalarType)0.07957747154594766788444188168626;
		ScalarType c = invFourPi * (2 * l + 1);
		ScalarType t;
		if (m < 0)
		{
			t = ScalarType(factorial(l + m) / factorial(l - m));
		}
		else
		{
			t = ScalarType(factorial(l - m) / factorial(l + m));
		}
		return static_cast<VectorType>(sqrt(t * c));
	}

	constexpr VectorType calc(VectorType theta, VectorType phi)
	{
		const VectorType *a = &coefficients[0];
		return calc_spherical_harmonic<VectorType>(maxDegree, a, theta, phi);
	}

  private:
	static constexpr long long factorial(int i)
	{
		if (i < 0 || i > 20)
			return 0;
		long long f[] = {
			1,
			1,
			2,
			6,
			24,
			120,
			720,
			5040,
			40320,
			362880,
			3628800,
			39916800,
			479001600,
			6227020800,
			87178291200,
			1307674368000,
			20922789888000,
			355687428096000,
			6402373705728000,
			121645100408832000,
			2432902008176640000};
		return f[i];
	};
};

template <typename VectorType, typename ScalarType>
void TSphericalHarmonic<VectorType, ScalarType>::resize(int maxDegree_)
{
	if (maxDegree_ < 0 || maxDegree_ > 10)
	{
		throw std::out_of_range("TSphericalHarmonic<VectorType>::resize(maxDegree): maxDegree must be in the range 0 <= maxDegree <= 10");
	}

	maxDegree = maxDegree_;
	maxCoefficients = maxDegree_ * (maxDegree_ + 1) + maxDegree_ + 1; // don't forget to reserve room for the 0th degree.
	if (maxCoefficients > coefficients.size())
		coefficients.resize(maxCoefficients);
}

template <typename VectorType, typename ScalarType>
void TSphericalHarmonic<VectorType, ScalarType>::resize(int maxDegree_, const VectorType &value)
{
	if (maxDegree_ < 0 || maxDegree_ > 10)
	{
		throw std::out_of_range("TSphericalHarmonic<VectorType>::resize(maxDegree): maxDegree must be in the range 0 <= maxDegree <= 10");
	}

	maxDegree = maxDegree_;
	maxCoefficients = maxDegree_ * (maxDegree_ + 1) + maxDegree_ + 1; // don't forget to reserve room for the 0th degree.
	if (maxCoefficients > coefficients.size())
		coefficients.resize(maxCoefficients);
	fill(coefficients.begin(), coefficients.end(), value);
}

template <typename VectorType, typename ScalarType>
void TSphericalHarmonic<VectorType, ScalarType>::readFromString(const string &data)
{
	istringstream iss(data);
	string fileId;
	int numberOfBands;
	bool calcNegativeBands = false;
	double value;

	iss >> fileId;
	if (fileId == "SPH0")
	{
		calcNegativeBands = false;
	}
	else if (fileId == "SPH1")
	{
		calcNegativeBands = true;
	}
	else
	{
		return;
	}

	iss >> numberOfBands;

	if (numberOfBands <= 0 || numberOfBands > 10)
	{
		return;
	}

	resize(numberOfBands);
	for (int l = 0; l <= numberOfBands; l++)
	{
		value = 0.0;
		if (calcNegativeBands)
		{
			for (int m = 0; m <= l; m++)
			{
				iss >> value;
				setCoefficient(l, m, (VectorType)value);
				double negCoefficient = ((m & 1) ? -1.0 : 1.0) * factorial(l - m) / factorial(l + m) * value;
				setCoefficient(l, -m, (VectorType)negCoefficient);
			}
		}
		else
		{
			for (int m = -l; m <= l; m++)
			{
				iss >> value;
				setCoefficient(l, m, (VectorType)value);
			}
		}
	}
}

template <typename VectorType, typename ScalarType>
void TSphericalHarmonic<VectorType, ScalarType>::readFromFile(const string &filename)
{
	ifstream fin(filename.c_str());

	if (!fin)
		return;

	string str;
	fin.seekg(0, ios::end);
	size_t size = (size_t)fin.tellg();
	str.resize(size);
	fin.seekg(0, ios::beg);
	fin.read(&str[0], size);

	fin.close();
	readFromString(str);
}

using SphericalHarmonicf = TSphericalHarmonic<float, float>;
using SphericalHarmonicd = TSphericalHarmonic<float, double>;
//using SphericalHarmonic3f = TSphericalHarmonic<Vector3f>;
//using SphericalHarmonic3d = TSphericalHarmonic<Vector3d>;
//using SphericalHarmonic4f = TSphericalHarmonic<Vector4f>;
//using SphericalHarmonic4d = TSphericalHarmonic<Vector4d>;

extern template class TSphericalHarmonic<float, float>;
extern template class TSphericalHarmonic<double, double>;
//extern template class TSphericalHarmonic<Vector3f>;
//extern template class TSphericalHarmonic<Vector3d>;
//extern template class TSphericalHarmonic<Vector4f>;
//extern template class TSphericalHarmonic<Vector4d>;

using MultispectralSph4f = SphericalHarmonicf[4];
using MultispectralSph4d = SphericalHarmonicd[4];

// When finished, sph = (R+G+B)/3 and msph[3] = 0.2126R + 0.7152G + 0.0722B
bool MakeStandardizedSph(SphericalHarmonicf &sph, MultispectralSph4f &msph);
// Sets fourth channel msph[3] to luminance (0.2126R + 0.7152G + 0.0722B)
bool MakeLuminanceChannel4f(MultispectralSph4f &msph);
// Sets fourth channel msph[3] to intensity (R+G+B)/3
bool MakeIntensityChannel4f(MultispectralSph4f &msph);

struct Sph4f
{
	MultispectralSph4f msph;
	int maxDegree = 0;

	Sph4f() {}
	Sph4f(int degree, float value = 0.0f)
	{
		for (int i = 0; i < 4; i++)
			msph[i].resize(degree, value);
		maxDegree = degree;
	}
	Sph4f(const Sph4f &sph)
	{
		for (int i = 0; i < 4; i++)
			msph[i] = sph.msph[i];
	}
	Sph4f &operator=(const Sph4f &sph)
	{
		for (int i = 0; i < 4; i++)
			msph[i] = sph.msph[i];
		return *this;
	}

	SphericalHarmonicf &r() { return msph[0]; }
	SphericalHarmonicf &g() { return msph[1]; }
	SphericalHarmonicf &b() { return msph[2]; }
	SphericalHarmonicf &a() { return msph[3]; }

	const SphericalHarmonicf &r() const { return msph[0]; }
	const SphericalHarmonicf &g() const { return msph[1]; }
	const SphericalHarmonicf &b() const { return msph[2]; }
	const SphericalHarmonicf &a() const { return msph[3]; }

	SphericalHarmonicf &operator[](size_t i) { return msph[i]; }
	const SphericalHarmonicf &operator[](size_t i) const { return msph[i]; }

	void resize(int degree, float value = 0.0f)
	{
		for (int i = 0; i < 4; i++)
			msph[i].resize(degree, value);
		maxDegree = degree;
	}
	void reset()
	{
		for (int i = 0; i < 4; i++)
			msph[i].resize(maxDegree, 0.0f);
	}
	size_t size() const { return maxDegree * (maxDegree + 1) + maxDegree + 1; }

	inline void MakeLuminanceChannel() { MakeLuminanceChannel4f(msph); }
	inline void MakeIntensityChannel() { MakeIntensityChannel4f(msph); }

	void Accumulate(const Sph4f &b, const float c, int maxDegrees = -1)
	{
		for (int i = 0; i < 4; i++)
			msph[i].Accumulate(b[i], c, maxDegrees);
	}
	void Accumulate(const Sph4f &b, const Sph4f &c, int maxDegrees = -1)
	{
		for (int i = 0; i < 4; i++)
			msph[i].Accumulate(b[i], c[i], maxDegrees);
	}

	void SaveJSON(const string &path);
};
} // namespace Fluxions

#endif
