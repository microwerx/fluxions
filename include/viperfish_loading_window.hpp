#ifndef VIPERFISH_LOADING_WINDOW_HPP
#define VIPERFISH_LOADING_WINDOW_HPP

#include <viperfish_dear_imgui.hpp>

namespace Vf
{
	class LoadingWindow : public Widget {
	public:
		LoadingWindow(const std::string& name);
		virtual ~LoadingWindow() override;

		void OnRenderDearImGui() override;

		std::string msg;
	private:
		std::string popupId;
	};

	using LoadingWindowPtr = std::shared_ptr<LoadingWindow>;
}

#endif