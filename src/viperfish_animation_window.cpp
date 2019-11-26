#include "pch.hpp"
#include <viperfish_dear_imgui.hpp>
#include <viperfish_animation_window.hpp>

namespace Vf
{

AnimationWindow::AnimationWindow(const std::string& name)
	: Window(name) {}

AnimationWindow::~AnimationWindow() {

}

void AnimationWindow::OnUpdate(double timeInSeconds) {
	Window::OnUpdate(timeInSeconds);

	if (speed != 0.0f) {
		animation.play();
	}
	else {
		animation.stop();
	}

	scalarGraph.push(animation.scalar());

	animation.update((AnimationTime)GetFrameTime(), speed);
}

void AnimationWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	if (!beginWindow()) return;
	Window::OnRenderDearImGui();

	ImGui::Value("frame", animation.frame());
	ImGui::Value("t", animation.time());
	ImGui::Value("playing", animation.playing());
	ImGui::Value("x", animation.scalar());

	ImGui::Value("keyframes: ", animation.numFrames());

	if (ImGui::Button("|<KF")) { animation.first_frame(); }
	ImGui::SameLine();
	if (ImGui::Button("<KF")) { animation.previous_frame(); }
	ImGui::SameLine();
	if (ImGui::Button("<<")) { speed = -2.0f; }
	ImGui::SameLine();
	if (ImGui::Button("<")) { speed = -1.0f; }
	ImGui::SameLine();
	if (ImGui::Button("[]")) { speed = 0.0f; }
	ImGui::SameLine();
	if (ImGui::Button(">")) { speed = 1.0f; }
	ImGui::SameLine();
	if (ImGui::Button(">>")) { speed = 2.0f; }
	ImGui::SameLine();
	if (ImGui::Button("KF>")) { animation.next_frame(); }
	ImGui::SameLine();
	if (ImGui::Button("KF>|")) { animation.last_frame(); }

	scalarGraph.height = 100.0f;
	scalarGraph.plotLines("scalar");

	float fps = animation.fps();
	ImGui::DragFloat("fps", &fps, 1.0f, 0.0f, 120.0f);
	animation.fps(fps);

	endWindow();
}

Animation::Animation() {
	this->frames.push_back(Keyframe(new FrameCountSource(0.0f), new ParameterSource(-1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(0.25f), new ParameterSource(0.50)));
	this->frames.push_back(Keyframe(new FrameCountSource(0.50f), new ParameterSource(-0.25)));
	this->frames.push_back(Keyframe(new FrameCountSource(0.75f), new ParameterSource(0.75)));
	this->frames.push_back(Keyframe(new FrameCountSource(1.0f), new ParameterSource(-1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(1.25f), new ParameterSource(0.50)));
	this->frames.push_back(Keyframe(new FrameCountSource(1.50f), new ParameterSource(-0.25)));
	this->frames.push_back(Keyframe(new FrameCountSource(1.75f), new ParameterSource(0.75)));
	this->frames.push_back(Keyframe(new FrameCountSource(2.0f), new ParameterSource(-1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(2.25f), new ParameterSource(0.50)));
	this->frames.push_back(Keyframe(new FrameCountSource(2.50f), new ParameterSource(-0.25)));
	this->frames.push_back(Keyframe(new FrameCountSource(2.75f), new ParameterSource(0.75)));
	this->frames.push_back(Keyframe(new FrameCountSource(3.0f), new ParameterSource(-1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(3.25f), new ParameterSource(0.50)));
	this->frames.push_back(Keyframe(new FrameCountSource(3.50f), new ParameterSource(-0.25)));
	this->frames.push_back(Keyframe(new FrameCountSource(3.75f), new ParameterSource(0.75)));
	this->frames.push_back(Keyframe(new FrameCountSource(4.0f), new ParameterSource(0.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(5.0f), new SineParameterSource(1.0, 1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(10.0f), new SineParameterSource(2.0, 2.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(15.0f), new SineParameterSource(3.0, 0.5)));
}

void Animation::first_frame() {
	if (frames.empty()) t_ = 0.0f;
	t_ = frames.front().u->time();
}

void Animation::previous_frame() {
	advance(1.0f / fps_, -1.0f);
}

void Animation::next_frame() {
	advance(1.0f / fps_, 1.0f);
}

void Animation::last_frame() {
	if (frames.empty()) t_ = 0.0f;
	t_ = frames.back().u->time();
}

AnimationScalar Animation::scalar() const {
	if (frames.empty()) {
		return AnimationScalar();
	}
	else if (frames.size() == 1) {
		const Keyframe& frame = frames.front();
		AnimationTime t = t_ - frame.u->time();
		return frame.p->scalar(t);
	}
	else {
		const Keyframe& first = frames.front();
		AnimationTime u1 = first.u->time();
		if (t_ < u1) {
			return first.p->scalar(t_ - u1);
		}

		const Keyframe& last = frames.back();
		AnimationTime u2 = last.u->time();
		if (t_ > u2)
			return last.p->scalar(t_ - u2);

		for (unsigned i = 0; i < frames.size() - 1; i++) {
			const Keyframe& k1 = frames[i];
			const Keyframe& k2 = frames[i + 1];
			u2 = k2.u->time();
			if (t_ <= u2) {
				return interpolator.interpolate_scalar(t_, k1, k2);
			}
		}
	}
	return AnimationScalar();
}

}