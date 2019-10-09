#ifndef FLUXIONS_SIMPLE_CAMERA_HPP
#define FLUXIONS_SIMPLE_CAMERA_HPP

#include <fluxions_gte_matrix4.hpp>

namespace Fluxions
{
	struct SimpleCamera
	{
		bool isOrtho;
		bool isPerspective;
		Matrix4f projectionMatrix;
		Matrix4f viewMatrix;
		Matrix4f actualViewMatrix;
		float fov = 45.0;
		float width = 2.0;
		float fstop = 16.0;
		float filmWidth = 35.0;
		float focalDist = 100.0;
		float bokehPolygonalBlades = 3.0;
		float bokehPolygonalRotation = 0.0;
		float regionStartX = 0.0;
		float regionStartY = 0.0;
		float regionEndX = 1.0;
		float regionEndY = 1.0;
		std::string bokehImg;
		float imageAspect = 1.0;
		float imageWidth = 1024.0;
		float imageHeight = 1024.0;
		float imageX = 0.0;
		float imageY = 0.0;
		float imageNearZ = 0.1f;
		float imageFarZ = 1000.0f;

		void SetImageParameters(float screenWidth, float screenHeight, float znear, float zfar) {
			imageWidth = screenWidth;
			imageHeight = screenHeight;
			imageAspect = screenWidth / screenHeight;
			imageNearZ = znear;
			imageFarZ = zfar;
		}
	}; // struct SimpleCamera
} // namespace Fluxions

#endif