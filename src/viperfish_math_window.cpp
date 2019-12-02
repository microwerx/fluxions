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

void MathWindow::showv(const char* m, const Vector3f& v) {
	ImGui::Text(m);
	ImGui::SameLine();
	ImGui::Value("x", v.x);
	ImGui::SameLine();
	ImGui::Value("y", v.y);
	ImGui::SameLine();
	ImGui::Value("z", v.z);
}

void MathWindow::showq(const char* m, const Quaternionf& q) {
	ImGui::Text(m);
	ImGui::SameLine();
	ImGui::Value("a", q.a);
	ImGui::SameLine();
	ImGui::Value("b", q.b);
	ImGui::SameLine();
	ImGui::Value("c", q.c);
	ImGui::SameLine();
	ImGui::Value("d", q.d);
}

void MathWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	if (!beginWindow()) return;
	Window::OnRenderDearImGui();

	if (!set_key) {
		q1X = q1.pitchInDegrees();
		q1Y = q1.yawInDegrees();
		q1Z = q1.rollInDegrees();
	}

	Quaternionf qX = Quaternionf::makeFromAngleAxis(X, 1.0, 0.0, 0.0);
	Quaternionf qY = Quaternionf::makeFromAngleAxis(Y, 0.0, 1.0, 0.0);
	Quaternionf qZ = Quaternionf::makeFromAngleAxis(Z, 0.0, 0.0, 1.0);

	q = qZ * qX * qY;

	ImGui::SliderFloat("speed", &speed, 0.0f, 1.0f);
	ImGui::Value("t", t);
	ImGui::Separator();

	showv("p2", p2);
	showq("q2", q2);

	//ImGui::Text("K");
	//showq(kq0);
	//showq(kq1);
	//showq(kq2);
	//showq(kq3);
	//showq(ka);
	//showq(kb);

	ImGui::Text("Keyframe");

	showq("q1", q1);
	showv("p1", p1);

	ImGui::Text("Modify active");

	showq("q", q);

	ImGui::SliderFloat("X", &X, -90.0f, 90.0f);
	ImGui::SliderFloat("Y", &Y, -360.0f, 360.0f);
	ImGui::SliderFloat("Z", &Z, -180.0f, 180.0f);

	ImGui::Separator();

	if (max_keys > 0) {
		ImGui::SliderInt("Key", &key, 0, max_keys - 1);
	}

	ImGui::SliderFloat("qX", &q1X, -90.0f, 90.0f);
	ImGui::SliderFloat("qY", &q1Y, -360.0f, 360.0f);
	ImGui::SliderFloat("qZ", &q1Z, -180.0f, 180.0f);

	ImGui::SliderFloat("pX", &p1.x, -30.0f, 30.0f);
	ImGui::SliderFloat("pY", &p1.y, -30.0f, 30.0f);
	ImGui::SliderFloat("pZ", &p1.z, -30.0f, 30.0f);

	ImGui::Checkbox("Set Key", &set_key);

	if (ImGui::Button("Reset Q")) {
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	if (ImGui::Button("Reset Q1")) {
		q1X = 0.0f;
		q1Y = 0.0f;
		q1Z = 0.0f;
	}

	if (ImGui::Button("Reset P1")) {
		p1.reset();
	}

	if (set_key) {
		q1 = Quaternionf::makeFromAngles(q1Y, q1X, q1Z);
	}

	if (ImGui::Button("create")) {
		createNewPath = true;
	}
	ImGui::DragFloat("alpha", &alpha, 0.01f, -4.0f, 4.0f);
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