#ifndef VIPERFISH_WINDOW_HPP
#define VIPERFISH_WINDOW_HPP

#include <viperfish_widget.hpp>

namespace Vf
{

class Window : public Widget
{
public:
	Window(const std::string& name);
	virtual ~Window();

	void OnRenderDearImGui() override;

protected:
	bool beginWindow();
	void endWindow();

	std::string popupId;
	bool popupVisible = true;
};

}
#endif