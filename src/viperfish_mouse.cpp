// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include "pch.hpp"
#include <viperfish_mouse.hpp>

namespace Vf
{
	using namespace Fluxions;
	// @class MouseDragState

	void MouseDragState::Start(int whichButton, Vector2i newPosition, double currentTime) {
		button = whichButton;
		startPosition = newPosition;
		endPosition = newPosition;
		currentPosition = newPosition;
		previousPosition = newPosition;
		currentDelta = Vector2i(0, 0);
		totalDelta = Vector2i(0, 0);
		startTime = currentTime;
		endTime = currentTime;
		timeDragged = 0;
	}

	void MouseDragState::Update(Vector2i newPosition, double currentTime) {
		previousPosition = currentPosition;
		currentPosition = newPosition;
		endPosition = newPosition;
		currentDelta = currentPosition - previousPosition;
		totalDelta = endPosition - startPosition;
		endTime = currentTime;
		timeDragged = currentTime - startTime;
	}

	// @class MouseClickState

	void MouseClickState::FillFromDrag(const MouseDragState& mds) {
		button = mds.button;
		startPosition = mds.startPosition;
		position = mds.endPosition;
		dposition = mds.totalDelta;
		clickTime = mds.endTime;
	}

	// @class MouseDoubleClickState

	void MouseDoubleClickState::FillFromClicks(int whichButton, const MouseClickState& mcs1, const MouseClickState& mcs2) {
		button = whichButton;
		position = mcs2.position;
		deltaPosition = mcs2.position - mcs1.position;
		deltaTime = mcs2.clickTime - mcs1.clickTime;
	}

	bool MouseDoubleClickState::IsDoubleClick() const {
		int clickDistance = deltaPosition.lengthSquared();
		return (clickDistance == 0 && deltaTime <= 500) ? true : false;
	}

	// @class MouseState
	void MouseState::UpdateTime() {
		lastPollTime = SteadyClockNow();
	}

	void MouseState::OnMove(int x, int y) {
		UpdateTime();

		previousPosition = position;
		position = Vector2i(x, y);
		dposition = position - previousPosition;

		// Handle drag states because clicks and double clicks depend on this data
		for (auto button : buttons) {
			if (button.second) {
				dragStates[button.first].Update(position, lastPollTime);
				OnDrag(dragStates[button.first]);
			}
		}
	}

	void MouseState::OnButtonDown(int button) {
		UpdateTime();

		// Set the overall state
		buttons[button] = true;

		// Start a new drag state
		dragStates[button].Start(button, position, lastPollTime);
	}

	void MouseState::OnButtonUp(int button) {
		UpdateTime();

		// Set the overall state
		buttons[button] = false;

		// finalize the drag time
		dragStates[button].Update(position, lastPollTime);

		// fill in the mouse click information
		lastClickStates[button] = clickStates[button];
		clickStates[button].FillFromDrag(dragStates[button]);
		doubleClickStates[button].FillFromClicks(button, lastClickStates[button], clickStates[button]);

		// call click callbacks
		OnClick(clickStates[button]);
		if (doubleClickStates[button].IsDoubleClick())
			OnDoubleClick(doubleClickStates[button]);
	}

	void MouseState::OnDrag(const MouseDragState& mds) {
		// Tell listeners about event...
	}

	void MouseState::OnClick(const MouseClickState& mcs) {
		// Tell listeners about event...
	}

	void MouseState::OnDoubleClick(const MouseDoubleClickState& mdcs) {
		// Tell listeners about event...
	}
}
