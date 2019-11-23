#include "pch.hpp"
#include <viperfish_dear_imgui.hpp>
#include <viperfish_loading_window.hpp>

namespace Vf
{

LoadingWindow::LoadingWindow(const std::string& name)
	: Window(name) {}

LoadingWindow::~LoadingWindow() {}

void LoadingWindow::OnRenderDearImGui() {
	if (!beginWindow()) return;

	ImGui::Text("Loading...");
	static char buf[100]{ 0 };
	ImGui::InputText("sometext", buf, 99);
	ImGui::Separator();
	ImGui::Text("%s", msg.c_str());
	ImGui::Text("%s", buf);

	endWindow();
}

} // namespace Vf