#include "pch.hpp"
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_simple_path_animation.hpp>

namespace Fluxions
{
	bool SimplePathAnimation::read(const std::string& keyword, std::istream& istr) {
		// time
		if (keyword == "t") {
			float t = ReadFloat(istr);
			this->keyframes.push_back({ t, readAlpha });
			readLastIndex = (int)keyframes.size() - 1;
			return true;
		}
		// alpha
		else if (keyword == "a") {
			this->keyframes[readLastIndex].a = ReadFloat(istr);
			return true;
		}
		// point
		else if (keyword == "p") {
			this->keyframes[readLastIndex].p = ReadVector3f(istr);
			return true;
		}
		// quaternion
		else if (keyword == "q") {
			this->keyframes[readLastIndex].q = ReadQuaternionf(istr);
			return true;
		}
		// euler angles
		else if (keyword == "euler") {
			float yaw = ReadFloat(istr);
			float pitch = ReadFloat(istr);
			float roll = ReadFloat(istr);
			this->keyframes[readLastIndex].q = Quaternionf::makeFromAngles(yaw, pitch, roll);
			return true;
		}
		// latlon
		else if (keyword == "azel") {
			float az = ReadFloat(istr);
			float el = ReadFloat(istr);
			float twist = ReadFloat(istr);
			this->keyframes[readLastIndex].q = Quaternionf::makeFromAzElTwist(az, el, twist);
			return true;
		}
		else if (keyword == "alpha") {
			float alpha = ReadFloat(istr);
			set_alpha(alpha);
			return true;
		}

		return false;
	}

	bool SimplePathAnimation::write(std::ostream& ostr) {
		ostr << "path " << nodename_ << "\n";
		for (auto& kf : keyframes) {
			ostr << "t " << kf.t << "\n";
			ostr << "a " << kf.a << "\n";
			ostr << "p ";
			WriteVector3f(ostr, kf.p) << "\n";
			ostr << "q ";
			WriteQuaternionf(ostr, kf.q) << "\n";
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
										   keyframes[cp0].p,
										   keyframes[cp1].p,
										   keyframes[cp2].p,
										   keyframes[cp3].p,
										   alpha);
	}

	Fluxions::Vector3f SimplePathAnimation::plerp(float t) const {
		int cp1 = int(t) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		t = t - int(t);
		return (1 - t) * keyframes[cp1].p + t * keyframes[cp2].p;
	}

	Fluxions::Quaternionf SimplePathAnimation::qslerp(float t) const {
		int cp1 = int(t) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		t = t - int(t);
		return Fluxions::slerp(keyframes[cp1].q, keyframes[cp2].q, t).normalized();
	}

	Fluxions::Quaternionf SimplePathAnimation::qsquad(float t) const {
		using Fluxions::slerp;
		int cp1 = int(t) % keyframes.size();
		int cp0 = (cp1 - 1) % keyframes.size();
		int cp2 = (cp1 + 1) % keyframes.size();
		int cp3 = (cp1 + 2) % keyframes.size();
		t = t - int(t);
		Fluxions::Quaternionf q0 = keyframes[cp0].q;
		Fluxions::Quaternionf q1 = keyframes[cp1].q;
		Fluxions::Quaternionf q2 = keyframes[cp2].q;
		Fluxions::Quaternionf q3 = keyframes[cp3].q;
		return Fluxions::squad(q0, q1, q2, q3, t).normalized();
	}

	void SimplePathAnimation::set_alpha(float a) {
		readAlpha = a;
		for (auto& k : keyframes) {
			k.a = a;
		}
	}
}
