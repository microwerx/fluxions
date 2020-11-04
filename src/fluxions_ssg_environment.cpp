#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_environment.hpp>
#include <starfish.hpp>


namespace Fluxions {
	struct SimpleEnvironmentPrivate {
		Sf::PhysicallyBasedSky pbsky;
		Sf::PA::CivilDateTime pbsky_dtg;
		bool genCubeMap{ true };
		bool genCylMap{ false };
		bool isSkyComputing{ false };
		bool isSkyComputed{ false };
		double lastSkyGenTime{ 0.0 };
	};


	//void async_pbsky_compute(Sf::PhysicallyBasedSky* pbsky, bool genCubeMap,
	//						 bool genCylMap, bool* completed,
	//						 double* timeElapsed = nullptr) {
	void async_pbsky_compute(SimpleEnvironmentPrivate* pvt) {
		Hf::StopWatch stopwatch;

		if (pvt->genCubeMap) {
			pvt->pbsky.computeCubeMap(64, false, 1.0f);
		}
		if (pvt->genCylMap) {
			pvt->pbsky.computeCylinderMap(128, 32);
			// pvt->pbsky.generatedSunCylMap.savePPM("pbsky.ppm");
		}

		stopwatch.Stop();
		pvt->lastSkyGenTime = stopwatch.GetMillisecondsElapsed();
		pvt->isSkyComputed = true;
	}


	SimpleEnvironment::SimpleEnvironment() :
		pvt(new SimpleEnvironmentPrivate) {}


	SimpleEnvironment::~SimpleEnvironment() {
		delete pvt;
	}


	void SimpleEnvironment::update(const BoundingBoxf& bbox) {
		pvt->pbsky.computeSunGroundRadiances();
		sunDirTo = pvt->pbsky.sunDirTo().normalize();
		moonDirTo = pvt->pbsky.moonDirTo().normalize();
		curGroundRadiance = pvt->pbsky.getGroundRadiance();
		curSunDiskRadiance = pvt->pbsky.getSunDiskRadiance();

		if (hasSun) {
			curSunDirTo = sunDirTo.normalize();
		}
		if (hasMoon) {
			curMoonDirTo = moonDirTo.normalize();
		}

		if (within(bbox.MaxSize(), 0.0f, 1000.0f)) {
			// const float padding = 1.0f;
			// const float size = bbox.MaxSize();
			// float correctRadius = sqrtf(0.75f) * boundingBox.MaxSize();
			// float correctBase = correctRadius;
			// float correctHeight = sqrt(1.0f - powf(sinf(fov / 2.0f), 2.0f)) * 4.0f *
			// correctRadius; float fov = 45.0f * FX_F32_DEGREES_TO_RADIANS;

			const float radius = bbox.RadiusRounded();
			//sunShadowMapTarget = bbox.Center();
			//sunShadowMapDistance =
			//	radius; // calculation for perspective: sqrt(1.0f - powf(sinf(fov
			//			// / 2.0f), 2.0f)) * 4.0f * radius;
			//sunShadowMapNearZ = sunShadowMapDistance - radius;
			//sunShadowMapFarZ = sunShadowMapDistance + radius;
			//sunShadowMapOrigin = curSunDirTo.unit() * sunShadowMapDistance + sunShadowMapTarget;
			//sunShadowMapUp = ((curSunDirTo.cross(Vector3f(0.0f, 1.0f, 0.0f))).cross(curSunDirTo)).unit();
		}
		else {
			//sunShadowMapUp.reset(0.0f, 1.0f, 0.0f);
			//sunShadowMapOrigin = curSunDirTo.unit() * 20.0f;
			//sunShadowMapTarget.reset(0.0f, 0.0f, 0.0f);
			//sunShadowMapUp =
			//	((Vector3f(0.0f, 1.0f, 0.0f).cross(curSunDirTo)).cross(curSunDirTo))
			//	.unit();
			//sunShadowMapNearZ = 1.0f;
			//sunShadowMapFarZ = 100.0f;
		}
		//sunShadowProjectionMatrix.LoadIdentity();
		//// sunShadowProjectionMatrix.Perspective(45.0f, 1.0f, sunShadowMapNearZ,
		//// sunShadowMapFarZ);
		//sunShadowProjectionMatrix.Ortho(
		//	sunShadowMapZoom * (-sunShadowMapDistance + sunShadowMapOffset.x),
		//	sunShadowMapZoom * (sunShadowMapDistance + sunShadowMapOffset.x),
		//	sunShadowMapZoom * (-sunShadowMapDistance + sunShadowMapOffset.y),
		//	sunShadowMapZoom * (sunShadowMapDistance + sunShadowMapOffset.y),
		//	sunShadowMapNearZ, sunShadowMapFarZ);
		//sunShadowViewMatrix.LoadIdentity();
		//sunShadowViewMatrix.LookAt(sunShadowMapOrigin, sunShadowMapTarget,
		//						   sunShadowMapUp);
		//sunShadowInverseViewMatrix = sunShadowViewMatrix.AsInverse();

		//if (isSkyComputed && pbskyColorMapId != 0) {
		//	FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, pbskyColorMapId);
		//	for (int i = 0; i < 6; i++) {
		//		GLsizei size = (GLsizei)pbsky.generatedSunCubeMap.width();
		//		void* pixels = pbsky.generatedSunCubeMap.getPixels(i);
		//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, size,
		//					 size, 0, GL_RGBA, GL_FLOAT, pixels);
		//	}
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//	FxGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		//	isSkyComputed = false;
		//	isSkyComputing = false;
		//}
	}


	void SimpleEnvironment::computePBSky() {
		if (pvt->isSkyComputing)
			return;

		pvt->isSkyComputed = false;
		pvt->isSkyComputing = true;
		pvt->lastSkyGenTime = 0.0;
		auto handle = async(std::launch::async, async_pbsky_compute, pvt);
		//async(std::launch::async, async_pbsky_compute,
			//	  &pvt->pbsky,
			//	  pvt->pbskyGenCubeMap,
			//	  pvt->pbskyGenCylMap,
			//	  &pvt->isSkyComputed,
			//	  &pvt->lastSkyGenTime);
	}


	bool SimpleEnvironment::isSkyComputed() const {
		return pvt->isSkyComputed;
	}


	double SimpleEnvironment::lastSkyGenTime() const {
		return pvt->lastSkyGenTime;
	}


	void SimpleEnvironment::computeAstroFromLocale() {
		pvt->pbsky.computeAstroFromLocale();
	}


	Color3f SimpleEnvironment::computeModisAlbedo() const {
		return pvt->pbsky.computeModisAlbedo(true);
	}


	void SimpleEnvironment::setTime(time_t t, float fractSeconds) {
		pvt->pbsky.setTime(t, fractSeconds);
	}


	void SimpleEnvironment::setCivilDateTime(const Sf::PA::CivilDateTime& dtg) {
		pvt->pbsky.setCivilDateTime(dtg);
	}


	Sf::PA::CivilDateTime& SimpleEnvironment::getCivilDateTime() const {
		return pvt->pbsky_dtg;
	}


	void SimpleEnvironment::setNumSamples(int samples) {
		pvt->pbsky.setNumSamples(samples);
	}


	float SimpleEnvironment::getTurbidity() const {
		return pvt->pbsky.getTurbidity();
	}


	void SimpleEnvironment::setTurbidity(float turbidity) {
		pvt->pbsky.setTurbidity(turbidity);
	}


	Color4f SimpleEnvironment::getGroundAlbedo() const {
		return pvt->pbsky.getGroundAlbedo();
	}


	void SimpleEnvironment::setGroundAlbedo(Color3f albedo) {
		pvt->pbsky.setGroundAlbedo(albedo.r,
								   albedo.g,
								   albedo.b);
	}


	time_t SimpleEnvironment::getTime() const {
		return pvt->pbsky.getTime();
	}


	float SimpleEnvironment::getLatitude() const {
		return pvt->pbsky.getLatitude();
	}


	float SimpleEnvironment::getLongitude() const {
		return pvt->pbsky.getLongitude();
	}


	void SimpleEnvironment::setLocation(float latitude, float longitude) {
		pvt->pbsky.setLocation(latitude, longitude);
	}


	Color4f SimpleEnvironment::getSunDiskRadiance() const {
		return pvt->pbsky.getSunDiskRadiance();
	}


	Color4f SimpleEnvironment::getGroundRadiance() const {
		return pvt->pbsky.getGroundRadiance();
	}
}
