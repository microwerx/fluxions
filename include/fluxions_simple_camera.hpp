#ifndef FLUXIONS_SIMPLE_CAMERA_HPP
#define FLUXIONS_SIMPLE_CAMERA_HPP

#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	struct SimpleCamera : public SimpleSceneGraphNode {
		enum class CameraType {
			PerspectiveTMF,
			PerspectiveOTRF,
			OrthoTMW,
			OrthoOTRW,
			Other
		} cameraType;

		Matrix4f actualViewMatrix;
		float actualFov;
		float actualWidth;

		float fov = 45.0;	// horizontal field of view in degrees
		float width = 2.0;
		//float fstop = 16.0;
		//float filmWidth = 35.0;
		//float focalDist = 100.0;
		//float bokehPolygonalBlades = 3.0;
		//float bokehPolygonalRotation = 0.0;
		//float regionStartX = 0.0;
		//float regionStartY = 0.0;
		//float regionEndX = 1.0;
		//float regionEndY = 1.0;
		//std::string bokehImg;
		float imageAspect = 1.0;
		float imageWidth = 1024.0;
		float imageHeight = 1024.0;
		float imageX = 0.0;
		float imageY = 0.0;
		float imageNearZ = 0.1f;
		float imageFarZ = 1000.0f;

		const Matrix4f& viewMatrix(const Matrix4f& M);
		const Matrix4f& viewMatrix() const { return viewMatrix_; }
		const Matrix4f& projectionMatrix() const { return projectionMatrix_; }

		const char* type() const override { return "SimpleCamera"; }
		const char* keyword() const override { return "camera"; }
		Color3f color() const override { return FxColors3::Gold; }

		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;

		void setImageParameters(float screenWidth, float screenHeight, float znear, float zfar) {
			imageWidth = screenWidth;
			imageHeight = screenHeight;
			imageAspect = screenWidth / screenHeight;
			imageNearZ = znear;
			imageFarZ = zfar;
		}

		Vector3f origin() const noexcept;
		Vector3f target() const noexcept;
		Vector3f roll() const noexcept;
	private:
		Vector3f origin_;
		Vector3f target_;
		Vector3f roll_;

		Matrix4f projectionMatrix_;
		Matrix4f viewMatrix_;

		void recalcMatrices();
	}; // struct SimpleCamera
} // namespace Fluxions

#endif