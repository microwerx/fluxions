#ifndef VIPERFISH_LOADING_WINDOW_HPP
#define VIPERFISH_LOADING_WINDOW_HPP

#include <viperfish_window.hpp>

namespace Vf
{
	class LoadingWindow : public Window {
	public:
		LoadingWindow(const std::string& name);
		virtual ~LoadingWindow() override;

		void OnRenderDearImGui() override;

		std::string msg;
	};

	using LoadingWindowPtr = std::shared_ptr<LoadingWindow>;
}

#endif