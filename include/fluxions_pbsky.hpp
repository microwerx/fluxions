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
		void SetTurbidity(float T) noexcept;
		float GetTurbidity() const noexcept;
		void SetSunPosition(double azimuth, double altitude) noexcept;
		void SetSunPosition(double sunLong);
		float GetAverageRadiance() const noexcept;
		float GetSunAzimuth() const noexcept { return static_cast<float>(sunPosition.A); }
		float GetSunAltitude() const noexcept { return static_cast<float>(sunPosition.a); }
		const Vector3f& GetSunVector() const noexcept { return sunVector; }
		void SetGroundAlbedo(float r, float g, float b) noexcept { groundAlbedo.reset(r, g, b); };
		const Color4f& GetGroundAlbedo() const noexcept { return groundAlbedo; }

		// Compute MODIS Albedo at the specified latitude and longitude, measured in degrees
		Color3f computeModisAlbedo(float latitude, float longitude, float month) const;

		float getDayOfYear() const { return (float)astroCalc.getDayOfYear(); }
		float getMonthOfYear() const { return (float)astroCalc.getMonthOfYear(); }

		void SetNumSamples(int samples) noexcept { nSamples = clamp(samples, 1, 16); }
		int GetNumSamples() const noexcept { return nSamples; }
		void ComputeSunFromLocale() noexcept;
		void ComputeCubeMap(int resolution, bool normalize = false, float sampleScale = 8.0f, bool flipY = false) noexcept;
		// not implemented
		void ComputeCylinderMap(int width, int height, bool normalize = false, float sampleScale = 8.0f) noexcept;
		// not implemented
		//void ComputeSphereMap(int width, int height, bool normalize = false, float sampleScale = 8.0f) noexcept;

		void ComputeSunGroundRadiances() noexcept;
		Color4f GetSunDiskRadiance() const noexcept { return sunDiskRadiance; }
		Color4f GetGroundRadiance() const noexcept { return groundRadiance; }

		float getMinRgbValue() const noexcept { return minRgbValue; }
		float getMaxRgbValue() const noexcept { return maxRgbValue; }

		Image4f generatedCubeMap;
		// unused
		Image4f generatedSphMap;
		// unused
		Image4f generatedCylMap;

		int getDay() const noexcept { return astroCalc.GetDateTime().day; }
		int getMonth() const noexcept { return astroCalc.GetDateTime().month; }
		int getYear() const noexcept { return astroCalc.GetDateTime().year; }
		int getHour() const noexcept { return astroCalc.GetDateTime().hh; }
		int getMin() const noexcept { return astroCalc.GetDateTime().mm; }
		int getSec() const noexcept { return astroCalc.GetDateTime().ss; }
		double getSecFract() const noexcept { return astroCalc.GetDateTime().ss_frac; }
		double getLST() const noexcept { return astroCalc.getLST(); }

	private:
		Astronomy::AstroCalc astroCalc;
		Astronomy::HorizonCoord sunPosition;
		Vector3f sunVector;
		float turbidity = 1.0f;
		Color4f groundAlbedo;
		Color4f groundRadiance;
		Color4f sunDiskRadiance;
		int nSamples;
		float minRgbValue;
		float maxRgbValue;
		std::unique_ptr<HosekWilkiePBSky> hwpbsky;

		void prepareForCompute(bool resetStats = true) noexcept;
	};
} // namespace Fluxions

#endif
