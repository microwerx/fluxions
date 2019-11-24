#include "pch.hpp"
#include <viperfish_dear_imgui.hpp>
#include <viperfish_animation_window.hpp>

namespace Vf
{

AnimationWindow::AnimationWindow(const std::string& name)
	: Window(name) {
}

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

	animation.update((AnimationTime)GetFrameTime(), speed);
}

void AnimationWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	if (!beginWindow()) return;
	Window::OnRenderDearImGui();

	ImGui::Value("frame", animation.frame());
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

	float fps = animation.fps();
	ImGui::DragFloat("fps", &fps, 1.0f, 0.0f, 120.0f);
	animation.fps(fps);

	endWindow();
}

Animation::Animation() {
	this->frames.push_back(Keyframe(new FrameCountSource(0.0f), new SineParameterSource(1.0, 1.0)));
	this->frames.push_back(Keyframe(new FrameCountSource(10.0f), new SineParameterSource(2.0, 1.0)));
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
	if (frames.empty()) return AnimationScalar();
	if (frames.size() == 1) {
		const Keyframe& frame = frames.front();
		AnimationTime t = t_ - frame.u->time();
		return frame.p->scalar(t);
	}
	return AnimationScalar();
}

}