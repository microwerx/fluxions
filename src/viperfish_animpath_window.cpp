#include "viperfish_pch.hpp"
#include <viperfish_base.hpp>
#include <viperfish_animpath_window.hpp>
#include <fluxions_gte_noise.hpp>

namespace Vf {
	AnimPathWindow::AnimPathWindow(const std::string& name)
		: Window(name) {}

	AnimPathWindow::~AnimPathWindow() {}

	void AnimPathWindow::OnUpdate(double timeStamp) {
		Window::OnUpdate(timeStamp);

		Fx::Quaternionf qX = Fx::Quaternionf::makeFromAngleAxis(X, 1.0, 0.0, 0.0);
		Fx::Quaternionf qY = Fx::Quaternionf::makeFromAngleAxis(Y, 0.0, 1.0, 0.0);
		Fx::Quaternionf qZ = Fx::Quaternionf::makeFromAngleAxis(Z, 0.0, 0.0, 1.0);

		q = qZ * qX * qY;
	}

	void AnimPathWindow::_showv(const char* m, const Fx::Vector3f& v) {
		ImGui::Text(m);
		ImGui::SameLine();
		ImGui::Value("x", v.x);
		ImGui::SameLine();
		ImGui::Value("y", v.y);
		ImGui::SameLine();
		ImGui::Value("z", v.z);
	}

	void AnimPathWindow::_showq(const char* m, const Fx::Quaternionf& q) {
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

	void AnimPathWindow::_drawPath() {
		ImDrawList* dl = ImGui::GetWindowDrawList();
		if (dl) {
			const float size = scale;
			constexpr float radius = 4.0f;
			ImVec2 ip = ImGui::GetCursorScreenPos();
			ip.y += 100.0f;

			Fx::Vector3f Z(q2.m13(), q2.m23(), q2.m33());
			Z.normalize();
			Z *= 2;
			ImVec2 curp{ ip.x + (p2.x - offsetX) * size, ip.y + (p2.z - offsetY) * size };
			ImVec2 curq{ ip.x + (p2.x - offsetX + Z.x) * size, ip.y + (p2.z - offsetY + Z.z) * size };

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
	}

	void AnimPathWindow::OnRenderDearImGui() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (!beginWindow()) return;
		Window::OnRenderDearImGui();

		if (!set_key) {
			q1X = q1.pitchInDegrees();
			q1Y = q1.yawInDegrees();
			q1Z = q1.rollInDegrees();
		}

		ImGui::SliderFloat("speed", &speed, 0.0f, 1.0f);
		ImGui::SliderFloat("%", &t, 0.0f, (float)max_keys);
		ImGui::Value("t", t);
		ImGui::SameLine();
		if (ImGui::Button("|<")) action = PathAnimActions::GoToStart;
		ImGui::SameLine();
		if (ImGui::Button("<||")) action = PathAnimActions::GoToPrevKeyFrame;
		ImGui::SameLine();
		if (ImGui::Button("<<")) action = PathAnimActions::GoToPrevFrame;
		ImGui::SameLine();
		if (ImGui::Button(">>")) action = PathAnimActions::GoToNextFrame;
		ImGui::SameLine();
		if (ImGui::Button("||>")) action = PathAnimActions::GoToNextKeyFrame;
		ImGui::SameLine();
		if (ImGui::Button(">|"))  action = PathAnimActions::GoToEnd;

		ImGui::Separator();

		if (ImGui::Button("clear")) action = PathAnimActions::ClearAnimation;
		ImGui::SameLine();
		if (ImGui::Button("Save")) action = PathAnimActions::SaveAnimation;
		ImGui::SameLine();
		if (ImGui::Button("Load")) action = PathAnimActions::LoadAnimation;

		ImGui::Separator();

		_showv("p2", p2);
		_showq("q2", q2);

		ImGui::Text("Keyframe");

		_showq("q1", q1);
		_showv("p1", p1);

		ImGui::Text("Modify active");

		_showq("q", q);

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

		if (ImGui::Button("Set Key")) {
			q1 = Fx::Quaternionf::makeFromAngles(q1Y, q1X, q1Z);
			action = PathAnimActions::SetKey;
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Alpha")) action = PathAnimActions::SetAlpha;
		ImGui::SameLine();
		if (ImGui::Button("New Path")) action = PathAnimActions::NewPathAnim;
		ImGui::DragFloat("alpha", &alpha, 0.01f, -4.0f, 4.0f);

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

		ImGui::Checkbox("LERP", &blerp);
		ImGui::Checkbox("SQUAD", &bsquad);

		_drawPath();

		endWindow();
	}

}
