// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#include "pch.hpp"

#include <hatchetfish.hpp>
#include <fluxions_pbsky.hpp>
#include <ArHosekSkyModel.h>

//#include <ArHosekSkyModel.h>
//#include <fluxions_astronomy.hpp>
//#include <fluxions_gte_image.hpp>

namespace Fluxions
{
	using Real = float;
	const int NUM_WAVELENGTHS = 11;

	Real wavelengths[NUM_WAVELENGTHS] = {
		320.0f,
		360.0f,
		400.0f,
		440.0f,
		480.0f,
		520.0f,
		560.0f,
		600.0f,
		640.0f,
		680.0f,
		720.0f };

	// weights to integrate the tristimulus values about a
	Real tristimulus[NUM_WAVELENGTHS][3] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0174f, 0.0018f, 0.0881f},
		{0.1056f, 0.0173f, 0.585f},
		{0.037f, 0.0656f, 0.2973f},
		{0.0359f, 0.2406f, 0.0287f},
		{0.2179f, 0.3455f, 0.0009f},
		{0.3822f, 0.2433f, 0.0f},
		{0.1805f, 0.0768f, 0.0f},
		{0.0224f, 0.0087f, 0.0f},
		{0.0013f, 0.0005f, 0.0f} };

	template <typename T>
	T flterrzero(T x) {
		if (isfinite(x))
			return x;
		return 0.0;
	}

	class HosekWilkiePBSky
	{
	public:
		using Real = float;
		ArHosekSkyModelState* rgbRadianceState[3];
		ArHosekSkyModelState* sunRadianceState; // 320nm to 720nm in steps of 40nm for wavelength

		volatile Real minValue;
		volatile Real maxValue;
		volatile Real totalValue;
		volatile int nSamples;

		Real sunAltitude;
		Real sunInclination;
		Real sunAzimuth;
		Real sun[3];
		Real sunTheta;
		Real sunGamma;
		Real sunTurbidity = 1.0f;
		Real sunElevation = 0.0f;
		Color4f albedo;

		HosekWilkiePBSky();
		HosekWilkiePBSky(Real turbidity, Color4f albedo, Real elevation, Real azimuth);
		~HosekWilkiePBSky();
		void Init(Real turbidity, Color4f albedo, Real elevation, Real azimuth);
		void Delete();
		void resetStatisticSamples();
		void addStatisticSample(Real amount);
		void ComputeThetaGamma(Real inclination, Real azimuth, Real* outTheta, Real* outGamma) const;
		void ComputeThetaGamma(Real x, Real y, Real z, Real* outTheta, Real* outGamma) const;
		Real Compute(Real theta, Real gamma, int index);
		void ComputeSunRadiance(Real theta, Real gamma, Color4f& output) const;
		Real ComputeSunRadiance2(Real theta, Real gamma, int index);
		// 11 band Solar Radiance
		void ComputeSunRadiance3(Real theta, Real gamma, Color4f& output);
		// 3 band RGB Radiance
		void ComputeSunRadiance4(Real theta, Real gamma, Color4f& output);
		void ComputeSunRadiance4_NoStatistics(Real theta, Real gamma, Color4f& output) const;

		Color4f GetSunDiskRadiance() const;
		Color4f GetGroundRadiance() const;
	};


	HosekWilkiePBSky::HosekWilkiePBSky() {
		for (int i = 0; i < 3; i++)
			rgbRadianceState[i] = nullptr;
		sunRadianceState = nullptr;
		//for (int i = 0; i < NUM_WAVELENGTHS; i++) sunRadianceState[i] = nullptr;
	}

	HosekWilkiePBSky::HosekWilkiePBSky(Real turbidity, Color4f albedo, Real elevation, Real azimuth) {
		for (int i = 0; i < 3; i++)
			rgbRadianceState[i] = nullptr;
		sunRadianceState = nullptr;
		//for (int i = 0; i < 11; i++) sunRadianceState[i] = nullptr;
		Init(turbidity, albedo, elevation, azimuth);
	}

	HosekWilkiePBSky::~HosekWilkiePBSky() {
		// Delete();
	}

	void HosekWilkiePBSky::Init(Real turbidity, Color4f albedo_, Real elevation, Real azimuth) {
		// offset by -90 degrees because we are measuring from NORTH which is positive Y
		azimuth = 90.0f - azimuth;
		elevation *= (float)FX_DEGREES_TO_RADIANS;
		azimuth *= (float)FX_DEGREES_TO_RADIANS;

		if (this->albedo.r != albedo_.r || this->albedo.g != albedo_.g || this->albedo.b != albedo_.b || this->sunTurbidity != turbidity || this->sunElevation != elevation) {
			Delete();
		}

		if (!rgbRadianceState[0])
			rgbRadianceState[0] = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedo_.r, elevation);
		if (!rgbRadianceState[1])
			rgbRadianceState[1] = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedo_.g, elevation);
		if (!rgbRadianceState[2])
			rgbRadianceState[2] = arhosek_rgb_skymodelstate_alloc_init(turbidity, albedo_.b, elevation);

		sunRadianceState = arhosekskymodelstate_alloc_init(elevation, turbidity, albedo_.r);
		//for (int i = 0; i < NUM_WAVELENGTHS; i++)
		//{
		//	sunRadianceState[i] = arhosekskymodelstate_alloc_init(elevation, turbidity, albedo_.r);
		//}

		this->albedo = albedo_;
		sunTurbidity = turbidity;
		sunElevation = elevation;

		minValue = FLT_MAX;
		maxValue = -FLT_MAX;
		totalValue = 0.0;
		nSamples = 0;

		sunAltitude = elevation;
		sunAzimuth = azimuth;
		sunInclination = (float)FX_PIOVERTWO - sunAltitude;
		sun[0] = sin(sunInclination) * cos(sunAzimuth);
		sun[1] = sin(sunInclination) * sin(sunAzimuth);
		sun[2] = cos(sunInclination);
		ComputeThetaGamma(sun[0], sun[1], sun[2], &sunTheta, &sunGamma);
	}

	void HosekWilkiePBSky::Delete() {
		try {
			for (int i = 0; i < 3; i++) {
				if (rgbRadianceState[i])
					arhosekskymodelstate_free(rgbRadianceState[i]);
				rgbRadianceState[i] = nullptr;
			}

			if (sunRadianceState)
				arhosekskymodelstate_free(sunRadianceState);
			sunRadianceState = nullptr;
			//for (int i = 0; i < NUM_WAVELENGTHS; i++)
			//{
			//	if (sunRadianceState[i])
			//		arhosekskymodelstate_free(sunRadianceState[i]);
			//	sunRadianceState[i] = nullptr;
			//}
		}
		catch (...) {
			HFLOGERROR("unknown error freeing memory.");
		}
	}

	void HosekWilkiePBSky::resetStatisticSamples() {
		minValue = 1e10;
		maxValue = -1e10;
		totalValue = 0.0;
		nSamples = 0;
	}

	void HosekWilkiePBSky::addStatisticSample(Real amount) {
		if (minValue > amount) {
			minValue = amount;
			//cout << "minValue: " << minValue << std::endl;
		}
		if (maxValue < amount) {
			maxValue = amount;
			//cout << "maxValue: " << maxValue << std::endl;
		}
		totalValue += amount;
		nSamples++;
	}

	void HosekWilkiePBSky::ComputeThetaGamma(Real inclination, Real azimuth, Real* outTheta, Real* outGamma) const {
		Real v[3];
		v[0] = sin(inclination) * cos(azimuth);
		v[1] = sin(inclination) * sin(azimuth);
		v[2] = cos(inclination);
		// Gamma is angle between sun vector and sky element vector which is the dot product
		Real cosine = v[0] * sun[0] + v[1] * sun[1] + v[2] * sun[2];
		*outGamma = acos(cosine);
		*outTheta = inclination;
	}

	void HosekWilkiePBSky::ComputeThetaGamma(Real x, Real y, Real z, Real* outTheta, Real* outGamma) const {
		Real length = sqrtf(x * x + y * y + z * z);
		Real v[3];
		v[0] = x / length;
		v[1] = y / length;
		v[2] = z / length;
		Real cosine = clamp(v[0] * sun[0] + v[1] * sun[1] + v[2] * sun[2], -1.0f, 1.0f);
		Real gamma = acos(cosine);
		Real theta = acos(v[2]);
		if (isfinite(gamma)) {
			*outGamma = gamma;
		}
		else {
			*outGamma = 0.0;
		}
		if (isfinite(theta)) {
			*outTheta = theta;
		}
		else {
			*outTheta = 0.0;
		}
	}

	Real HosekWilkiePBSky::Compute(Real theta, Real gamma, int index) {
		if (index < 0 || index >= 3)
			return 0.0;

		if (theta < 0.0 || theta >= FX_PIOVERTWO || gamma < 0.0)
			return 0.0;

		Real amount = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[index], theta, gamma, index);

		addStatisticSample(amount);

		return amount;
	}

	void HosekWilkiePBSky::ComputeSunRadiance(Real theta, Real gamma, Color4f& output) const {
		Color4f xyz;

		for (int i = 0; i < NUM_WAVELENGTHS; i++) {
			Real amount = theta > FX_PIOVERTWO ? 0.0f : (float)arhosekskymodel_solar_radiance(sunRadianceState, theta, gamma, wavelengths[i]);
			xyz.r += tristimulus[i][0] * amount;
			xyz.g += tristimulus[i][1] * amount;
			xyz.b += tristimulus[i][2] * amount;
		}

		// convert XYZ to sRGB
		Real m[3][3] = {
			{3.2406f, -1.5372f, -0.4986f},
			{-0.9689f, 1.8758f, 0.0415f},
			{0.0557f, -0.2040f, 1.0570f} };

		Color4f rgb_linear(
			xyz.r * m[0][0] + xyz.g * m[0][1] + xyz.b * m[0][2],
			xyz.r * m[1][0] + xyz.g * m[1][1] + xyz.b * m[1][2],
			xyz.r * m[2][0] + xyz.g * m[2][1] + xyz.b * m[2][2],
			0.0f);

		Color4f sRGB(
			rgb_linear.r <= 0.0031308f ? rgb_linear.r * 12.92f : (1 + 0.055f) * powf(rgb_linear.r, 1.0f / 2.4f) - 0.055f,
			rgb_linear.g <= 0.0031308f ? rgb_linear.g * 12.92f : (1 + 0.055f) * powf(rgb_linear.g, 1.0f / 2.4f) - 0.055f,
			rgb_linear.b <= 0.0031308f ? rgb_linear.b * 12.92f : (1 + 0.055f) * powf(rgb_linear.b, 1.0f / 2.4f) - 0.055f,
			0.0f);

		output = sRGB;
	}

	Real HosekWilkiePBSky::ComputeSunRadiance2(Real theta, Real gamma, int index) {
		//return Compute(theta, gamma, index);

		if (theta < 0.0 || theta >= FX_PIOVERTWO || gamma < 0.0)
			return 0.0;
		Real amount;

		Color4f xyz;

		for (int i = 0; i < NUM_WAVELENGTHS; i++) {
			//Real amount = arhosekskymodel_solar_radiance(sunRadianceState[i], theta, gamma, wavelengths[i]);
			amount = (float)arhosekskymodel_inscattered_radiance(sunRadianceState, theta, gamma, wavelengths[i]);
			if (theta < 1.0f)
				amount += (float)arhosekskymodel_sun_direct_radiance(sunRadianceState, theta, gamma, wavelengths[i]);
			//amount = (float)arhosekskymodel_solar_radiance(sunRadianceState[i], theta, gamma, wavelengths[i]);

			if (isfinite(amount))
				addStatisticSample(amount);
			//else std::cout << "BLAH!\n";

			xyz.r += tristimulus[i][0] * amount;
			xyz.g += tristimulus[i][1] * amount;
			xyz.b += tristimulus[i][2] * amount;
		}
		if (index == 0)
			return xyz.r;
		if (index == 1)
			return xyz.g;
		if (index == 2)
			return xyz.b;

		// convert XYZ to sRGB
		Real m[3][3] = {
			{3.2406f, -1.5372f, -0.4986f},
			{-0.9689f, 1.8758f, 0.0415f},
			{0.0557f, -0.2040f, 1.0570f} };

		Color4f rgb_linear(
			xyz.r * m[0][0] + xyz.g * m[0][1] + xyz.b * m[0][2],
			xyz.r * m[1][0] + xyz.g * m[1][1] + xyz.b * m[1][2],
			xyz.r * m[2][0] + xyz.g * m[2][1] + xyz.b * m[2][2],
			0.0f);

		Color4f sRGB(
			rgb_linear.r <= 0.0031308f ? rgb_linear.r * 12.92f : (1 + 0.055f) * powf(rgb_linear.r, 1.0f / 2.4f) - 0.055f,
			rgb_linear.g <= 0.0031308f ? rgb_linear.g * 12.92f : (1 + 0.055f) * powf(rgb_linear.g, 1.0f / 2.4f) - 0.055f,
			rgb_linear.b <= 0.0031308f ? rgb_linear.b * 12.92f : (1 + 0.055f) * powf(rgb_linear.b, 1.0f / 2.4f) - 0.055f,
			0.0f);

		if (index == 0)
			return sRGB.r;
		if (index == 1)
			return sRGB.g;
		if (index == 2)
			return sRGB.b;
		return 0.0f;
	}

	void HosekWilkiePBSky::ComputeSunRadiance3(Real theta, Real gamma, Color4f& output) {
		//return Compute(theta, gamma, index);

		if (theta < 0.0 || theta >= FX_PIOVERTWO || gamma < 0.0)
			return;
		Real amount;

		Color4f xyz;

		for (int i = 0; i < NUM_WAVELENGTHS; i++) {
			//Real amount = arhosekskymodel_solar_radiance(sunRadianceState[i], theta, gamma, wavelengths[i]);
			amount = (float)arhosekskymodel_inscattered_radiance(sunRadianceState, theta, gamma, wavelengths[i]);

			if (isfinite(amount))
				addStatisticSample(amount);

			if (gamma < 0.01f)
				amount += (float)arhosekskymodel_sun_direct_radiance(sunRadianceState, theta, gamma, wavelengths[i]);
			//amount = (float)arhosekskymodel_solar_radiance(sunRadianceState[i], theta, gamma, wavelengths[i]);

			//else std::cout << "BLAH!\n";

			xyz.r += tristimulus[i][0] * amount;
			xyz.g += tristimulus[i][1] * amount;
			xyz.b += tristimulus[i][2] * amount;
		}
		output.r = xyz.r;
		output.g = xyz.g;
		output.b = xyz.b;
	}

	void HosekWilkiePBSky::ComputeSunRadiance4(Real theta, Real gamma, Color4f& output) {
		Color4f xyz;

		// Return 0 if we are outside of the domain of this function.
		if (theta < 0.0 || theta >= FX_PIOVERTWO || gamma < 0.0) {
			output.r = 0.5f;
			output.g = 0.5f;
			output.b = 0.5f;
			output.a = 1.0f;
			return;
		}

		output.r = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[0], theta, gamma, 0);
		output.g = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[1], theta, gamma, 1);
		output.b = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[2], theta, gamma, 2);
		output.a = 1.0f;

		if (isfinite(output.r))
			addStatisticSample(output.r);
		if (isfinite(output.g))
			addStatisticSample(output.g);
		if (isfinite(output.b))
			addStatisticSample(output.b);
	}

	void HosekWilkiePBSky::ComputeSunRadiance4_NoStatistics(Real theta, Real gamma, Color4f& output) const {
		Color4f xyz;

		// Return 0 if we are outside of the domain of this function.
		if (theta < 0.0 || theta >= FX_PIOVERTWO || gamma < 0.0) {
			output.r = 0.0f;
			output.g = 0.0f;
			output.b = 0.0f;
			output.a = 1.0f;
			return;
		}

		output.r = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[0], theta, gamma, 0);
		output.g = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[1], theta, gamma, 1);
		output.b = (float)arhosek_tristim_skymodel_radiance(rgbRadianceState[2], theta, gamma, 2);
		output.a = output.ToVector3().length();
	}

	Color4f HosekWilkiePBSky::GetSunDiskRadiance() const {
		Color4f output;
		//ComputeSunRadiance4_NoStatistics(sunTheta, sunGamma, output);
		ComputeSunRadiance(sunTheta, sunGamma, output);

		if (!isnormal(output.r) || output.r < 0.0f)
			output.r = 0.0f;
		if (!isnormal(output.g) || output.g < 0.0f)
			output.g = 0.0f;
		if (!isnormal(output.b) || output.b < 0.0f)
			output.b = 0.0f;
		if (!isnormal(output.a) || output.a < 0.0f)
			output.a = 0.0f;

		return Color4f(output);
	}

	Color4f HosekWilkiePBSky::GetGroundRadiance() const {
		Color4f sunRadiance = GetSunDiskRadiance();
		float f_r = (float)(1.0 / FX_PI * std::max(0.0f, sun[2]));

		return Color4f(
			sunRadiance.r * albedo.r * f_r,
			sunRadiance.g * albedo.g * f_r,
			sunRadiance.b * albedo.b * f_r,
			1.0);
	}

	// Determines which face should be selected for rendering/getting from
	int classifyCubeFaceFromVector(float x, float y, float z) {
		// ma is absolute value
		float ax = fabs(x);
		float ay = fabs(y);
		float az = fabs(z);
		// signs of ma
		int sx = x > 0 ? 1 : 0;
		int sy = y > 0 ? 1 : 0;
		int sz = z > 0 ? 1 : 0;

		// GL_TEXTURE_CUBE_MAP_POSITIVE_X
		if (sx && ax >= ay && ax >= az)
			return 0;
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		if (!sx && ax >= ay && ax >= az)
			return 1;
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		if (sy && ay >= ax && ay >= az)
			return 2;
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		if (!sy && ay >= ax && ay >= az)
			return 3;
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		if (sz && az >= ax && az >= ay)
			return 4;
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		if (!sz && az >= ax && az >= ay)
			return 5;

		return -1;
	}

	void makeST(float x, float y, float z, float* s, float* t, int* whichFace) {
		// ma is absolute value
		float ax = fabs(x);
		float ay = fabs(y);
		float az = fabs(z);
		// signs of ma
		int sx = x > 0 ? 1 : 0;
		int sy = y > 0 ? 1 : 0;
		int sz = z > 0 ? 1 : 0;

		// 0 | sc = -z | tc = -y | ma = +X | dir: +rx
		// 1 | sc = +z | tc = -y | ma = +X | dir: -rx
		// 2 | sc = +X | tc = +z | ma = +y | dir: +ry
		// 3 | sc = +X | tc = -z | ma = +y | dir: -ry
		// 4 | sc = +X | tc = -y | ma = +z | dir: +rz
		// 5 | sc = -X | tc = -y | ma = +z | dir: -rz
		float ma = 0.0f;
		float sc = 0.0f;
		float tc = 0.0f;
		// GL_TEXTURE_CUBE_MAP_POSITIVE_X
		if (sx && ax >= ay && ax >= az) {
			ma = ax;
			sc = z;
			tc = y;
			*whichFace = 0;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		if (!sx && ax >= ay && ax >= az) {
			ma = ax;
			sc = z;
			tc = y;
			*whichFace = 1;
		}
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		if (sy && ay >= ax && ay >= az) {
			ma = ay;
			sc = x;
			tc = z;
			*whichFace = 2;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		if (!sy && ay >= ax && ay >= az) {
			ma = ay;
			sc = x;
			tc = z;
			*whichFace = 3;
		}
		// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		if (sz && az >= ax && az >= ay) {
			ma = az;
			sc = x;
			tc = y;
			*whichFace = 4;
		}
		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		if (!sz && az >= ax && az >= ay) {
			ma = az;
			sc = x;
			tc = y;
			*whichFace = 5;
		}
		// s = 0.5 * (sc / ma + 1)
		// t = 0.5 * (tc / ma + 1)
		*s = 0.5f * (sc / ma + 1.0f);
		*t = 0.5f * (tc / ma + 1.0f);
	}

	Vector3f makeCubeVector(int face, float s, float t) {
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
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float sc = 2.0f * s - 1;
		float tc = 2.0f * t - 1;

		switch (face) {
		case 0:
			x = 1.0;
			y = tc;
			z = -sc;
			break;
		case 1:
			x = -1.0;
			y = tc;
			z = sc;
			break;
		case 2:
			x = sc;
			y = 1.0;
			z = -tc;
			break;
		case 3:
			x = sc;
			y = -1.0;
			z = tc;
			break;
		case 4:
			x = sc;
			y = tc;
			z = 1.0;
			break;
		case 5:
			x = -sc;
			y = tc;
			z = -1.0;
			break;
		}

		return Vector3f(x, y, z);

		//// Test this by going the other way...
		//float scheck, tcheck;
		//int whichFace;
		//makeST(x, y, z, &scheck, &tcheck, &whichFace);

		//// check if face maps out. =^)
		////whichFace = classifyCubeFaceFromVector(X, y, z);

		//cout << std::setprecision(2) << left;
		//cout << "S: " << std::setw(5) << s << " T: " << std::setw(5) << t << " ";
		//cout << "S: " << std::setw(5) << scheck << " T: " << std::setw(5) << tcheck << " ";
		//cout << "Face: " << face << " ";
		//cout << "<--> " << whichFace << " ";
		//cout << showpos << left << showpoint;
		//cout << "(" << std::setw(7) << x << ", " << std::setw(7) << y << ", " << z << ") ";
		//cout << noshowpos;
		//cout << std::endl;

		//return Vector3f(x, y, z);
	}

	Vector3f makeCubeVector2(int face, float s, float t) {
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
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float sc = 2.0f * s - 1;
		float tc = 2.0f * t - 1;

		switch (face) {
		case 0:
			x = 1.0;
			y = tc;
			z = -sc;
			break;
		case 1:
			x = -1.0;
			y = tc;
			z = sc;
			break;
		case 2:
			x = sc;
			y = 1.0;
			z = -tc;
			break;
		case 3:
			x = sc;
			y = -1.0;
			z = tc;
			break;
		case 4:
			x = sc;
			y = tc;
			z = 1.0;
			break;
		case 5:
			x = -sc;
			y = tc;
			z = -1.0;
			break;
		}

		return Vector3f(x, y, z);

		// TODO: Move this to a separate test

		//if ((rand() % 400) != 1)
		//	return Vector3f(x, y, z);

		//// Test this by going the other way...
		//float scheck, tcheck;
		//int whichFace;
		//makeST(x, y, z, &scheck, &tcheck, &whichFace);

		//// check if face maps out. =^)
		//whichFace = classifyCubeFaceFromVector(x, y, z);

		//std::cout << std::setprecision(2) << std::left;
		//std::cout << "S: " << std::setw(5) << s << " T: " << std::setw(5) << t << " ";
		//std::cout << "S: " << std::setw(5) << scheck << " T: " << std::setw(5) << tcheck << " ";
		//std::cout << "Face: " << face << " ";
		//if (whichFace >= 0)
		//	std::cout << "<--> " << whichFace << " ";
		//else
		//	std::cout << "<--> X ";
		//std::cout << std::showpos << std::left << std::showpoint;
		//std::cout << "(" << std::setw(7) << x << ", " << std::setw(7) << y << ", " << z << ") ";
		//std::cout << std::noshowpos;
		//std::cout << std::endl;

		//return Vector3f(x, y, z);
	}

	/////////////////////////////////////////////////////////////////////
	// P H Y S I C A L L Y   B A S E D   S K Y //////////////////////////
	/////////////////////////////////////////////////////////////////////

	PhysicallyBasedSky::PhysicallyBasedSky() {
		hwpbsky = std::make_unique<HosekWilkiePBSky>();
	}

	PhysicallyBasedSky::~PhysicallyBasedSky() {}

	void PhysicallyBasedSky::SetLocation(float latitude, float longitude) {
		astroCalc.SetLocation(latitude, longitude);
		ComputeSunFromLocale();
	}

	time_t PhysicallyBasedSky::GetTime() const {
		return astroCalc.GetTime();
	}

	void PhysicallyBasedSky::SetTime(time_t t, float fractSeconds) {
		astroCalc.SetTime(t, fractSeconds);
		ComputeSunFromLocale();
	}

	void PhysicallyBasedSky::SetLocalDate(int day, int month, int year, bool isdst, int timeOffset) {
		astroCalc.SetDate(day, month, year, isdst, timeOffset);
		ComputeSunFromLocale();
	}

	void PhysicallyBasedSky::SetCivilDateTime(const Astronomy::PA::CivilDateTime& dtg) {
		astroCalc.SetDateTime(dtg.day, dtg.month, dtg.year, dtg.isdst, dtg.timeZoneOffset, dtg.hh, dtg.mm, dtg.ss, dtg.ss_frac);
		ComputeSunFromLocale();
	}

	void PhysicallyBasedSky::SetLocalTime(int hh, int mm, int ss, float ss_frac) {
		astroCalc.SetTime(hh, mm, ss, ss_frac);
		ComputeSunFromLocale();
	}

	void PhysicallyBasedSky::SetTurbidity(float T) noexcept {
		turbidity = T;
	}

	float PhysicallyBasedSky::GetTurbidity() const noexcept {
		return turbidity;
	}

	void PhysicallyBasedSky::SetSunPosition(double azimuth, double altitude) noexcept {
		sunPosition.A = azimuth;
		sunPosition.a = altitude;
	}

	void PhysicallyBasedSky::SetSunPosition(double sunLong) {
		Astronomy::EclipticCoord sunCoord(sunLong, 0.0);
		sunPosition = astroCalc.ecliptic_to_horizon(sunCoord);
		Astronomy::Vector v = sunPosition.toOpenGLVector();
		sunVector = Vector3f(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z));
	}

	float PhysicallyBasedSky::GetAverageRadiance() const noexcept {
		return hwpbsky->totalValue / hwpbsky->nSamples;
	}

	void PhysicallyBasedSky::ComputeSunFromLocale() noexcept {
		double sunLong = wrap(astroCalc.getSun().lambda, 360.0);
		SetSunPosition(sunLong);
	}

	void PhysicallyBasedSky::ComputeCubeMap(int resolution, bool normalize, float sampleScale, bool flipY) noexcept {
		prepareForCompute();
		generatedCubeMap.resize(resolution, resolution, 6);

		float sampleRadius = nSamples > 1 ? 0.5f / (float)resolution : 0.0f;

		// Loop through each texel and compute (s, t) coordinates such that each
		// ray goes the center of each pixel.
		for (int face = 0; face < 6; face++) {
			for (int is = 0; is < resolution; is++) {
				for (int it = 0; it < resolution; it++) {
					float s = (is + 0.5f) / (float)resolution;
					float t = (it + 0.5f) / (float)resolution;
					Color4f color;

					for (int curSample = 0; curSample < nSamples; curSample++) {
						float sp = s;
						float tp = t;
						if (curSample >= 1) {
							sp += randomSampler(-1.0, 1.0) * sampleRadius;
							tp += randomSampler(-1.0, 1.0) * sampleRadius;
						}

						Vector3f v;
						MakeCubeVectorFromFaceST(face, sp, tp, &v.x, &v.y, &v.z);
						v = v.norm();

						Color4f sampleColor;

						float theta;
						float gamma;
						hwpbsky->ComputeThetaGamma(v.x, -v.z, v.y, &theta, &gamma);

						if (theta >= 0.0f) {
							hwpbsky->ComputeSunRadiance4(theta, gamma, sampleColor);

							sampleColor.r = flterrzero(sampleColor.r * sampleScale);
							sampleColor.g = flterrzero(sampleColor.g * sampleScale);
							sampleColor.b = flterrzero(sampleColor.b * sampleScale);

							//float r = hwpbsky->ComputeSunRadiance2(theta, gamma, 0) * sampleScale;
							//float g = hwpbsky->ComputeSunRadiance2(theta, gamma, 1) * sampleScale;
							//float b = hwpbsky->ComputeSunRadiance2(theta, gamma, 2) * sampleScale;

							//if (isfinite(r) && isfinite(g) && isfinite(b))
							//{
							//	sampleColor.r = r;
							//	sampleColor.g = g;
							//	sampleColor.b = b;
							//}
							//else
							//{
							//	sampleColor.r = 0.0f;
							//	sampleColor.g = 0.0f;
							//	sampleColor.b = 0.0f;
							//}
						}
						else {
							sampleColor = groundRadiance * sampleScale;
						}
						// debug to see if cube map is mapped properly
						//sampleColor.r = 0.5f*ptr.X + 0.5f;
						//sampleColor.g = 0.5f*ptr.y + 0.5f;
						//sampleColor.b = 0.5f*ptr.z + 0.5f;
						//sampleColor.r = sp;
						//sampleColor.g = tp;
						//sampleColor.b = 0.0f;
						color += sampleColor;
					}

					if (nSamples > 1)
						color = color / (float)nSamples;

					// flip the t axis
					if (!flipY)
						generatedCubeMap.setPixelUnsafe(is, (resolution - 1) - it, face, color);
					else
						generatedCubeMap.setPixelUnsafe(is, it, face, color);
				}
			}
		}

		if (normalize) {
			//float average = hwpbsky->totalValue / hwpbsky->nSamples;
			float invScale = 1.0f / (sampleScale * hwpbsky->maxValue);
			//generatedCubeMap.scaleColors(invScale);
			invScale = 1.0f / hwpbsky->GetSunDiskRadiance().ToVector3().length();
			generatedCubeMap.scaleColors(invScale);
		}
		else {
			// float average = hwpbsky->totalValue / hwpbsky->nSamples;
			// float invScale = 1.0f / (sampleScale * hwpbsky->maxValue);
			// generatedCubeMap.scaleColors(invScale);
			// invScale = 1.0f / hwpbsky->GetSunDiskRadiance().ToVector3().length();
			// generatedCubeMap.scaleColors(2.5f * powf(2.0f, -6.0f));
		}

		minRgbValue = hwpbsky->minValue;
		maxRgbValue = hwpbsky->maxValue;
	}

	void PhysicallyBasedSky::ComputeCylinderMap(int width, int height, bool normalize, float sampleScale) noexcept {
		prepareForCompute();
		generatedCylMap.resize(width, height);

		float sampleRadius = nSamples > 1 ? 0.5f : 0.0f;

		// (u, ptr) is the texture_ coordinate in the range [0.0, 1.0]
		// this maps to 0 to 2 * pi and -pi to pi
		// (i, j) are the indices into the texture_ map
		// (theta, gamma) are the coordinates in the sky with (theta, phi) being (theta, pi/2 - gamma)
		//
		int i, j;
		float du = 2.0f / (generatedCylMap.width() - 1.0f); // subtract 1.0 from width to ensure image covers -1.0 to 1.0
		float dv = 2.0f / (generatedCylMap.height() - 1.0f);

		float u = -1.0f;
		for (i = 0; i < width; i++) {
			float v = -1.0f;
			for (j = 0; j < height; j++) {
				Color4f color;

				for (int s = 0; s < nSamples; s++) {
					Color4f sampleColor;
					float us = (float)(u)+randomSampler(-du, du) * sampleRadius;
					float vs = (float)(v)+randomSampler(-dv, dv) * sampleRadius;

					us = clamp<float>(us, -1.0f, 1.0f);
					vs = clamp<float>(vs, -1.0f, 1.0f);

					float inclination = ((1.0f + vs) / 2.0f * (float)FX_PIOVERTWO);
					float azimuth = (1.0f + us) / 2.0f * (float)FX_TWOPI;

					float theta;
					float gamma;
					hwpbsky->ComputeThetaGamma(inclination, azimuth, &theta, &gamma);

					if (theta >= 0.0f) {
						//sampleColor.r = hwpbsky->ComputeSunRadiance2(theta, gamma, 0) * sampleScale;
						//sampleColor.g = hwpbsky->ComputeSunRadiance2(theta, gamma, 1) * sampleScale;
						//sampleColor.b = hwpbsky->ComputeSunRadiance2(theta, gamma, 2) * sampleScale;

						hwpbsky->ComputeSunRadiance4(theta, gamma, sampleColor);

						sampleColor.r = flterrzero(sampleColor.r * sampleScale);
						sampleColor.g = flterrzero(sampleColor.g * sampleScale);
						sampleColor.b = flterrzero(sampleColor.b * sampleScale);
					}
					color += sampleColor;
				}

				if (nSamples > 1)
					color = color / (float)nSamples;

				generatedCylMap.setPixel(i, j, color);
				v += dv;
			}
			u += du;
		}

		if (normalize) {
			float invScale = 1.0f / hwpbsky->maxValue;
			for (i = 0; i < width; i++) {
				for (j = 0; j < height; j++) {
					Color4f color = generatedCylMap.getPixel(i, j);
					color *= invScale;
				}
			}
		}

		minRgbValue = hwpbsky->minValue;
		maxRgbValue = hwpbsky->maxValue;
	}

	//void PhysicallyBasedSky::ComputeSphereMap(int width, int height, bool normalize, float sampleScale)
	//{
	//}

	void PhysicallyBasedSky::ComputeSunGroundRadiances() noexcept {
		prepareForCompute(false);
		sunDiskRadiance = hwpbsky->GetSunDiskRadiance();
		groundRadiance = hwpbsky->GetGroundRadiance();
	}

	void PhysicallyBasedSky::prepareForCompute(bool resetStats) noexcept {
		minRgbValue = FLT_MAX;
		maxRgbValue = -FLT_MAX;
		hwpbsky->Init(turbidity, groundAlbedo, static_cast<float>(sunPosition.a), static_cast<float>(sunPosition.A));
		if (resetStats)
			hwpbsky->resetStatisticSamples();
	}

} // namespace Fluxions