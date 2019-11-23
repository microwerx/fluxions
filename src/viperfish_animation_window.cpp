#include "pch.hpp"
#include <viperfish_animation_window.hpp>

namespace Vf
{

AnimationWindow::AnimationWindow(const std::string& name)
	: Window(name) {

}

AnimationWindow::~AnimationWindow() {

}

void AnimationWindow::OnRenderDearImGui() {
	Window::OnRenderDearImGui();
}

}