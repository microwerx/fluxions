#include "fluxions_ssg_pch.hpp"
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_ssg_path_animation.hpp>

namespace Fluxions {
	Vector3f PathKeyframe::p() const {
		return position_;
	};

	Quaternionf PathKeyframe::q() const {
		switch (orientationType) {
		case OrientationType::Quaternion:
			return quaternion_;
		case OrientationType::LookAt:
			return Quaternionf::makeFromLookDir(lookAt_ - position_, Vector3d(0, 1, 0));
		case OrientationType::AzElTwist:
			return Quaternionf::makeFromAzElTwist(azeltwist_.x, azeltwist_.y, azeltwist_.z);
		case OrientationType::AngleAxis:
			return Quaternionf::makeFromAngleAxis(angle_, axis_.x, axis_.y, axis_.z);
		case OrientationType::EulerXYZ:
			return Quaternionf::makeFromEulerXYZ(euler_.x, euler_.y, euler_.z);
		case OrientationType::EulerZXY:
			return Quaternionf::makeFromEulerZXY(euler_.x, euler_.y, euler_.z);
		case OrientationType::EulerZYX:
			return Quaternionf::makeFromEulerZYX(euler_.x, euler_.y, euler_.z);
		}
		return quaternion_;
	}

	void PathKeyframe::setPosition(Vector3f p) {
		position_ = p;
	}

	void PathKeyframe::setQuaternion(Quaternionf q) {
		quaternion_ = q;
		orientationType = OrientationType::Quaternion;
	}

	void PathKeyframe::setLookAt(Vector3f lookAt) {
		lookAt_ = lookAt;
		orientationType = OrientationType::LookAt;
	}

	void PathKeyframe::setAzElTwist(Vector3f azeltwist) {
		azeltwist_ = azeltwist;
		orientationType = OrientationType::AzElTwist;
	}

	void PathKeyframe::setEulerXYZ(Vector3f angles) {
		euler_ = angles;
		orientationType = OrientationType::EulerXYZ;
	}

	void PathKeyframe::setEulerZYX(Vector3f angles) {
		euler_ = angles;
		orientationType = OrientationType::EulerZYX;
	}

	void PathKeyframe::setEulerZXY(Vector3f angles) {
		euler_ = angles;
		orientationType = OrientationType::EulerZXY;
	}

	void PathKeyframe::setAngleAxis(float angle, Vector3f axis) {
		angle_ = angle;
		axis_ = axis;
		orientationType = OrientationType::LookAt;
	}

	bool PathKeyframe::read(const std::string& keyword, std::istream& istr) {
		// time
		if (keyword == "t") {
			t = ReadFloat(istr);
			return true;
		}
		// alpha
		else if (keyword == "a") {
			a = ReadFloat(istr);
			return true;
		}
		// point
		else if (keyword == "p") {
			position_ = ReadVector3f(istr);
			return true;
		}
		// point
		if (keyword == "p") {
			position_ = ReadVector3f(istr);
			return true;
		}
		// quaternion
		else if (keyword == "q") {
			setQuaternion(ReadQuaternionf(istr));
			return true;
		}
		// euler angles
		else if (keyword == "eulerxyz") {
			setEulerXYZ(ReadVector3f(istr));
			return true;
		}
		// euler angles
		else if (keyword == "eulerzyx") {
			setEulerZYX(ReadVector3f(istr));
			return true;
		}
		// euler angles
		else if (keyword == "eulerzxy") {
			setEulerZXY(ReadVector3f(istr));
			return true;
		}
		// latlon
		else if (keyword == "azel") {
			setAzElTwist(ReadVector3f(istr));
			return true;
		}
		else if (keyword == "lookat") {
			setLookAt(ReadVector3f(istr));
			return true;
		}
		else if (keyword == "angleaxis") {
			setAngleAxis(ReadFloat(istr), ReadVector3f(istr));
			return true;
		}

		return false;
	}

	bool PathKeyframe::write(std::ostream& ostr) const {
		ostr << "t " << t << "\n";
		ostr << "a " << a << "\n";
		ostr << "p ";
		WriteVector3f(ostr, position_) << "\n";
		switch (orientationType) {
		case Quaternion:
			ostr << "q ";
			WriteQuaternionf(ostr, quaternion_);
			break;
		case LookAt:
			ostr << "lookat ";
			WriteVector3f(ostr, lookAt_);
			break;
		case AngleAxis:
			ostr << "angleaxis ";
			WriteFloat(ostr, angle_);
			WriteVector3f(ostr, axis_);
			break;
		case AzElTwist:
			ostr << "azeltwist ";
			WriteVector3f(ostr, azeltwist_);
			break;
		case EulerXYZ:
			ostr << "eulerxyz ";
			WriteVector3f(ostr, euler_);
			break;
		case EulerZYX:
			ostr << "eulerzyx ";
			WriteVector3f(ostr, euler_);
			break;
		case EulerZXY:
			ostr << "eulerzxy ";
			WriteVector3f(ostr, euler_);
			break;
		}
		ostr << "\n";
		return true;
	}

	bool SimplePathAnimation::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			setName(ReadString(istr));
			return true;
		}
		// time
		else if (keyword == "t") {
			float t = ReadFloat(istr);
			keyframes.push_back({ t, readAlpha });
			readLastIndex = (int)keyframes.size() - 1;
			return true;
		}
		else if (keyword == "alpha") {
			readAlpha = ReadFloat(istr);
			return true;
		}
		else if (keyframes.size()) {
			return keyframes.back().read(keyword, istr);
		}
		return false;
	}

	bool SimplePathAnimation::write(std::ostream& ostr) const {
		SimpleSceneGraphNode::write(ostr);
		for (auto& kf : keyframes) {
			kf.write(ostr);
		}
		return true;
	}

	void SimplePathAnimation::resize(unsigned count) {
		keyframes.resize(count);
	}

	void SimplePathAnimation::clear() {
		keyframes.clear();
	}

	void SimplePathAnimation::create() {
		clear();
		resize(w * h);

		using Fluxions::randomSampler;
		constexpr float xsize = 10.0f;
		constexpr float ysize = 5.0f;
		Vector3f lastp;
		for (int i = 0; i < (int)size(); i++) {
			float x = (float)(i % w) * xsize;
			float y = (float)(i / w) * xsize;
			float s = 0.25f;
			Vector3f p(1 + x + 0.5f, 3.0f, 1 + y + 0.5f);

			Vector3f cp = p + Vector3f(randomSampler(-s, s) * xsize,
									   randomSampler(-s, s) * ysize,
									   randomSampler(-s, s) * xsize);
			float ca = 1.0f;
			Quaternionf cq = Quaternionf::makeFromAngles(randomSampler(-180, 180.0),
														 randomSampler(-30.0, 0.0),
														 randomSampler(-15.0, 15.0));
			float ct = 0.0f;
			if (i > 0) {
				ct = (lastp - p).length();
			}
			lastp = p;
			PathKeyframe kf{ ct, ca, cp, cq };
			keyframes[i] = kf;
		}
	}

	Fluxions::Vector3f SimplePathAnimation::pcatmullrom(float t) const {
		const unsigned size = (unsigned)keyframes.size();
		int cp1 = int(t) % keyframes.size();
		int cp0 = (cp1 - 1) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		int cp3 = (cp1 + 2) % keyframes.size();
		t = t - int(t);
		float alpha = Fluxions::lerp(t, keyframes[cp1].a, keyframes[cp2].a);
		return Fluxions::CatmullRomSegment(t,
										   keyframes[cp0].p(),
										   keyframes[cp1].p(),
										   keyframes[cp2].p(),
										   keyframes[cp3].p(),
										   alpha);
	}

	Fluxions::Vector3f SimplePathAnimation::plerp(float t) const {
		int cp1 = int(t) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		t = t - int(t);
		return (1 - t) * keyframes[cp1].p() + t * keyframes[cp2].p();
	}

	Fluxions::Quaternionf SimplePathAnimation::qslerp(float t) const {
		int cp1 = int(t) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		t = t - int(t);
		return slerp(keyframes[cp1].q(), keyframes[cp2].q(), t).normalized();
	}

	Fluxions::Quaternionf SimplePathAnimation::qsquad(float t) const {
		using Fluxions::slerp;
		int cp1 = int(t) % keyframes.size();
		int cp0 = (cp1 - 1) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		int cp3 = (cp1 + 2) % keyframes.size();
		t = t - int(t);
		Fluxions::Quaternionf q0 = keyframes[cp0].q();
		Fluxions::Quaternionf q1 = keyframes[cp1].q();
		Fluxions::Quaternionf q2 = keyframes[cp2].q();
		Fluxions::Quaternionf q3 = keyframes[cp3].q();
		return Fluxions::squad(q0, q1, q2, q3, t).normalized();
	}

	void SimplePathAnimation::set_alpha(float a) {
		readAlpha = a;
		for (auto& k : keyframes) {
			k.a = a;
		}
	}
} // namespace Fluxions
