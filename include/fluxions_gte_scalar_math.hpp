// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
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

#ifndef FLUXIONS_GTE_SCALAR_MATH_HPP
#define FLUXIONS_GTE_SCALAR_MATH_HPP

#include <algorithm>
#include <type_traits>
#define _USE_MATH_DEFINES
#include <numeric>
#include <cmath>

namespace Fluxions
{
	using f32_t = float;
	using f64_t = double;

	enum IndexType
	{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3,
		S = 0,
		T = 1,
		P = 2,
		Q = 3,
		R = 0,
		G = 1,
		B = 2,
		A = 3
	};

	//const double pi = 3.141592653589793;
	//const double e = 2.718281828459045;
	//const double PI = 3.141592653589793;
	//const double E = 2.718281828459045;
	//const double PI_OVER_180 = 0.01745329251994329576923690768489;
	//const double INV_PI_OVER_180 = 57.295779513082320876798154814105;
	//const double PI_OVER_2 = 1.5707963267948966192313216916398;

	//////////////////////////////////////////////////////////////////////
	// M A T H E M A T I C A L   C O N S T A N T S ///////////////////////
	//////////////////////////////////////////////////////////////////////

	constexpr double FX_SQRT2 = 1.4142135623730950488016887242097;
	constexpr double FX_SQRT3 = 1.7320508075688772935274463415059;
	constexpr double FX_SQRT5 = 2.2360679774997896964091736687313;
	constexpr double FX_ONEOVERSQRT2 = 0.70710678118654752440084436210485;
	constexpr double FX_ONEOVERSQRT3 = 0.57735026918962576450914878050196;
	constexpr double FX_ONEOVERSQRT5 = 0.44721359549995793928183473374626;
	constexpr double FX_E = 2.7182818284590452353602874713527;
	constexpr double FX_PHI = 1.618033988749894848204586834;
	constexpr double FX_TWOPI = 6.283185307179586476925286766559;
	constexpr double FX_PI = 3.1415926535897932384626433832795;
	constexpr double FX_PIOVERTWO = 1.5707963267948966192313216916398;
	constexpr double FX_PIOVERTHREE = 1.0471975511965977461542144610932;
	constexpr double FX_PIOVERFOUR = 0.78539816339744830961566084581988;
	constexpr double FX_RADIANS_TO_DEGREES = 57.295779513082320876798154814105;
	constexpr double FX_EXTRA_RADIANS_TO_HOURS = 3.8197186342054880584532103209403;
	constexpr double FX_EXTRA_RADIANS_TO_ARCMINS = 229.18311805232928350719261925642;
	constexpr double FX_EXTRA_RADIANS_TO_ARCSECS = 13750.987083139757010431557155385;
	constexpr double FX_DEGREES_TO_RADIANS = 0.01745329251994329576923690768489;
	constexpr double FX_EXTRA_DEGREES_TO_HOURS = 0.06666666666666666666666666666667;
	constexpr double FX_EXTRA_DEGREES_TO_ARCMINS = 4.0;
	constexpr double FX_EXTRA_DEGREES_TO_ARCSECS = 240.0;
	constexpr double FX_HOURS_TO_RADIANS = 0.26179938779914943653855361527329;
	constexpr double FX_HOURS_TO_DEGREES = 15.0;
	constexpr double FX_HOURS_TO_ARCMINS = 60.0;
	constexpr double FX_HOURS_TO_ARCSECS = 3600.0;
	constexpr double FX_ARCMINS_TO_RADIANS = 0.00436332312998582394230922692122;
	constexpr double FX_ARCMINS_TO_DEGREES = 0.00416666666666666666666666666667;
	constexpr double FX_ARCMINS_TO_HOURS = 0.01666666666666666666666666666667;
	constexpr double FX_ARCMINS_TO_ARCSECS = 60.0;
	constexpr double FX_ARCSECS_TO_DEGREES = 2.7777777777777777777777777777778e-4;
	constexpr double FX_ARCSECS_TO_RADIANS = 4.8481368110953599358991410235795e-6;
	constexpr double FX_ARCSECS_TO_HOURS = 2.7777777777777777777777777777778e-4;
	constexpr double FX_ARCSECS_TO_ARCMINS = 0.01666666666666666666666666666667;
	constexpr double FX_C = 299792458.0;            // speed of light
	constexpr double FX_G = 6.6740831e-11;          // Newton's constant of gravitation
	constexpr double FX_H = 6.62607004081e-34;      // Planck's h constant
	constexpr double FX_HBAR = 1.05457180013e-34;   // Planck's h bar constant
	constexpr double FX_EPSILON0 = 8.854187817e-12; // electric constant

	constexpr double FX_LOG2E = 1.44269504088896340736;    // log2(e)
	constexpr double FX_LOG10E = 0.434294481903251827651;  // log10(e)
	constexpr double FX_LN2 = 0.693147180559945309417;     // ln(2)
	constexpr double FX_LN10 = 2.30258509299404568402;     // ln(10)
	constexpr double FX_PI_2 = 1.57079632679489661923;     // pi/2
	constexpr double FX_PI_4 = 0.785398163397448309616;    // pi/4
	constexpr double FX_1_PI = 0.318309886183790671538;    // 1/pi
	constexpr double FX_2_PI = 0.636619772367581343076;    // 2/pi
	constexpr double FX_2_SQRTPI = 1.12837916709551257390; // 2/sqrt(pi)
	constexpr double FX_SQRT1_2 = 0.707106781186547524401; // 1/sqrt(2)

	constexpr float FX_F32_SQRT2 = 1.41421356f;
	constexpr float FX_F32_SQRT3 = 1.73205080f;
	constexpr float FX_F32_SQRT5 = 2.23606797f;
	constexpr float FX_F32_ONEOVERSQRT2 = 0.70710678f;
	constexpr float FX_F32_ONEOVERSQRT3 = 0.57735026f;
	constexpr float FX_F32_ONEOVERSQRT5 = 0.44721359f;
	constexpr float FX_F32_E = 2.71828182f;
	constexpr float FX_F32_PHI = 1.61803398f;
	constexpr float FX_F32_TWOPI = 6.28318530f;
	constexpr float FX_F32_PI = 3.1415926535f;
	constexpr float FX_F32_PIOVERTWO = 1.57079632f;
	constexpr float FX_F32_PIOVERTHREE = 1.04719755f;
	constexpr float FX_F32_PIOVERFOUR = 0.78539816f;
	constexpr float FX_F32_RADIANS_TO_DEGREES = 57.29577951f;
	constexpr float FX_F32_EXTRA_RADIANS_TO_HOURS = 3.81971863f;
	constexpr float FX_F32_EXTRA_RADIANS_TO_ARCMINS = 229.18311805f;
	constexpr float FX_F32_EXTRA_RADIANS_TO_ARCSECS = 13750.98708313f;
	constexpr float FX_F32_DEGREES_TO_RADIANS = 0.01745329f;
	constexpr float FX_F32_EXTRA_DEGREES_TO_HOURS = 0.06666666f;
	constexpr float FX_F32_EXTRA_DEGREES_TO_ARCMINS = 4.0f;
	constexpr float FX_F32_EXTRA_DEGREES_TO_ARCSECS = 240.0f;
	constexpr float FX_F32_HOURS_TO_RADIANS = 0.26179938f;
	constexpr float FX_F32_HOURS_TO_DEGREES = 15.0f;
	constexpr float FX_F32_HOURS_TO_ARCMINS = 60.0f;
	constexpr float FX_F32_HOURS_TO_ARCSECS = 3600.0f;
	constexpr float FX_F32_ARCMINS_TO_RADIANS = 0.00436332f;
	constexpr float FX_F32_ARCMINS_TO_DEGREES = 0.00416666f;
	constexpr float FX_F32_ARCMINS_TO_HOURS = 0.01666666f;
	constexpr float FX_F32_ARCMINS_TO_ARCSECS = 60.0f;
	constexpr float FX_F32_ARCSECS_TO_DEGREES = 2.77777777e-4f;
	constexpr float FX_F32_ARCSECS_TO_RADIANS = 4.84813681e-6f;
	constexpr float FX_F32_ARCSECS_TO_HOURS = 2.77777777e-4f;
	constexpr float FX_F32_ARCSECS_TO_ARCMINS = 0.01666666f;
	constexpr float FX_F32_C = 299792458.0f;           // speed of light
	constexpr float FX_F32_G = 6.6740831e-11f;         // Newton's constant of gravitation
	constexpr float FX_F32_H = 6.62607004e-34f;        // Planck's h constant
	constexpr float FX_F32_HBAR = 1.05457180e-34f;     // Planck's h bar constant
	constexpr float FX_F32_EPSILON0 = 8.85418781e-12f; // electric constant

	constexpr float FX_F32_LOG2E = 1.44269504f;    // log2(e)
	constexpr float FX_F32_LOG10E = 0.43429448f;   // log10(e)
	constexpr float FX_F32_LN2 = 0.69314718f;      // ln(2)
	constexpr float FX_F32_LN10 = 2.30258509f;     // ln(10)
	constexpr float FX_F32_PI_2 = 1.57079632f;     // pi/2
	constexpr float FX_F32_PI_4 = 0.78539816f;     // pi/4
	constexpr float FX_F32_1_PI = 0.31830988f;     // 1/pi
	constexpr float FX_F32_2_PI = 0.63661977f;     // 2/pi
	constexpr float FX_F32_2_SQRTPI = 1.12837916f; // 2/sqrt(pi)
	constexpr float FX_F32_SQRT1_2 = 0.70710678f;  // 1/sqrt(2)

	template <typename T>
	constexpr T DegToRad(T x) noexcept {
		return x * FX_DEGREES_TO_RADIANS;
	}

	template <typename T>
	constexpr T RadToDeg(T x) noexcept {
		return x * FX_RADIANS_TO_DEGREES;
	}

	template <typename T>
	constexpr bool within(T x, T a, T b) noexcept {
		return x >= a && x <= b;
	}

	template <typename T>
	constexpr T clamp(T x, T minValue, T maxValue) noexcept {
		if (x > maxValue)
			return maxValue;
		if (x < minValue)
			return minValue;
		return x;
	}

	// Wraps the variable X in the half open domain [a, b)
	template <typename T>
	constexpr T wrap(T x, T a, T b) noexcept {
		if (a > b)
			std::swap(a, b);
		if (a == b)
			return x;
		T out = fmod(x - a, b - a) + a;
		if (out < a)
			out += b - a;
		return out;
	}

	// same as wrap(X, a, b), but with a = 0.0
	template <typename T>
	constexpr T wrap(T x, T b) noexcept {
		if (b == 0)
			return x;
		T out = fmod(x, b);
		if (out < 0.0)
			out += b;
		return out;
	}

	template<typename T>
	constexpr T incr_wrap(T& x, T b) noexcept {
		if (x >= b) x = 0;
		else x++;
		return x;
	}

	template <typename T>
	constexpr T decr_wrap(T& x, T b) noexcept {
		if (x == 0) x = b;
		else x--;
		return x;
	}

	template <typename T>
	constexpr T saturate(T x, T maxvalue) noexcept {
		return (x > maxvalue) ? maxvalue : x;
	}

	template <typename T>
	constexpr T saturate(T x) noexcept {
		return (x > T(1)) ? T(1) : x;
	}

	template <typename _Ty1, typename _Ty2>
	constexpr _Ty2 remap_value_min_max(_Ty1 x, const _Ty2 minValue, const _Ty2 maxValue) noexcept {
		if (maxValue == minValue)
			return 0;
		return (_Ty2(x) - minValue) / (maxValue - minValue);
	}

	template <typename _Ty1, typename _Ty2>
	constexpr _Ty2 remap_value_min_max(_Ty1 x, const _Ty1 min1, const _Ty1 max1, const _Ty2 min2, const _Ty2 max2) noexcept {
		if (max1 == min1)
			return 0;

		return (_Ty2)(((x - min1) / (max1 - min1)) * (max2 - min2) + min2);
	}

	template <typename IntType, typename FloatType>
	FloatType int_to_float0to1(IntType x, const FloatType minValue, const FloatType maxValue) noexcept {
		if (maxValue == minValue)
			return 0;

		return (FloatType(x) - minValue) / (maxValue - minValue);
	}

	template <typename IntType, typename FloatType>
	constexpr IntType float0to1_to_int(FloatType x, const IntType min, const IntType max) noexcept {
		return clamp<IntType>(static_cast<IntType>((max - min) * x) + min, min, max);
	}

	// (a * b) + c
	template <typename T>
	constexpr T multiply_add(T a, T b, T c) noexcept {
		return a * b + c;
	}

	float randomSampler(float _min0, float _max0);

	inline void MakeFaceSTFromCubeVector(float x, float y, float z, float* s, float* t, unsigned* whichFace) noexcept {
		// ma is absolute value
		float absX = fabsf(x);
		float absY = fabsf(y);
		float absZ = fabsf(z);
		// signs of ma
		int isXPositive = x > 0 ? 1 : 0;
		int isYPositive = y > 0 ? 1 : 0;
		int isZPositive = z > 0 ? 1 : 0;

		// 0 | sc = -z | tc = -y | ma = +X | dir: +rx
		// 1 | sc = +z | tc = -y | ma = +X | dir: -rx
		// 2 | sc = +X | tc = +z | ma = +y | dir: +ry
		// 3 | sc = +X | tc = -z | ma = +y | dir: -ry
		// 4 | sc = +X | tc = -y | ma = +z | dir: +rz
		// 5 | sc = -X | tc = -y | ma = +z | dir: -rz
		float maxAxis = 0.0f;
		float sc = 0.0f;
		float tc = 0.0f;
		// GL_TEXTURE_CUBE_MAP_POSITIVE_X
		if (isXPositive && absX >= absY && absX >= absZ) {
			// s (0 to 1) goes from -z to +z
			// t (0 to 1) goes from -y to +y
			maxAxis = absX;
			sc = -z;
			tc = y;
			*whichFace = 0;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		if (!isXPositive && absX >= absY && absX >= absZ) {
			// s (0 to 1) goes from +z to -z
			// t (0 to 1) goes from -y to +y
			maxAxis = absX;
			sc = z;
			tc = y;
			*whichFace = 1;
		}
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		if (isYPositive && absY >= absX && absY >= absZ) {
			// s (0 to 1) goes from -X to +X
			// t (0 to 1) goes from -z to +z
			maxAxis = absY;
			sc = x;
			tc = -z;
			*whichFace = 2;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		if (!isYPositive && absY >= absX && absY >= absZ) {
			// s (0 to 1) goes from -X to +X
			// t (0 to 1) goes from +z to -z
			maxAxis = absY;
			sc = x;
			tc = z;
			*whichFace = 3;
		}
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		if (isZPositive && absZ >= absX && absZ >= absY) {
			// s (0 to 1) goes from +X to -X
			// t (0 to 1) goes from -y to +y
			maxAxis = absZ;
			sc = x;
			tc = y;
			*whichFace = 4;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		if (!isZPositive && absZ >= absX && absZ >= absY) {
			// s (0 to 1) goes from -X to +X
			// t (0 to 1) goes from -y to +y
			maxAxis = absZ;
			sc = -x;
			tc = y;
			*whichFace = 5;
		}
		// s = 0.5 * (sc / ma + 1)
		// t = 0.5 * (tc / ma + 1)
		*s = 0.5f * (sc / maxAxis + 1.0f);
		*t = 0.5f * (tc / maxAxis + 1.0f);
	}

	constexpr void MakeCubeVectorFromFaceST(unsigned face, float s, float t, float* x, float* y, float* z) noexcept {
		// 0 GL_TEXTURE_CUBE_MAP_POSITIVE_X
		// 1 GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		// 2 GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		// 3 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		// 4 GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		// 5 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		//
		// ma is the major axis (the largest of the absolute values of X, y, z)
		// sc is the s coordinate in 3D
		// tc is the t coordinate in 3D
		// 0 | sc = -z | tc = -y | ma = +X | dir: +rx
		// 1 | sc = +z | tc = -y | ma = +X | dir: -rx
		// 2 | sc = +X | tc = +z | ma = +y | dir: +ry
		// 3 | sc = +X | tc = -z | ma = +y | dir: -ry
		// 4 | sc = +X | tc = -y | ma = +z | dir: +rz
		// 5 | sc = -X | tc = -y | ma = +z | dir: -rz
		//
		// s = 0.5 * (sc / ma + 1)
		// t = 0.5 * (tc / ma + 1)
		//
		// sc = ma (2s - 1)
		// tc = ma (2t - 1)
		//
		float sc = 2.0f * s - 1;
		float tc = 2.0f * t - 1;

		switch (face) {
			//case 0: *X = 1.0; *y = tc; *z = sc; break;
			//case 1: *X = -1.0; *y = tc; *z = -sc; break;
			//case 2: *X = sc; *y = 1.0; *z = tc; break;
			//case 3: *X = sc; *y = -1.0; *z = -tc; break;
			//case 4: *X = -sc; *y = tc; *z = 1.0; break;
			//case 5: *X = sc; *y = tc; *z = -1.0; break;
		case 0:
			*x = 1.0f;
			*y = tc;
			*z = -sc;
			break; // POSITIVE X
		case 1:
			*x = -1.0f;
			*y = tc;
			*z = sc;
			break; // NEGATIVE X
		case 2:
			*x = sc;
			*y = 1.0f;
			*z = -tc;
			break; // POSITIVE Y
		case 3:
			*x = sc;
			*y = -1.0f;
			*z = tc;
			break; // NEGATIVE Y
		case 4:
			*x = sc;
			*y = tc;
			*z = 1.0f;
			break; // POSITIVE Z
		case 5:
			*x = -sc;
			*y = tc;
			*z = -1.0f;
			break; // NEGATIVE Z
		}
	}

	//template <typename T>
	//T wrap(T X, T min, T max)
	//{
	//	T width = max - min;
	//	while (X >= max)
	//	{
	//		X -= width;
	//	}
	//	while (X < min)
	//	{
	//		X += width;
	//	}
	//	return X;
	//}

	template <typename T>
	constexpr T min2(T x1, T x2) noexcept {
		return x1 < x2 ? x1 : x2;
	}

	template <typename T>
	constexpr T max2(T x1, T x2) noexcept {
		return x1 > x2 ? x1 : x2;
	}

	template <typename T>
	constexpr T min3(T x1, T x2, T x3) noexcept {
		if (x1 < x2) {
			if (x1 < x3)
				return x1;
			else
				return x3;
		}
		else if (x2 < x3)
			return x2;
		else
			return x3;
	}

	template <typename T>
	constexpr T max3(T x1, T x2, T x3) noexcept {
		return (x1 > x2) ? (x1 > x3) ? x1 : x3 : (x2 > x3) ? x2 : x3;
	}

	template <typename T>
	constexpr T maxof(T x) noexcept {
		return x;
	}

	template <typename T>
	constexpr T sqr(T x) noexcept {
		return x * x;
	}

	template <typename T>
	constexpr T sgn(T x) noexcept {
		if (x < 0)
			return -1;
		else if (x > 0)
			return 1;
		else
			return 0;
	}

	template <typename T>
	constexpr T abs(T x) noexcept {
		if (x < 0)
			return -x;
		return x;
	}

	// SUPERQUADRIC GENERATOR FUNCTIONS
	/* sqS (ptr, n)
	* This function implements the s(ptr,n) utility function
	*
	* s(ptr,n) = sgn(sin(ptr)) * |sin(ptr)|^n
	*/
	inline double sqS(double v, double n) noexcept {
		return sgn(sin(v)) * pow(abs(sin(v)), n);
	}

	inline float sqSf(float v, float n) noexcept {
		return sgn(sinf(v)) * powf(fabsf(sinf(v)), n);
	}

	/* sqC (ptr, n)
	* This function implements the c(ptr,n) utility function
	*
	* c(ptr,n) = sgn(cos(ptr)) * |cos(ptr)|^n
	*/
	inline double sqC(double v, double n) noexcept {
		return sgn(cos(v)) * pow(abs(cos(v)), n);
	}

	inline float sqCf(float v, float n) noexcept {
		return sgn(cosf(v)) * powf(fabsf(cosf(v)), n);
	}

	/* sqCT (ptr, n, alpha)
	* This function implements the CT(ptr,n,alpha) utility function
	*
	* CT(ptr,n,alpha) = alpha + c(ptr,n)
	*/
	inline double sqCT(double v, double n, double alpha) noexcept {
		return alpha + sqC(v, n);
	}

	inline float sqCTf(float v, float n, float alpha) noexcept {
		return alpha + sqCf(v, n);
	}

	inline void sqEllipsoid(float a1, float a2, float a3, float u, float v, float n, float e,
							float* x, float* y, float* z, float* nx, float* ny, float* nz) {
		*x = a1 * sqCf(u, n) * sqCf(v, e);
		*y = a2 * sqCf(u, n) * sqSf(v, e);
		*z = a3 * sqSf(u, n);
		*nx = sqCf(u, 2.0f - n) * sqCf(v, 2.0f - e);
		*ny = sqCf(u, 2.0f - n) * sqSf(v, 2.0f - e);
		*nz = sqSf(u, 2.0f - n);
	}

	template <typename T>
	constexpr T Radians(T angleInDegrees) noexcept {
		return (T)(angleInDegrees * FX_RADIANS_TO_DEGREES);
	}

	template <typename T>
	constexpr T Degrees(T angleInRadians) noexcept {
		return (T)(angleInRadians * FX_DEGREES_TO_RADIANS);
	}

	template <class T>
	constexpr T SRGBToLinear(T x) noexcept {
		T a = T(0.055);
		T e = T(2.4);
		T bottom = T(0.04045);
		T bottomFactor = T(12.92);
		return x <= bottom ? T(x / bottomFactor) : T(std::pow((x + a) / (T(1) + a), e));
	}

	template <class T>
	constexpr T LinearToSRGB(T x) noexcept {
		constexpr T a = T(0.055);
		constexpr T e = T(1.0 / 2.4);
		constexpr T bottom = T(0.0031308);
		// constexpr T bottomFactor = T(12.92);

		if (x <= bottom) {
			return T(x * 12.92f);
		}

		return ((1 + a) * std::pow(x, e) - a);
	}

	constexpr f32_t LinearToSRGBf(f32_t x) noexcept { return LinearToSRGB<f32_t>(x); }
	constexpr f64_t LinearToSRGBd(f64_t x) noexcept { return LinearToSRGB<f64_t>(x); }
	constexpr f32_t SRGBToLinearf(f32_t x) noexcept { return SRGBToLinear<f32_t>(x); }
	constexpr f64_t SRGBToLineard(f64_t x) noexcept { return SRGBToLinear<f64_t>(x); }
} //namespace Fluxions

#endif
