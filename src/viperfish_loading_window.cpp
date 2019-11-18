#include "stdafx.h"
#include <sstream>
#include <viperfish_dear_imgui.hpp>
#include <viperfish_loading_window.hpp>

namespace Vf
{
	LoadingWindow::LoadingWindow(const std::string& name)
		: Widget(name) {
		std::ostringstream ostr;
		ostr << name << (void*)this;
		popupId = ostr.str();
	}

	LoadingWindow::~LoadingWindow() {

	}

	void LoadingWindow::OnRenderDearImGui() {
		if (!isVisible()) return;

		constexpr float width = 512.0f;
		constexpr float height = 384.0f;

		ImGui::SetNextWindowContentSize(ImVec2(width, height));
		ImGui::Begin(getName().c_str());
		ImGui::PushID(popupId.c_str());
		ImGui::Text("Loading...");
		ImGui::Separator();
		ImGui::Text("%s", msg.c_str());
		ImGui::PopID();
		ImGui::End();
	}
}