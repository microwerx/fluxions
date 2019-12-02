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

	ImGui::SliderFloat("speed", &speed, 0.0f, 1.0f);
	ImGui::Value("t", t);
	ImGui::Separator();

	ImGui::Value("x", p2.x);
	ImGui::SameLine();
	ImGui::Value("y", p2.y);
	ImGui::SameLine();
	ImGui::Value("z", p2.z);

	ImGui::Value("a", q2.a);
	ImGui::SameLine();
	ImGui::Value("b", q2.b);
	ImGui::SameLine();
	ImGui::Value("c", q2.c);
	ImGui::SameLine();
	ImGui::Value("d", q2.d);

	ImGui::Text("Keyframe");

	ImGui::Value("a", q1.a);
	ImGui::SameLine();
	ImGui::Value("b", q1.b);
	ImGui::SameLine();
	ImGui::Value("c", q1.c);
	ImGui::SameLine();
	ImGui::Value("d", q1.d);

	ImGui::Value("x", p1.x);
	ImGui::SameLine();
	ImGui::Value("y", p1.y);
	ImGui::SameLine();
	ImGui::Value("z", p1.z);

	ImGui::Text("Modify active");

	ImGui::Value("a", q.a);
	ImGui::SameLine();
	ImGui::Value("b", q.b);
	ImGui::SameLine();
	ImGui::Value("c", q.c);
	ImGui::SameLine();
	ImGui::Value("d", q.d);

	if (max_keys > 0) {
		ImGui::SliderInt("Key", &key, 0, max_keys - 1);
	}

	ImGui::SliderFloat("qX", &X, -90.0f, 90.0f);
	ImGui::SliderFloat("qY", &Y, -360.0f, 360.0f);
	ImGui::SliderFloat("qZ", &Z, -180.0f, 180.0f);

	ImGui::SliderFloat("pX", &p1.x, -30.0f, 30.0f);
	ImGui::SliderFloat("pY", &p1.y, -30.0f, 30.0f);
	ImGui::SliderFloat("pZ", &p1.z, -30.0f, 30.0f);

	if (ImGui::Button("Set Key")) {
		set_key = 1;
	}
	else {
		set_key = 0;
	}

	if (ImGui::Button("Reset Q")) {
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	if (ImGui::Button("Reset P")) {
		p1.reset();
	}

	ImGui::Checkbox("LERP", &blerp);
	ImGui::Checkbox("SQUAD", &bsquad);

	ImDrawList* dl = ImGui::GetWindowDrawList();
	if (dl) {
		constexpr float size = 20.0f;
		constexpr float radius = 4.0f;
		ImVec2 ip = ImGui::GetCursorScreenPos();
		ip.y += 100.0f;

		Vector3f Z(q2.m13(), q2.m23(), q2.m33());
		Z.normalize();
		Z *= 2;
		ImVec2 curp{ ip.x + p2.x * size, ip.y + p2.z * size };
		ImVec2 curq{ ip.x + (p2.x + Z.x) * size, ip.y + (p2.z + Z.z) * size };

		//points[pointIdx].y = p2.z * size;
		//pointIdx = incr_wrap(pointIdx, (int)points.size() - 1);

		//if (pointIdx % 15 == 0) {
		//	Vector3f Z(q2.m13(), q2.m23(), q2.m33());
		//	Z.normalize();
		//	points[pointIdx].x = (4 * Z.x + p2.x) * size;
		//	points[pointIdx].y = (4 * Z.z + p2.z) * size;
		//	pointIdx = incr_wrap(pointIdx, (int)points.size() - 1);
		//	points[pointIdx].x = p2.x * size;
		//	points[pointIdx].y = p2.z * size;
		//	pointIdx = incr_wrap(pointIdx, (int)points.size() - 1);
		//}

		unsigned mcolor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
		unsigned ycolor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		unsigned ccolor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f));

		for (auto& c : circles) {
			dl->AddCircleFilled(ImVec2(ip.x + c.x, ip.y + c.y),
								radius, ccolor);
		}

		for (auto& p : points) {
			dl->PathLineTo(ImVec2(ip.x + p.x, ip.y + p.y));
		}
		dl->PathStroke(ccolor, false);

		for (auto& l : lines) {
			dl->AddLine(ImVec2(ip.x + l.first.x, ip.y + l.first.y),
						ImVec2(ip.x + l.second.x, ip.y + l.second.y), ycolor);
		}

		dl->AddCircleFilled(curp, radius * 1.5f, mcolor);
		dl->AddLine(curp, curq, mcolor);
	}

	endWindow();
}

}