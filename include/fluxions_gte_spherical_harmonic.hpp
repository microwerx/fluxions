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

	// Some C routines to compute legendre polynomials and spherical harmonics
	double legendre_polynomial(int l, int m, double x) noexcept;
	double spherical_harmonic(int l, int m, double theta, double phi) noexcept;

	constexpr int GetMaxSphCoefficients(int l) noexcept { return l * (l + 1) + l + 1; }

	template <typename T>
	T calc_spherical_harmonic(int max_degree, const T *a, T theta, T phi);

#ifdef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template float calc_spherical_harmonic<float>(int max_degree, const float *, float theta, float phi);
	extern template double calc_spherical_harmonic<double>(int max_degree, const double *, double theta, double phi);
#endif

	template <typename VectorType, typename ScalarType>
	class TSphericalHarmonic
	{
	private:
		size_t maxCoefficients = 1;
		size_t maxDegree = 0;
		std::vector<VectorType> coefficients;

	public:
		constexpr TSphericalHarmonic()
		{
			resize(0);
		}

		constexpr void reset() noexcept { resize(maxDegree, VectorType(0)); }
		constexpr void reset(size_t maxDegree_) noexcept { resize(maxDegree_, VectorType(0)); }
		constexpr void reset(size_t maxDegree_, const VectorType &value) noexcept { resize(maxDegree_, value); }
		constexpr void resize(size_t maxDegree_) noexcept;
		constexpr void resize(size_t maxDegree_, const VectorType &value) noexcept;
		void readFromString(const std::string &data) noexcept;
		void readFromFile(const std::string &filename) noexcept;

		// VectorType sum() const { VectorType a = 0; for (auto x : coefficients) a += x; return a; }
		constexpr VectorType sum() const noexcept { return accumulate(coefficients.begin(), coefficients.end(), VectorType(0)); }

		constexpr const VectorType *cdata() const noexcept { return &coefficients[0]; }
		constexpr VectorType *data() noexcept { return &coefficients[0]; }

		constexpr TSphericalHarmonic<VectorType, ScalarType> &operator*=(const ScalarType s) noexcept
		{
			for (auto &x : coefficients)
			{
				x *= (VectorType)s;
			}
			return *this;
		}

		constexpr TSphericalHarmonic<VectorType, ScalarType> &operator+=(const TSphericalHarmonic<VectorType, ScalarType> &b) noexcept
		{
			if (coefficients.size() != b.coefficients.size())
			{
				//hflog.error("%s(): spherical harmonics do not have the same degree! addition not done.", __FUNCTION__);
				return *this;
			}

			for (size_t i = 0; i < coefficients.size(); i++)
			{
				coefficients[i] += b.coefficients[i];
			}

			return *this;
		}

		constexpr TSphericalHarmonic<VectorType, ScalarType> &Accumulate(const TSphericalHarmonic<VectorType, ScalarType> &b, const ScalarType c = 1, int maxDegrees = -1) noexcept
		{
			size_t firstIndex = 0;
			size_t lastDegree = maxDegree;
			if (maxDegrees >= 0)
			{
				lastDegree = min3<size_t>(maxDegrees, maxDegree, b.maxDegree);
			}
			else
			{
				lastDegree = std::min(maxDegree, b.maxDegree);
			}
			size_t lastIndex = lastDegree * (lastDegree + 1);

			for (size_t i = firstIndex; i <= lastIndex; i++)
			{
				coefficients[i] += b.coefficients[i] * (VectorType)c;
			}

			return *this;
		}

		constexpr TSphericalHarmonic<VectorType, ScalarType> &Accumulate(const TSphericalHarmonic<VectorType, ScalarType> &b, const TSphericalHarmonic<VectorType, ScalarType> &c, int maxDegrees = -1) noexcept
		{
			size_t firstIndex = 0;
			size_t lastDegree = maxDegree;
			if (maxDegrees >= 0)
			{
				lastDegree = min3<size_t>(maxDegrees, maxDegree, b.maxDegree);
			}
			else
			{
				lastDegree = std::min(maxDegree, b.maxDegree);
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

		constexpr const VectorType operator[](size_t i) const noexcept
		{
			if (i >= 0 && i < maxCoefficients)
				return coefficients[i];
			else
				return VectorType();
		}

		constexpr VectorType &operator[](size_t i)
		{
			if (i <= maxCoefficients) {
				return coefficients[i];
			}
			else
			{
				hflog.errorfn(__FUNCTION__, "maxDegree must be in the range 0 <= maxDegree <= 10");
				throw std::out_of_range("maxDegree must be in the range 0 <= maxDegree <= 10");
			}
		}

		constexpr size_t GetMaxDegree() const noexcept
		{
			return maxDegree;
		}

		constexpr size_t getMaxCoefficients() const noexcept
		{
			return maxDegree * (maxDegree + 1) + maxDegree + 1;
		}

		// returns -1 if out of range, or index if in range
		constexpr size_t getCoefficientIndex(size_t l, int m) const noexcept
		{
			if (l <= maxDegree && (size_t)abs(m) <= l)
				return l * (l + 1) + m;
			else
				return 0;
		}

		constexpr std::vector<VectorType> getCoefficients() noexcept
		{
			return coefficients;
		}

		constexpr VectorType getCoefficient(size_t l, int m) const noexcept
		{
			size_t index = getCoefficientIndex(l, m);
			if (index < coefficients.size())
				return coefficients[index];
			else
				return VectorType();
		}

		constexpr VectorType getCoefficient(size_t lm) const noexcept
		{
			if (lm < coefficients.size())
				return coefficients[lm];
			else
				return VectorType();
		}

		constexpr void setCoefficient(size_t l, int m, const VectorType x) noexcept
		{
			size_t index = getCoefficientIndex(l, m);
			if (index < coefficients.size())
				coefficients[index] = x;
		}

		constexpr void setCoefficient(size_t lm, const VectorType x) noexcept
		{
			if (lm < coefficients.size())
				coefficients[lm] = x;
		}

		constexpr VectorType e_m_phi_real(int m, double phi) noexcept
		{
			if (m < 0)
				return VectorType(FX_SQRT2 * sin(-m * phi));
			if (m > 0)
				return VectorType(FX_SQRT2 * cos(m * phi));
			return VectorType(1);
		}

		constexpr VectorType K(size_t l, int m) noexcept
		{
			// ScalarType fourPi = (ScalarType)12.566370614359172953850573533118;
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

		constexpr VectorType calc(VectorType theta, VectorType phi) noexcept
		{
			const VectorType *a = &coefficients[0];
			return calc_spherical_harmonic<VectorType>(maxDegree, a, theta, phi);
		}

	private:
		static constexpr long long factorial(int i) noexcept
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
				2432902008176640000 };
			return f[i];
		};
	};

	template <typename VectorType, typename ScalarType>
	constexpr void TSphericalHarmonic<VectorType, ScalarType>::resize(size_t maxDegree_) noexcept
	{
		if (maxDegree_ > 10)
		{
			hflog.warningfn(__FUNCTION__, "maxDegree must be in the range 0 <= maxDegree <= 10");
			maxDegree_ = 10;
		}

		maxDegree = maxDegree_;
		maxCoefficients = maxDegree_ * (maxDegree_ + 1) + maxDegree_ + 1; // don't forget to reserve room for the 0th degree.
		if (maxCoefficients > coefficients.size())
			coefficients.resize(maxCoefficients);
	}

	template <typename VectorType, typename ScalarType>
	constexpr void TSphericalHarmonic<VectorType, ScalarType>::resize(size_t maxDegree_, const VectorType &value) noexcept
	{
		if (maxDegree_ > 10)
		{
			hflog.warningfn(__FUNCTION__, "maxDegree must be in the range 0 <= maxDegree <= 10");
			maxDegree_ = 10;
		}

		maxDegree = maxDegree_;
		maxCoefficients = maxDegree_ * (maxDegree_ + 1) + maxDegree_ + 1; // don't forget to reserve room for the 0th degree.
		if (maxCoefficients > coefficients.size())
			coefficients.resize(maxCoefficients);
		fill(coefficients.begin(), coefficients.end(), value);
	}

	template <typename VectorType, typename ScalarType>
	void TSphericalHarmonic<VectorType, ScalarType>::readFromString(const std::string &data) noexcept
	{
		std::istringstream iss(data);
		std::string fileId;
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
	void TSphericalHarmonic<VectorType, ScalarType>::readFromFile(const std::string &filename) noexcept
	{
		std::ifstream fin(filename.c_str());

		if (!fin)
			return;

		std::string str;
		fin.seekg(0, std::ios::end);
		size_t size = (size_t)fin.tellg();
		str.resize(size);
		fin.seekg(0, std::ios::beg);
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

#ifdef FLUXIONS_NO_EXTERN_TEMPLATES
	extern template class TSphericalHarmonic<float, float>;
	extern template class TSphericalHarmonic<double, double>;
	//extern template class TSphericalHarmonic<Vector3f>;
	//extern template class TSphericalHarmonic<Vector3d>;
	//extern template class TSphericalHarmonic<Vector4f>;
	//extern template class TSphericalHarmonic<Vector4d>;
#endif

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

		void SaveJSON(const std::string &path);
	};
} // namespace Fluxions

#endif
