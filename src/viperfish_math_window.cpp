#include "pch.hpp"
#include <sstream>
#include <viperfish.hpp>
#include <viperfish_math_window.hpp>
#include <fluxions_gte_noise.hpp>

namespace Vf
{

MathWindow::MathWindow(const std::string& name)
	: Window(name) {}

MathWindow::~MathWindow() {}

void MathWindow::OnUpdate(double timeStamp) {
	Window::OnUpdate(timeStamp);
}

void MathWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	if (!beginWindow()) return;
	Window::OnRenderDearImGui();

	Quaternionf qX = Quaternionf::makeFromAngleAxis(X, 1.0, 0.0, 0.0);
	Quaternionf qY = Quaternionf::makeFromAngleAxis(Y, 0.0, 1.0, 0.0);
	Quaternionf qZ = Quaternionf::makeFromAngleAxis(Z, 0.0, 0.0, 1.0);

	q = qZ * qX * qY;

	ImGui::Value("a", q.a);
	ImGui::SameLine();
	ImGui::Value("b", q.b);
	ImGui::SameLine();
	ImGui::Value("c", q.c);
	ImGui::SameLine();
	ImGui::Value("d", q.d);

	ImGui::Value("a", q2.a);
	ImGui::SameLine();
	ImGui::Value("b", q2.b);
	ImGui::SameLine();
	ImGui::Value("c", q2.c);
	ImGui::SameLine();
	ImGui::Value("d", q2.d);

	ImGui::Value("x", p2.x);
	ImGui::SameLine();
	ImGui::Value("y", p2.y);
	ImGui::SameLine();
	ImGui::Value("z", p2.z);

	ImGui::SliderFloat("X", &X, -360.0f, 360.0f);
	ImGui::SliderFloat("Y", &Y, -360.0f, 360.0f);
	ImGui::SliderFloat("Z", &Z, -360.0f, 360.0f);

	if (ImGui::Button("Reset")) {
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	endWindow();
}

}