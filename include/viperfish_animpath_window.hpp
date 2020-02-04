#ifndef VIPERFISH_MATH_WINDOW_HPP
#define VIPERFISH_MATH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <viperfish_stats_window.hpp>
#include <viperfish_dear_imgui.hpp>

namespace Vf
{
	enum class PathAnimActions {
		None,
		GoToStart,
		GoToPrevKeyFrame,
		GoToPrevFrame,
		GoToPrevMicroUnit,
		GoToNextMicroUnit,
		GoToNextFrame,
		GoToNextKeyFrame,
		GoToEnd,
		FragileOperation,
		SetKey,
		SetAlpha,
		NewPathAnim,
		ClearAnimation,
		LoadAnimation,
		SaveAnimation,
		NumPathAnimActions
	};

	class AnimPathWindow : public Window {
	public:
		AnimPathWindow(const std::string& name);
		virtual ~AnimPathWindow() override;

		void OnUpdate(double timeStamp) override;
		void OnRenderDearImGui() override;

		std::string msg;
		Fx::Quaternionf q;
		Fx::Quaternionf q1;
		Fx::Quaternionf q2;

		Fx::Vector3f p1;
		Fx::Vector3f p2;
		float speed = 0.1f;
		float t = 0.0f;
		float alpha = 0.0f;
		bool bsquad = false;
		bool blerp = false;
		bool createNewPath = false;
		bool set_key = false;
		bool clear_animation = false;
		bool save_animation = false;
		bool load_animation = false;
		PathAnimActions action{ PathAnimActions::LoadAnimation };
		int key = 0;
		int max_keys = 0;
		std::vector<ImVec2> circles;
		std::vector<ImVec2> points;
		std::vector<std::pair<ImVec2, ImVec2>> lines;
		float offsetX{ 0.0f };
		float offsetY{ 0.0f };
		float scale{ 1.0f };
	private:
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float q1X = 0.0f;
		float q1Y = 0.0f;
		float q1Z = 0.0f;
		int pointIdx = 0;

		void _drawPath();
	};

	using AnimPathWindowPtr = std::shared_ptr<AnimPathWindow>;

} // namespace Vf

#endif
