#ifndef VIPERFISH_MATH_WINDOW_HPP
#define VIPERFISH_MATH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <viperfish_stats_window.hpp>
#include <viperfish_dear_imgui.hpp>

namespace Vf
{

	class AnimPathWindow : public Window {
	public:
		AnimPathWindow(const std::string& name);
		virtual ~AnimPathWindow() override;

		void OnUpdate(double timeStamp) override;
		void OnRenderDearImGui() override;

		std::string msg;
		Quaternionf q;
		Quaternionf q1;
		Quaternionf q2;

		Quaternionf kq0;
		Quaternionf kq1;
		Quaternionf kq2;
		Quaternionf kq3;
		Quaternionf ka;
		Quaternionf kb;

		Vector3f p1;
		Vector3f p2;
		float speed = 0.1f;
		float t = 0.0f;
		float alpha = 0.0f;
		bool bsquad = false;
		bool blerp = false;
		bool createNewPath = false;
		int key = 0;
		int max_keys = 0;
		bool set_key = false;
		std::vector<ImVec2> circles;
		std::vector<ImVec2> points;
		std::vector<std::pair<ImVec2, ImVec2>> lines;
	private:
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float q1X = 0.0f;
		float q1Y = 0.0f;
		float q1Z = 0.0f;
		int pointIdx = 0;

		void showv(const char* m, const Vector3f& v);
		void showq(const char* m, const Quaternionf& q);
	};

	using AnimPathWindowPtr = std::shared_ptr<AnimPathWindow>;

} // namespace Vf

#endif