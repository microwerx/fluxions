#ifndef VIPERFISH_ANIMATION_WINDOW_HPP
#define VIPERFISH_ANIMATION_WINDOW_CPP

#include <viperfish_window.hpp>

namespace Vf
{

class AnimationWindow : public Window
{
public:
	AnimationWindow(const std::string& name);
	virtual ~AnimationWindow();

	void OnRenderDearImGui() override;
};

}

#endif