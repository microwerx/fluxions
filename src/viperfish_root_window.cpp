#include "viperfish_pch.hpp"
#include <viperfish_root_window.hpp>

namespace Vf {
	RootWindow::RootWindow(const std::string& name)
		: Widget(name) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	}

	RootWindow::~RootWindow() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	}

	void RootWindow::OnRenderDearImGui() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		Widget::OnRenderDearImGui();
	}

} // namespace Vf
