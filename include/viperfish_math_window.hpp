#ifndef VIPERFISH_MATH_WINDOW_HPP
#define VIPERFISH_MATH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <viperfish_stats_window.hpp>
#include <viperfish_dear_imgui.hpp>

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
	Quaternionf q1;
	Quaternionf q2;
	Vector3f p1;
	Vector3f p2;
	float speed = 0.1f;
	float t = 0.0f;
	bool bsquad = false;
	bool blerp = false;
	int key = 0;
	int max_keys = 0;
	int set_key = 0;
	std::vector<ImVec2> circles;
	std::vector<ImVec2> points;
	std::vector<std::pair<ImVec2, ImVec2>> lines;
private:
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	int pointIdx = 0;
};

using MathWindowPtr = std::shared_ptr<MathWindow>;

} // namespace Vf


#endif