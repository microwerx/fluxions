#ifndef VIPERFISH_ROOT_WINDOW_HPP
#define VIPERFISH_ROOT_WINDOW_HPP

#include <viperfish_window.hpp>

namespace Vf
{
	class RootWindow : public Widget {
	public:
		RootWindow(const std::string& name);
		virtual ~RootWindow() override;

		void OnRenderDearImGui() override;
	};

	using RootWindowPtr = std::shared_ptr<RootWindow>;
}

#endif