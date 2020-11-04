#ifndef FLUXIONS_SSG_ENVIRONMENT_HPP
#define FLUXIONS_SSG_ENVIRONMENT_HPP

#include <fluxions_ssg_base.hpp>
#include <starfish-astronomy.hpp>

namespace Fluxions {
	struct SimpleEnvironmentPrivate;

	struct SimpleEnvironment : BaseEnvironment {
		bool hasColor = false;
		bool hasTexmap = false;
		bool hasSun = false;
		bool hasMoon = false;
		Color3f color;
		std::string texmap;
		
		// sun and moon directions as read from file
		Vector3f sunDirTo;
		Vector3f moonDirTo;
		
		// sun and moon directions as rendered
		Vector3f curSunDirTo;
		Vector3f curMoonDirTo;

		Color4f curSunDiskRadiance;
		Color4f curGroundRadiance;
		//float toneMapScale = -6.0f;
		//float toneMapExposure = -6.0f;
		//float toneMapGamma = 1.0f;
		// GLuint curSkyTextureId = 0;
		// GLuint curSkySamplerId = 0;

		Color3f sunColor;
		float sunSize = 1.0;
		GLint isHemisphere = 0;
		//float sunShadowMapDistance = 20.0f;
		//float sunShadowMapZoom = 1.0f;
		//Vector2f sunShadowMapOffset = Vector2f(0.0f, 0.0f);
		//Vector3f sunShadowMapTarget;
		//Vector3f sunShadowMapOrigin;
		//Vector3f sunShadowMapUp;
		//float sunShadowMapNearZ = 1.0f;
		//float sunShadowMapFarZ = 100.0f;
		//float sunShadowMapTime = 0.0;

		//Matrix4f sunShadowBiasMatrix;
		//Matrix4f sunShadowProjectionMatrix;
		//Matrix4f sunShadowViewMatrix;
		//Matrix4f sunShadowInverseViewMatrix;

		//GLuint sunColorMapUnit = 0;
		//GLuint sunColorMapId = 0;
		//GLuint sunColorMapSamplerId = 0;
		//GLuint sunDepthMapUnit = 0;
		//GLuint sunDepthMapId = 0;
		//GLuint sunDepthMapSamplerId = 0;
		//GLuint enviroColorMapUnit = 0;
		//GLuint enviroColorMapId = 0;
		//GLuint enviroColorMapSamplerId = 0;
		//GLuint enviroDepthMapUnit = 0;
		//GLuint enviroDepthMapId = 0;
		//GLuint enviroDepthMapSamplerId = 0;
		//GLuint pbskyColorMapUnit = 0;
		//GLuint pbskyColorMapId = 0;
		//GLuint pbskyColorMapSamplerId = 0;

		SimpleEnvironment();
		~SimpleEnvironment();

		void update(const BoundingBoxf& bbox);
		void computePBSky();
		bool isSkyComputed() const;
		double lastSkyGenTime() const;

		void computeAstroFromLocale();
		Color3f computeModisAlbedo() const;
		Color4f getPixelCubeMap(Vector3f uvw) const;

		void setCivilDateTime(const Sf::PA::CivilDateTime& dtg);
		Sf::PA::CivilDateTime& getCivilDateTime() const;
		time_t getTime() const;
		void setTime(time_t t, float fractSeconds);

		float getLatitude() const;
		float getLongitude() const;
		void setLocation(float latitude, float longitude);

		void setNumSamples(int samples);

		float getTurbidity() const;
		void setTurbidity(float turbidity);

		Color4f getGroundAlbedo() const;
		void setGroundAlbedo(Color3f albedo);

		Color4f getSunDiskRadiance() const;
		Color4f getGroundRadiance() const;

	private:
		SimpleEnvironmentPrivate* pvt{ nullptr };

	};
} // namespace Fluxions

#endif
