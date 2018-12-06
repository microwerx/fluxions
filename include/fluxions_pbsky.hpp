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
#ifndef FLUXIONS_PBSKY_HPP
#define FLUXIONS_PBSKY_HPP

#include <fluxions_astronomy.hpp>
#include <ArHosekSkyModel.h>
#include <fluxions_gte.hpp>

namespace Fluxions
{
	/////////////////////////////////////////////////////////////////////
	// P H Y S I C A L L Y   B A S E D   S K Y //////////////////////////
	/////////////////////////////////////////////////////////////////////

	struct HosekWilkiePBSky
	{
		using Real = float;
		ArHosekSkyModelState *rgbRadianceState[3];
		ArHosekSkyModelState *sunRadianceState; // 320nm to 720nm in steps of 40nm for wavelength

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
		void ComputeThetaGamma(Real inclination, Real azimuth, Real *outTheta, Real *outGamma) const;
		void ComputeThetaGamma(Real x, Real y, Real z, Real *outTheta, Real *outGamma) const;
		Real Compute(Real theta, Real gamma, int index);
		void ComputeSunRadiance(Real theta, Real gamma, Color4f &output) const;
		Real ComputeSunRadiance2(Real theta, Real gamma, int index);
		// 11 band Solar Radiance
		void ComputeSunRadiance3(Real theta, Real gamma, Color4f &output);
		// 3 band RGB Radiance
		void ComputeSunRadiance4(Real theta, Real gamma, Color4f &output);
		void ComputeSunRadiance4_NoStatistics(Real theta, Real gamma, Color4f &output) const;

		Color4f GetSunDiskRadiance() const;
		Color4f GetGroundRadiance() const;
	};



	class PhysicallyBasedSky
	{
	public:
		PhysicallyBasedSky();
		~PhysicallyBasedSky();

		void SetLocation(float latitude, float longitude);
		float GetLatitude() const { return (float)astroCalc.getLatitude(); }
		float GetLongitude() const { return (float)astroCalc.getLongitude(); }
		const Astronomy::PA::CivilDateTime GetCivilDateTime() const { return astroCalc.GetDateTime(); }
		time_t GetTime() const;
		void SetCivilDateTime(const Astronomy::PA::CivilDateTime & dtg);
		void SetTime(time_t t, float fractSeconds = 0.0f);
		void SetLocalDate(int day, int month, int year, bool isdst, int timeOffset);
		void SetLocalTime(int hh, int mm, int ss, float ss_frac);
		void SetTurbidity(float T) { turbidity = T; }
		float GetTurbidity() const { return turbidity; }
		void SetSunPosition(double azimuth, double altitude) { sunPosition.A = azimuth; sunPosition.a = altitude; }
		void SetSunPosition(double sunLong);
		float GetAverageRadiance() const { return pbsky.totalValue / pbsky.nSamples; }
		float GetSunAzimuth() const { return static_cast<float>(sunPosition.A); }
		float GetSunAltitude() const { return static_cast<float>(sunPosition.a); }
		const Vector3f & GetSunVector() const { return sunVector; }
		void SetGroundAlbedo(float r, float g, float b) { groundAlbedo.r = r; groundAlbedo.g = g; groundAlbedo.b = b; }
		const Color4f & GetGroundAlbedo() const { return groundAlbedo; }
		void SetNumSamples(int samples) { nSamples = clamp(samples, 1, 16); }
		int GetNumSamples() const { return nSamples; }
		void ComputeSunFromLocale();
		void ComputeCubeMap(int resolution, bool normalize = false, float sampleScale = 8.0f, bool flipY = false);
		// not implemented
		void ComputeCylinderMap(int width, int height, bool normalize = false, float sampleScale = 8.0f);
		// not implemented
		void ComputeSphereMap(int width, int height, bool normalize = false, float sampleScale = 8.0f);

		void ComputeSunGroundRadiances();
		Color4f GetSunDiskRadiance() const { return sunDiskRadiance; }
		Color4f GetGroundRadiance() const { return groundRadiance; }

		float getMinRgbValue() const { return minRgbValue; }
		float getMaxRgbValue() const { return maxRgbValue; }

		Image4f generatedCubeMap;
		// unused
		Image4f generatedSphMap;
		// unused
		Image4f generatedCylMap;

		int getDay() { return astroCalc.GetDateTime().day; }
		int getMonth() { return astroCalc.GetDateTime().month; }
		int getYear() { return astroCalc.GetDateTime().year; }
		int getHour() { return astroCalc.GetDateTime().hh; }
		int getMin() { return astroCalc.GetDateTime().mm; }
		int getSec() { return astroCalc.GetDateTime().ss; }
		double getSecFract() { return astroCalc.GetDateTime().ss_frac; }
		double getLST() { return astroCalc.getLST(); }
	private:
		Astronomy::AstroCalc astroCalc;
		Astronomy::HorizonCoord sunPosition;
		Vector3f sunVector;
		float turbidity;
		Color4f groundAlbedo;
		Color4f groundRadiance;
		Color4f sunDiskRadiance;
		int nSamples;
		float minRgbValue;
		float maxRgbValue;
		HosekWilkiePBSky pbsky;

		void prepareForCompute(bool resetStats = true);
	};
}

#endif
