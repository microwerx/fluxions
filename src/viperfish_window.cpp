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
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnRenderDearImGui();
	}

	bool Window::beginWindow() {
		if (!isVisible()) return false;
		ImGui::SetNextWindowContentSize(ImVec2((float)getWidth(), (float)getHeight()));
		if (!ImGui::Begin(name().c_str(), getVisiblePtr())) {
			ImGui::End();
			return false;
		}
		//ImGui::PushID(popupId.c_str());

		return true;
	}

	void Window::endWindow() {
		//ImGui::PopID();
		ImGui::End();
	}

}