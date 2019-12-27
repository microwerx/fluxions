#ifndef FLUXIONS_SIMPLE_PATH_ANIMATION_HPP
#define FLUXIONS_SIMPLE_PATH_ANIMATION_HPP

#include <fluxions_gte_catmull_rom.hpp>
#include <fluxions_simple_scene_graph_node.hpp>

namespace Fluxions
{
	struct PathKeyframe {

		enum OrientationType {
			Quaternion,
			LookAt,
			AngleAxis,
			AzElTwist,
			EulerXYZ,
			EulerZYX,
			EulerZXY,
		};

		// time of keyframe
		float t = 0.0f;
		// alpha of keyframe for motion
		float a = 1.0f;
		// position of keyframe
		Vector3f position_;
		// orientation of keyframe
		Quaternionf quaternion_;
		
		// orientation data of keyframe depending on orientationType
		Vector3f lookAt_;
		Vector3f azeltwist_;
		Vector3f euler_;
		Vector3f axis_;
		float angle_;

		OrientationType orientationType = OrientationType::Quaternion;

		void setPosition(Vector3f p);
		void setQuaternion(Quaternionf q);
		void setLookAt(Vector3f lookAt);
		void setAzElTwist(Vector3f azeltwist);
		void setEulerXYZ(Vector3f angles);
		void setEulerZYX(Vector3f angles);
		void setEulerZXY(Vector3f angles);
		void setAngleAxis(float angle, Vector3f axis);

		bool read(const std::string& keyword, std::istream& istr);
		bool write(std::ostream& ostr) const;

		Vector3f p() const;
		Quaternionf q() const;

		bool operator<(const PathKeyframe& b) {
			return t < b.t;
		}
	};

	class SimplePathAnimation : public SimpleSceneGraphNode {
	public:
		static constexpr unsigned w = 4;
		static constexpr unsigned h = 4;
		static constexpr unsigned numPoints = 6;

		using Quaternionf = Fluxions::Quaternionf;
		using Vector3f = Fluxions::Vector3f;

		std::vector<PathKeyframe> keyframes;

		const char* type() const override { return "SimplePathAnimation"; }
		const char* keyword() const override { return "path"; }

		// reads format
		// path "name"
		// path t 0         # creates new key frame at time t
		// path a 0         # sets alpha
		// path p x y z
		// path q a b c d                     # sets quaternion
		// path euler yaw pitch roll          # sets industry standard
		// path azel azimuth elevation twist  # az/el/twist
		bool read(const std::string& keyword, std::istream& istr) override;
		bool write(std::ostream& ostr) const override;

		unsigned size() const { return (unsigned)keyframes.size(); }
		void sort() { std::sort(keyframes.begin(), keyframes.end()); }
		void resize(unsigned count);
		void clear();
		void create();

		Vector3f pcatmullrom(float t) const;
		Vector3f plerp(float t) const;
		Quaternionf qslerp(float t) const;
		Quaternionf qsquad(float t) const;

		void set_alpha(float a);

		const PathKeyframe& operator[](int i) const {
			return keyframes[i % keyframes.size()];
		}
	private:
		int readLastIndex = 0;
		float readAlpha = 0.0f;
	};
}

#endif