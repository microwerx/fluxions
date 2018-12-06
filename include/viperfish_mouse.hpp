// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#ifndef VIPERFISH_MOUSE_HPP
#define VIPERFISH_MOUSE_HPP

#include <viperfish_utilities.hpp>
#include <fluxions_gte.hpp>

namespace Viperfish
{
using namespace std;
using namespace Fluxions;

struct MouseDragState
{
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

struct MouseClickState
{
	int button;
	Vector2i position;
	Vector2i startPosition;
	Vector2i dposition;
	double clickTime;

	void FillFromDrag(int button, const MouseDragState &mds);
};

struct MouseDoubleClickState
{
	int button;
	Vector2i position;
	Vector2i deltaPosition;
	double deltaTime;
	void FillFromClicks(int button, const MouseClickState &mcs1, const MouseClickState &mcs2);
	bool IsDoubleClick() const;
};

struct MouseState
{
	map<int, bool> buttons;
	Vector2i position;
	Vector2i previousPosition;
	Vector2i dposition;
	map<int, MouseDragState> dragStates;
	map<int, MouseClickState> clickStates;
	map<int, MouseClickState> lastClickStates;
	map<int, MouseDoubleClickState> doubleClickStates;

	double lastPollTime;
	void UpdateTime();

	void OnMove(int x, int y);
	void OnButtonDown(int button);
	void OnButtonUp(int button);
	void OnDrag(const MouseDragState &mds);
	void OnClick(const MouseClickState &mcs);
	void OnDoubleClick(const MouseDoubleClickState &mdcs);
};
} // namespace Viperfish

#endif
