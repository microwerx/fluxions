#ifndef VIPERFISH_MATH_WINDOW_HPP
#define VIPERFISH_MATH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <viperfish_stats_window.hpp>

namespace Vf
{

class MathWindow : public Window {
public:
	MathWindow(const std::string& name);
	virtual ~MathWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;

	std::string msg;
	Quaternionf q;
	Quaternionf q2;
	Vector3f p2;
private:
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
};

using MathWindowPtr = std::shared_ptr<MathWindow>;

} // namespace Vf


#endif