#ifndef VIPERFISH_ANIMATION_WINDOW_HPP
#define VIPERFISH_ANIMATION_WINDOW_CPP

#include <fluxions_gte_transformation.hpp>
#include <viperfish_window.hpp>
#include <viperfish_stats_window.hpp>

namespace Vf
{

	using AnimationTime = float;
	using AnimationFrame = float;
	using AnimationScalar = float;
	using AnimationVector = Fluxions::Vector3f;
	using AnimationMatrix = Fluxions::Matrix4f;
	using AnimationTransformation = Fluxions::Transformationf;
	static constexpr AnimationTime MaxAnimationTime = float(24 * 60 * 60);

	struct FrameCountSource {
		FrameCountSource(AnimationTime _t = 0) : t(_t) {}
		virtual ~FrameCountSource() {}

		virtual AnimationTime time() const {
			return t;
		}

		AnimationTime t;
	};

	struct ParameterSource {
		ParameterSource(AnimationScalar _x = 0) : x(_x) {}
		virtual ~ParameterSource() {}

		virtual AnimationScalar scalar(AnimationTime t) const {
			return x;
		}

		virtual AnimationVector vector(AnimationTime t) const {
			return V;
		}

		virtual AnimationMatrix matrix(AnimationTime t) const {
			return M;
		}

		virtual AnimationTransformation transformation(AnimationTime t) const {
			return T;
		}

		AnimationScalar x;
		AnimationVector V;
		AnimationMatrix M;
		AnimationTransformation T;
	};

	struct SineParameterSource : public ParameterSource {
		SineParameterSource(float frequency, float amplitude)
			: f(frequency), A(amplitude) {}

		virtual AnimationScalar scalar(AnimationTime t) const override {
			return A * sin(f * t * FX_F32_TWOPI);
		}

		AnimationScalar f;
		AnimationScalar A;
	};

	struct Keyframe {
		Keyframe(FrameCountSource* _u, ParameterSource* _p)
			: u(_u), p(_p) {}
		~Keyframe() {}

		std::shared_ptr<FrameCountSource> u;
		std::shared_ptr<ParameterSource> p;
	};

	struct Interpolator {
		Interpolator() {}
		virtual ~Interpolator() {}

		virtual float interpolate_scalar(AnimationTime u, const Keyframe& keyframe1, const Keyframe& keyframe2) const {
			AnimationTime u1 = keyframe1.u->time();
			AnimationTime u2 = keyframe2.u->time();
			//AnimationTime t = (u - u1) / (u2 - u1);
			if (u < u2) return keyframe1.p->scalar(u - u1);
			if (u >= u2) return keyframe2.p->scalar(u - u2);
		}
	};

	struct LinearInterpolator : public Interpolator {
		virtual ~LinearInterpolator() {}

		virtual float interpolate_scalar(AnimationTime u, const Keyframe& keyframe1, const Keyframe& keyframe2) const override {
			AnimationTime u1 = keyframe1.u->time();
			AnimationTime u2 = keyframe2.u->time();
			AnimationTime t = (u - u1) / (u2 - u1);
			if (u < u1) return keyframe1.p->scalar(0);
			if (u >= u2) return keyframe2.p->scalar(0);
			AnimationScalar a = keyframe1.p->scalar(u - u1);
			AnimationScalar b = keyframe2.p->scalar(0);
			return (1 - t) * a + t * b;
		}
	};

	struct CubicInterpolator : public Interpolator {
		virtual ~CubicInterpolator() {}

		virtual float interpolate_scalar(AnimationTime u, const Keyframe& keyframe1, const Keyframe& keyframe2) const override {
			AnimationTime u1 = keyframe1.u->time();
			AnimationTime u2 = keyframe2.u->time();
			AnimationTime t = (u - u1) / (u2 - u1);
			if (u < u1) return keyframe1.p->scalar(0);
			if (u >= u2) return keyframe2.p->scalar(0);
			AnimationScalar a = keyframe1.p->scalar(u - u1);
			AnimationScalar b = keyframe2.p->scalar(0);
			return (1 - t) * a + t * b;
		}
	};

	class Animation {
	public:
		Animation();

		void first_frame();
		void previous_frame();
		void stop() { playing_ = false; }
		void play() { playing_ = true; }
		void next_frame();
		void last_frame();

		void update(AnimationTime timeInSeconds, float speed = 1.0f) {
			if (playing_) { advance(timeInSeconds, speed); }
		}

		AnimationFrame advance(AnimationTime timeInSeconds, float speed = 1.0f) {
			t_ = Fluxions::clamp(t_ + (timeInSeconds * speed), 0.0f, MaxAnimationTime);
			return frame();
		}

		bool stopped() const { return playing_ == false; }
		bool playing() const { return playing_; }
		float frame() const { return t_ * fps_; }
		float time() const { return t_; }
		float fps() const { return fps_; }
		void fps(float fps) { fps_ = Fluxions::clamp(fps, 0.0f, MaxAnimationTime); }

		int numFrames() const { return (int)frames.size(); }

		AnimationScalar scalar() const;
	private:
		// AnimationFrame frame_ = 0.0f;
		AnimationTime t_ = 0.0f;
		float fps_ = 30.0f;
		bool playing_ = false;
		std::vector<Keyframe> frames;
		LinearInterpolator interpolator;
	};

	class AnimationWindow : public Window {
	public:
		AnimationWindow(const std::string& name);
		virtual ~AnimationWindow();

		void OnUpdate(double timeInSeconds) override;
		void OnRenderDearImGui() override;

		Animation animation;
	private:
		float speed = 0.0f;
		FloatStat scalarGraph{ 100,-2.0f, 2.0f };
	};

	using AnimationWindowPtr = std::shared_ptr<AnimationWindow>;

}

#endif