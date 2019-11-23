#include "pch.hpp"
#include <viperfish_root_window.hpp>

namespace Vf
{

constexpr int MAX_RUN_MESSAGES = 2;

RootWindow::RootWindow(const std::string& name)
	: Window(name) {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
}

RootWindow::~RootWindow() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
}

void RootWindow::OnRenderDearImGui() {
	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
}

} // namespace Vf