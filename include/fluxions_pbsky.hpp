#ifndef FLUXIONS_PBSKY_HPP
#define FLUXIONS_PBSKY_HPP

#include <fluxions_base.hpp>
#include <fluxions_astronomy.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_gte_image.hpp>

namespace Fluxions {
	/////////////////////////////////////////////////////////////////////
	// P H Y S I C A L L Y   B A S E D   S K Y //////////////////////////
	/////////////////////////////////////////////////////////////////////

	class HosekWilkiePBSky;

	class PhysicallyBasedSky {
	public:
		PhysicallyBasedSky();
		~PhysicallyBasedSky();

		void SetLocation(float latitude, float longitude);
		float GetLatitude() const { return (float)astroCalc.getLatitude(); }
		float GetLongitude() const { return (float)astroCalc.getLongitude(); }
		const Astronomy::PA::CivilDateTime GetCivilDateTime() const { return astroCalc.GetDateTime(); }
		time_t GetTime() const;
		void SetCivilDateTime(const Astronomy::PA::CivilDateTime& dtg);
		void SetTime(time_t t, float fractSeconds = 0.0f);
		void SetLocalDate(int day, int month, int year, bool isdst, int timeOffset);
		void SetLocalTime(int hh, int mm, int ss, float ss_frac);
		void SetTurbidity(float T);
		float GetTurbidity() const;
		void setSunPosition(double azimuth, double altitude);
		void setSunPosition(double sunLong);
		void setMoonPosition(double RA, double dec);
		void setMoonPosition(Astronomy::EquatorialCoord moonRADec);
		float GetAverageRadiance() const;
		float GetSunAzimuth() const { return static_cast<float>(sunPosition.A); }
		float GetSunAltitude() const { return static_cast<float>(sunPosition.a); }
		void SetGroundAlbedo(float r, float g, float b) { groundAlbedo.reset(r, g, b); };
		const Color4f& GetGroundAlbedo() const { return groundAlbedo; }

		// Compute MODIS Albedo at the specified latitude and longitude, measured in degrees
		Color3f computeModisAlbedo(float latitude, float longitude, float month) const;

		float getDayOfYear() const { return (float)astroCalc.getDayOfYear(); }
		float getMonthOfYear() const { return (float)astroCalc.getMonthOfYear(); }

		void SetNumSamples(int samples) { nSamples = clamp(samples, 1, 16); }
		int GetNumSamples() const { return nSamples; }
		void computeSunFromLocale();
		void computeMoonFromLocale();
		void ComputeCubeMap(int resolution, bool normalize = false, float sampleScale = 8.0f, bool flipY = false);
		// not implemented
		void ComputeCylinderMap(int width, int height, bool normalize = false, float sampleScale = 8.0f);
		// not implemented
		//void ComputeSphereMap(int width, int height, bool normalize = false, float sampleScale = 8.0f);

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

		int getDay() const { return astroCalc.GetDateTime().day; }
		int getMonth() const { return astroCalc.GetDateTime().month; }
		int getYear() const { return astroCalc.GetDateTime().year; }
		int getHour() const { return astroCalc.GetDateTime().hh; }
		int getMin() const { return astroCalc.GetDateTime().mm; }
		int getSec() const { return astroCalc.GetDateTime().ss; }
		double getSecFract() const { return astroCalc.GetDateTime().ss_frac; }
		double getLST() const { return astroCalc.getLST(); }

		Vector3f sunDirTo() const { return  sunVector_; }
		Vector3f moonDirTo() const { return moonVector_; }
	private:
		Astronomy::AstroCalc astroCalc;
		Astronomy::HorizonCoord sunPosition;
		Astronomy::EquatorialCoord moonPosition;
		Vector3f sunVector_;
		Vector3f moonVector_;
		float turbidity = 1.0f;
		Color4f groundAlbedo;
		Color4f groundRadiance;
		Color4f sunDiskRadiance;
		int nSamples;
		float minRgbValue;
		float maxRgbValue;
		std::unique_ptr<HosekWilkiePBSky> hwpbsky;

		void prepareForCompute(bool resetStats = true);
	};
} // namespace Fluxions

#endif
