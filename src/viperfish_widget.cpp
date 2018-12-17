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
#include "stdafx.h"
#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>

namespace Viperfish
{
	Widget::Widget() {
		std::ostringstream ostr;
		ostr << "unknownwidget" << (void*)this;
		name_ = ostr.str();
		hflog.infofn(__FUNCTION__, "Constructing %s", name_.c_str());
	}

	void Widget::Init(int argc, char **argv)
	{
		std::vector<std::string> args;
		for (int i = 0; i < argc; i++)
		{
			args.push_back(argv[i]);
		}

		Init(args);
	}

	void Widget::Init(std::vector<std::string> args)
	{
		for (int i = 0; i < 4; i++)
		{
			gamepads[i].Init(i);
		}

		this->OnInit(args);
	}

	void Widget::Kill()
	{
		OnKill();

		// free the child, then the parent
		decorateeWidget_.reset();
		decoraterWidget_.reset();
		for (auto &w : children_)
		{
			w.reset();
		}
		parent_.reset();
	}

	void Widget::MainLoop()
	{
		OnMainLoop();
	}

	void Widget::LeaveMainLoop()
	{
		OnLeaveMainLoop();
	}

	void Widget::OnUpdate(double timeStamp)
	{
		t0 = t1;
		t1 = timeStamp;

		PollGamepads();
		if (decorateeWidget_)
			decorateeWidget_->OnUpdate(timeStamp);
		for (auto &w : children_)
		{
			w->OnUpdate(timeStamp);
		}
	}

	void Widget::PollGamepads()
	{
		for (int i = 0; i < 4; i++)
			gamepads[i].index = i;
		for (int i = 0; i < 4; i++)
			gamepads[i].Poll();
	}

	void Widget::HandleKey(const std::string &key, int keymod, bool pressed)
	{
		keyboard.SetKey(key, keymod, pressed);

		if (keymod == 0)
		{
			kbgamepad.SetButton(GamePadBitNum::UP, keyboard.CheckKeyPressed({ "w", "W", "z", "Z", "Up", "ArrowUp" }));
			kbgamepad.SetButton(GamePadBitNum::DOWN, keyboard.CheckKeyPressed({ "s", "S", "Down", "ArrowDown" }));
			kbgamepad.SetButton(GamePadBitNum::LEFT, keyboard.CheckKeyPressed({ "a", "A", "Q", "q", "Left", "ArrowLeft" }));
			kbgamepad.SetButton(GamePadBitNum::RIGHT, keyboard.CheckKeyPressed({ "d", "D", "Right", "ArrowRight" }));
			kbgamepad.SetButton(GamePadBitNum::A, keyboard.CheckKeyPressed({ " ", "Space", "Spacebar" }));
			kbgamepad.SetButton(GamePadBitNum::B, keyboard.CheckKeyPressed({ "Esc", "Escape" }));
			kbgamepad.SetButton(GamePadBitNum::START, keyboard.CheckKeyPressed({ "Enter", "Return" }));

			float lthumbX = 0.0f;
			float lthumbY = 0.0f;
			float rthumbX = 0.0f;
			float rthumbY = 0.0f;
			float ltrigger = 0.0f;
			float rtrigger = 0.0f;

			if (keyboard.CheckKeyPressed({ "Left", "ArrowLeft" }))
				rthumbX -= 1.0f;
			if (keyboard.CheckKeyPressed({ "Right", "ArrowRight" }))
				rthumbX += 1.0f;
			if (keyboard.CheckKeyPressed({ "Up", "ArrowUp" }))
				rthumbY += 1.0f;
			if (keyboard.CheckKeyPressed({ "Down", "ArrowDown" }))
				rthumbY -= 1.0f;
			if (keyboard.CheckKeyPressed({ "q", "Q" }))
				ltrigger += 1.0f;
			if (keyboard.CheckKeyPressed({ "e", "E" }))
				rtrigger += 1.0f;
			if (keyboard.CheckKeyPressed({ "a", "A" }))
				lthumbX -= 1.0f;
			if (keyboard.CheckKeyPressed({ "d", "D" }))
				lthumbX += 1.0f;
			if (keyboard.CheckKeyPressed({ "w", "W" }))
				lthumbY += 1.0f;
			if (keyboard.CheckKeyPressed({ "s", "S" }))
				lthumbY -= 1.0f;
			if (keyboard.CheckKeyPressed({ "z", "Z" }))
				kbgamepad.SetButton(GamePadBitNum::A, 1.0f);
			if (keyboard.CheckKeyPressed({ "c", "C" }))
				kbgamepad.SetButton(GamePadBitNum::B, 1.0f);
			if (keyboard.CheckKeyPressed({ "r", "R" }))
				kbgamepad.SetButton(GamePadBitNum::Y, 1.0f);
			if (keyboard.CheckKeyPressed({ "1" }))
				kbgamepad.SetButton(GamePadBitNum::X, 1.0f);

			kbgamepad.SetButton(GamePadBitNum::LTHUMB_X, lthumbX);
			kbgamepad.SetButton(GamePadBitNum::LTHUMB_Y, lthumbY);
			kbgamepad.SetButton(GamePadBitNum::RTHUMB_X, rthumbX);
			kbgamepad.SetButton(GamePadBitNum::RTHUMB_Y, rthumbY);
			kbgamepad.SetButton(GamePadBitNum::L2, ltrigger);
			kbgamepad.SetButton(GamePadBitNum::R2, rtrigger);
		}
	}

	void Widget::HandleMouseButton(int button, bool pressed)
	{
		if (pressed)
			mouse.OnButtonDown(button);
		else
			mouse.OnButtonUp(button);
	}

	void Widget::HandleMouseMove(int x, int y)
	{
		mouse.OnMove(x, y);
		for (auto &button : mouse.buttons)
		{
			if (button.second)
			{
				OnMouseDrag(button.first, mouse.dragStates[button.first]);
			}
		}
	}

	void Widget::HandleMouseClick(const MouseClickState &mcs)
	{
		mouse.OnClick(mcs);
	}

	void Widget::HandleMouseDoubleClick(const MouseDoubleClickState &mdcs)
	{
		mouse.OnDoubleClick(mdcs);
	}

	void Widget::HandleMouseDrag(const MouseDragState &mds)
	{
		mouse.OnDrag(mds);
	}

	bool Widget::processStyle(const std::string &style)
	{
		using namespace KASL;
		JSONPtr json = JSON::MakeNull();
		if (!json->Deserialize(style))
			return false;

		if (!json->IsObject())
		{
			hflog.error("%s(): style is not an JSON object", __FUNCTION__);
			return false;
		}

		if (!jsonStyle_->IsObject())
		{
			jsonStyle_ = JSON::MakeObject(
				{ {"marginLeft", 0},
				 {"marginRight", 0},
				 {"marginTop", 0},
				 {"marginBottom", 0},
				 {"paddingLeft", 0},
				 {"paddingRight", 0},
				 {"paddingTop", 0},
				 {"paddingBottom", 0},
				 {"borderLeft", 0},
				 {"borderRight", 0},
				 {"borderTop", 0},
				 {"borderBottom", 0},
				 {"width", 0},
				 {"height", 0} });
		}

		if (json->HasKeyOfType("marginLeft", JSON::Type::Number))
		{
			jsonStyle_->getMember("marginLeft")->set(json->getMember("marginLeft"));
		}

		style_ = jsonStyle_->Serialize();
		return true;
	}

	//bool Widget::decorate(SharedPtr w)
	//{
	//	if (!w)
	//		return false;
	//	decorateeWidget_ = w;
	//	w->decoraterWidget_ = shared_from_this();
	//	return true;
	//}

	//void Widget::undecorate()
	//{
	//	if (!decoraterWidget_)
	//		return;
	//	decoraterWidget_->decorateeWidget_.reset();
	//}
} // namespace Viperfish
