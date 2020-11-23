#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_camera.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions {
	bool SimpleCamera::read(const std::string& keyword, std::istream& istr) {
		if (keyword != this->keyword())
			return false;

		std::string camtype = ReadString(istr);
		projectionMatrix_.LoadIdentity();
		viewMatrix_.LoadIdentity();

		if (camtype == "perspective_otrf") {
			cameraType = CameraType::PerspectiveOTRF;
			fov = ReadFloat(istr);
			origin_ = ReadVector3f(istr);
			target_ = ReadVector3f(istr);
			roll_ = ReadVector3f(istr);
		}
		else if (camtype == "perspective_tmf") {
			cameraType = CameraType::PerspectiveTMF;
			fov = ReadFloat(istr);
			viewMatrix_ = ReadAffineMatrix4f(istr);
		}
		else if (camtype == "ortho_otrw") {
			cameraType = CameraType::OrthoOTRW;
			width = ReadFloat(istr);
			origin_ = ReadVector3f(istr);
			target_ = ReadVector3f(istr);
			roll_ = ReadVector3f(istr);
		}
		else if (camtype == "ortho_tmw") {
			cameraType = CameraType::OrthoTMW;
			width = ReadFloat(istr);
			viewMatrix_ = ReadAffineMatrix4f(istr);
		}
		else {
			statusString_ = "bad!";
			return false;
		}

		statusString_ = camtype;
		recalcMatrices();

		Vector3f O = this->worldMatrix().origin();
		bbox.reset();
		bbox += O;
		bbox += O + 0.5f;
		bbox += O - 0.5f;

		return true;
		//if (!isBadCamera) {
		//	float fstop = 16.0f;
		//	float filmWidth = 35.0f;
		//	float focalDist = 100.0f;
		//	float bokehPolygonalBlades = 3.0f;
		//	float bokehPolygonalRotation = 0.0f;
		//	float regionStartX = 0.0f;
		//	float regionStartY = 0.0f;
		//	float regionEndX = 1.0f;
		//	float regionEndY = 1.0f;
		//	std::string bokehImg;

		//	// read optional components
		//	while (istr) {
		//		istr >> token;
		//		if (str == "fstop") {
		//			fstop = ReadFloat(istr);
		//		}
		//		else if (str == "filmWidth") {
		//			filmWidth = ReadFloat(istr);
		//		}
		//		else if (str == "focalDist") {
		//			focalDist = ReadFloat(istr);
		//		}
		//		else if (str == "bokehPolygonal") {
		//			bokehPolygonalBlades = ReadFloat(istr);
		//			bokehPolygonalRotation = ReadFloat(istr);
		//		}
		//		else if (str == "region") {
		//			regionStartX = ReadFloat(istr);
		//			regionStartY = ReadFloat(istr);
		//			regionEndX = ReadFloat(istr);
		//			regionEndY = ReadFloat(istr);
		//		}
		//		else if (str == "bokehImg") {
		//			bokehImg = ReadString(istr);
		//		}
		//	}

		//	camera.fstop = fstop;
		//	camera.filmWidth = filmWidth;
		//	camera.focalDist = focalDist;
		//	camera.bokehPolygonalBlades = bokehPolygonalBlades;
		//	camera.bokehPolygonalRotation = bokehPolygonalRotation;
		//	camera.bokehImg = bokehImg;
		//	camera.regionStartX = regionStartX;
		//	camera.regionStartY = regionStartY;
		//	camera.regionEndX = regionEndX;
		//	camera.regionEndY = regionEndY;
		//}
	}

	bool SimpleCamera::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		switch (cameraType) {
		case CameraType::PerspectiveOTRF:
			ostr << "perspective_otrf ";
			WriteFloat(ostr, fov);
			WriteVector3f(ostr, origin());
			WriteVector3f(ostr, target());
			WriteVector3f(ostr, roll());
			break;
		case CameraType::PerspectiveTMF:
			ostr << "perspective_tmf ";
			WriteFloat(ostr, fov);
			WriteAffineMatrix4f(ostr, viewMatrix_);
			break;
		case CameraType::OrthoOTRW:
			ostr << "ortho_otrw ";
			WriteFloat(ostr, width);
			WriteVector3f(ostr, origin());
			WriteVector3f(ostr, target());
			WriteVector3f(ostr, roll());
			break;
		case CameraType::OrthoTMW:
			ostr << "ortho_tmw ";
			WriteFloat(ostr, width);
			WriteAffineMatrix4f(ostr, viewMatrix_);
			break;
		case CameraType::Other:
			break;
		}
		ostr << "\n";
		return true;
	}

	void SimpleCamera::recalcMatrices() {
		switch (cameraType) {
		case CameraType::PerspectiveOTRF:
			viewMatrix_ = Matrix4f::MakeLookAt(origin_, target_, roll_);
			projectionMatrix_ = Matrix4f::MakePerspectiveY(fov, imageAspect, imageNearZ, imageFarZ);
			break;
		case CameraType::PerspectiveTMF:
			projectionMatrix_ = Matrix4f::MakePerspectiveY(fov, imageAspect, imageNearZ, imageFarZ);
			break;
		case CameraType::OrthoOTRW:
			viewMatrix_ = Matrix4f::MakeLookAt(origin_, target_, roll_);
			projectionMatrix_ = Matrix4f::MakeOrtho2D(0, width, 0, width / imageAspect);
			break;
		case CameraType::OrthoTMW:
			projectionMatrix_ = Matrix4f::MakeOrtho2D(0, width, 0, width / imageAspect);
			break;
		case CameraType::Other:
			break;
		}
		transform = viewMatrix_.AsInverse();
	}

	const Matrix4f& SimpleCamera::viewMatrix(const Matrix4f& M) {
		transform = M.AsInverse();
		viewMatrix_ = M;
		return viewMatrix_;
	}

	Vector3f SimpleCamera::origin() const noexcept {
		return worldMatrix().col4().xyz();
	}

	Vector3f SimpleCamera::target() const noexcept {
		return origin() - worldMatrix().col3().xyz();
	}

	Vector3f SimpleCamera::roll() const noexcept {
		return worldMatrix().col2().xyz();
	}
} // namespace Fluxions
