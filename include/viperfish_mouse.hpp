#ifndef VIPERFISH_MOUSE_HPP
#define VIPERFISH_MOUSE_HPP

#include <viperfish_base.hpp>
#include <viperfish_utilities.hpp>

#define VF_MOUSE_BUTTON_LEFT	0x0000;
#define VF_MOUSE_BUTTON_MIDDLE	0x0001;
#define VF_MOUSE_BUTTON_RIGHT	0x0002;

namespace Vf {
	using Fluxions::Vector2i;

	struct MouseDragState {
		int button;
		Vector2i startPosition;
		Vector2i endPosition;
		Vector2i previousPosition;
		Vector2i currentPosition;
		Vector2i currentDelta;
		Vector2i totalDelta;
		double startTime;
		double endTime;
		double timeDragged;

		void Start(int button, Vector2i newPosition, double currentTime);
		void Update(Vector2i newPosition, double currentTime);
	};

	struct MouseClickState {
		int button;
		Vector2i position;
		Vector2i startPosition;
		Vector2i dposition;
		double clickTime;

		void FillFromDrag(const MouseDragState& mds);
	};

	struct MouseDoubleClickState {
		int button;
		Vector2i position;
		Vector2i deltaPosition;
		double deltaTime;
		void FillFromClicks(int button, const MouseClickState& mcs1, const MouseClickState& mcs2);
		bool IsDoubleClick() const;
	};

	struct MouseState {
		std::map<int, bool> buttons;
		Vector2i position;
		Vector2i previousPosition;
		Vector2i dposition;
		std::map<int, MouseDragState> dragStates;
		std::map<int, MouseClickState> clickStates;
		std::map<int, MouseClickState> lastClickStates;
		std::map<int, MouseDoubleClickState> doubleClickStates;

		double lastPollTime;
		void UpdateTime();

		void OnMove(int x, int y);
		void OnButtonDown(int button);
		void OnButtonUp(int button);
		void OnDrag(const MouseDragState& mds);
		void OnClick(const MouseClickState& mcs);
		void OnDoubleClick(const MouseDoubleClickState& mdcs);
	};
} // namespace Vf

#endif
