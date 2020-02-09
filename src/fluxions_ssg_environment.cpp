#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_environment.hpp>

namespace Fluxions {
	void async_pbsky_compute(Fluxions::PhysicallyBasedSky* pbsky, bool genCubeMap,
							 bool genCylMap, bool* completed,
							 double* timeElapsed = nullptr) {
		Hf::StopWatch stopwatch;

		if (genCubeMap) {
			pbsky->ComputeCubeMap(64, false, 1.0f);
		}
		if (genCylMap) {
			pbsky->ComputeCylinderMap(128, 32);
			// pbsky.generatedCylMap.savePPM("pbsky.ppm");
		}

		stopwatch.Stop();
		if (timeElapsed != nullptr) {
			*timeElapsed = stopwatch.GetMillisecondsElapsed();
		}
		if (completed != nullptr) {
			*completed = true;
		}
	}

	void SimpleEnvironment::Update(const BoundingBoxf& bbox) {
		pbsky.ComputeSunGroundRadiances();
		sunDirTo = pbsky.sunDirTo().normalize();
		moonDirTo = pbsky.moonDirTo().normalize();
		curGroundRadiance = pbsky.GetGroundRadiance();
		curSunDiskRadiance = pbsky.GetSunDiskRadiance();

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
		//		GLsizei size = (GLsizei)pbsky.generatedCubeMap.width();
		//		void* pixels = pbsky.generatedCubeMap.getPixels(i);
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

	void SimpleEnvironment::ComputePBSky() {
		if (isSkyComputing)
			return;

		isSkyComputed = false;
		isSkyComputing = true;
		lastSkyGenTime = 0.0;
		auto handle =
			async(std::launch::async, async_pbsky_compute, &pbsky, pbskyGenCubeMap,
				  pbskyGenCylMap, &isSkyComputed, &lastSkyGenTime);
	}
}
