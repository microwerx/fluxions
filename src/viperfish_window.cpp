#include "pch.hpp"
#include <sstream>
#include <viperfish_dear_imgui.hpp>
#include <viperfish_window.hpp>

namespace Vf
{

Window::Window(const std::string& name)
	: Widget(name) {
	std::ostringstream ostr;
	ostr << name << (void*)this;
	popupId = ostr.str();
}

Window::~Window() {

}

void Window::OnRenderDearImGui() {
	Widget::OnRenderDearImGui();
}

bool Window::beginWindow() {
	ImGui::SetNextWindowContentSize(ImVec2(getWidth(), getHeight()));
	if (!ImGui::Begin(getName().c_str(), &popupVisible)) {
		ImGui::End();
		return false;
	}
	ImGui::PushID(popupId.c_str());

	return true;
}

void Window::endWindow() {
	ImGui::PopID();
	ImGui::End();
}

}