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

#include <sstream>
#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>

namespace Vf
{
	Widget::Widget() {
		std::ostringstream ostr;
		ostr << "unknownwidget" << (void*)this;
		name_ = ostr.str();
		HFLOGINFO("Constructing %s", name_.c_str());
	}

	Widget::Widget(const std::string& name) noexcept {
		common_constructor(name);
	}

	Widget::~Widget() {
		HFLOGINFO("Destroying Widget '%s'", name_.c_str());
		decorateeWidget_.reset();
		decoraterWidget_.reset();
		parent_.reset();
		for (auto& w : children_) {
			w.reset();
		}
	}

	void Widget::Init(int argc, char** argv) {
		std::vector<std::string> args;
		for (int i = 0; i < argc; i++) {
			args.push_back(argv[i]);
		}

		Init(args);
	}

	void Widget::common_constructor(const std::string& name) noexcept {
		name_ = name;
		HFLOGINFO("Creating Widget '%s'", name_.c_str());
	}

	void Widget::Init(std::vector<std::string> args) {
		for (int i = 0; i < 4; i++) {
			gamepads[i].Init(i);
		}

		OnInit(args);
	}

	void Widget::Kill() {
		OnKill();

		// free the child, then the parent
		decorateeWidget_.reset();
		decoraterWidget_.reset();
		for (auto& w : children_) {
			w.reset();
		}
		parent_.reset();
	}

	void Widget::MainLoop() {
		leaveMainLoop_ = false;
		OnMainLoop();
	}

	void Widget::LeaveMainLoop() {
		leaveMainLoop_ = true;
		OnLeaveMainLoop();
	}

	void Widget::PollGamepads() {
		for (int i = 0; i < 4; i++)
			gamepads[i].index = i;
		for (int i = 0; i < 4; i++)
			gamepads[i].Poll();
	}

	void Widget::HandleKey(const std::string& key, int keymod, bool pressed) {
		keyboard.SetKey(key, keymod, pressed);

		if (keymod == 0) {
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

	void Widget::HandleMouseButton(int button, bool pressed) {
		if (pressed)
			mouse.OnButtonDown(button);
		else
			mouse.OnButtonUp(button);
	}

	void Widget::HandleMouseMove(int x, int y) {
		mouse.OnMove(x, y);
		for (auto& button : mouse.buttons) {
			if (button.second) {
				OnMouseDrag(button.first, mouse.dragStates[button.first]);
			}
		}
	}

	void Widget::HandleMouseClick(const MouseClickState& mcs) {
		mouse.OnClick(mcs);
	}

	void Widget::HandleMouseDoubleClick(const MouseDoubleClickState& mdcs) {
		mouse.OnDoubleClick(mdcs);
	}

	void Widget::HandleMouseDrag(const MouseDragState& mds) {
		mouse.OnDrag(mds);
	}

	bool Widget::processStyle(const std::string& style) {
		/*
		using namespace Df;
		JSONPtr json = JSON::MakeNull();
		if (!json->Deserialize(style))
			return false;

		if (!json->IsObject())
		{
			Hf::Log.error("%s(): style is not an JSON object", __FUNCTION__);
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
		*/
		return true;
	}

	void Widget::OnInit(const std::vector<std::string>& args) {
		leaveMainLoop_ = false;
		if (decorateeWidget_)
			decorateeWidget_->OnInit(args);
		for (auto& w : children_) {
			w->OnInit(args);
		}
	}

	void Widget::OnKill() {
		if (decorateeWidget_)
			decorateeWidget_->OnKill();
		for (auto& w : children_) {
			w->OnKill();
		}
	}

	void Widget::OnUpdate(double timeStamp) {
		t0 = t1;
		t1 = timeStamp;

		PollGamepads();
		if (decorateeWidget_)
			decorateeWidget_->OnUpdate(timeStamp);
		for (auto& w : children_) {
			w->OnUpdate(timeStamp);
		}
	}

	void Widget::OnKeyDown(const std::string& key, int keymod) {
		HandleKey(key, keymod, true);
		if (decorateeWidget_)
			decorateeWidget_->OnKeyDown(key, keymod);
		for (auto& w : children_) {
			w->OnKeyDown(key, keymod);
		}
	}

	void Widget::OnKeyUp(const std::string& key, int keymod) {
		HandleKey(key, keymod, false);
		if (decorateeWidget_)
			decorateeWidget_->OnKeyUp(key, keymod);
		for (auto& w : children_) {
			w->OnKeyUp(key, keymod);
		}
	}

	void Widget::OnMouseButtonDown(int button) {
		HandleMouseButton(button, true);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseButtonDown(button);
		for (auto& w : children_) {
			w->OnMouseButtonDown(button);
		}
	}

	void Widget::OnMouseButtonUp(int button) {
		HandleMouseButton(button, false);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseButtonUp(button);
		for (auto& w : children_) {
			w->OnMouseButtonUp(button);
		}
	}

	void Widget::OnMouseMove(int x, int y) {
		HandleMouseMove(x, y);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseMove(x, y);
		for (auto& w : children_) {
			w->OnMouseMove(x, y);
		}
	}

	void Widget::OnMouseClick(int button, const MouseClickState& mcs) {
		HandleMouseClick(mcs);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseClick(button, mcs);
		for (auto& w : children_) {
			w->OnMouseClick(button, mcs);
		}
	}

	void Widget::OnMouseDoubleClick(int button, const MouseDoubleClickState& mdcs) {
		HandleMouseDoubleClick(mdcs);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseDoubleClick(button, mdcs);
		for (auto& w : children_) {
			w->OnMouseDoubleClick(button, mdcs);
		}
	}

	void Widget::OnMouseDrag(int button, const MouseDragState& mds) {
		HandleMouseDrag(mds);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseDrag(button, mds);
		for (auto& w : children_) {
			w->OnMouseDrag(button, mds);
		}
	}

	void Widget::OnMouseEnter() {
		if (decorateeWidget_)
			decorateeWidget_->OnMouseEnter();
		for (auto& w : children_) {
			w->OnMouseEnter();
		}
	}

	void Widget::OnMouseLeave() {
		if (decorateeWidget_)
			decorateeWidget_->OnMouseLeave();
		for (auto& w : children_) {
			w->OnMouseLeave();
		}
	}

	void Widget::OnMultiEnter(int id) {
		if (decorateeWidget_)
			decorateeWidget_->OnMultiEnter(id);
		for (auto& w : children_) {
			w->OnMultiEnter(id);
		}
	}

	void Widget::OnMultiLeave(int id) {
		if (decorateeWidget_)
			decorateeWidget_->OnMultiLeave(id);
		for (auto& w : children_) {
			w->OnMultiLeave(id);
		}
	}

	void Widget::OnMultiButtonDown(int id, int button, const MouseState& ms) {
		if (decorateeWidget_)
			decorateeWidget_->OnMultiButtonDown(id, button, ms);
		for (auto& w : children_) {
			w->OnMultiButtonDown(id, button, ms);
		}
	}

	void Widget::OnMultiButtonUp(int id, int button, const MouseState& ms) {
		if (decorateeWidget_)
			decorateeWidget_->OnMultiButtonUp(id, button, ms);
		for (auto& w : children_) {
			w->OnMultiButtonUp(id, button, ms);
		}
	}

	void Widget::OnMultiMove(int x, int y, const MouseState& ms) {
		if (decorateeWidget_)
			decorateeWidget_->OnMultiMove(x, y, ms);
		for (auto& w : children_) {
			w->OnMultiMove(x, y, ms);
		}
	}

	void Widget::OnGainFocus() {
		if (decorateeWidget_)
			decorateeWidget_->OnGainFocus();
		for (auto& w : children_) {
			w->OnGainFocus();
		}
	}

	void Widget::OnLostFocus() {
		if (decorateeWidget_)
			decorateeWidget_->OnLostFocus();
		for (auto& w : children_) {
			w->OnLostFocus();
		}
	}

	void Widget::OnInitContext() {
		if (decorateeWidget_)
			decorateeWidget_->OnInitContext();
		for (auto& w : children_) {
			w->OnInitContext();
		}
	}

	void Widget::OnPauseApp() {
		if (decorateeWidget_)
			decorateeWidget_->OnPauseApp();
		for (auto& w : children_) {
			w->OnPauseApp();
		}
	}

	void Widget::OnResumeApp() {
		if (decorateeWidget_)
			decorateeWidget_->OnResumeApp();
		for (auto& w : children_) {
			w->OnResumeApp();
		}
	}

	void Widget::OnWindowMove(int x, int y) {
		windowRect_.x = x;
		windowRect_.y = y;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowMove(x, y);
		for (auto& w : children_) {
			w->OnWindowMove(x, y);
		}
	}

	void Widget::OnWindowVisible() {
		visible_ = true;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowVisible();
		for (auto& w : children_) {
			w->OnWindowVisible();
		}
	}

	void Widget::OnWindowHidden() {
		visible_ = false;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowHidden();
		for (auto& w : children_) {
			w->OnWindowHidden();
		}
	}

	void Widget::OnGamepadAxis(int axis, float value, const GamepadState& gs) {
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadAxis(axis, value, gs);
		for (auto& w : children_) {
			w->OnGamepadAxis(axis, value, gs);
		}
	}

	void Widget::OnGamepadButtonDown(int button, float value, const GamepadState& gs) {
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadButtonDown(button, value, gs);
		for (auto& w : children_) {
			w->OnGamepadButtonDown(button, value, gs);
		}
	}

	void Widget::OnGamepadButtonUp(int button, float value, const GamepadState& gs) {
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadButtonUp(button, value, gs);
		for (auto& w : children_) {
			w->OnGamepadButtonUp(button, value, gs);
		}
	}

	void Widget::OnReshape(int width, int height) {
		windowRect_.w = width;
		windowRect_.h = height;
		if (decorateeWidget_)
			decorateeWidget_->OnReshape(width, height);
		for (auto& w : children_) {
			w->OnReshape(width, height);
		}
	}

	void Widget::OnPreRender() {
		HFLOGDEBUGFIRSTRUN();
		if (decorateeWidget_) {
			decorateeWidget_->OnPreRender();
		}
		for (auto& w : children_) {
			w->OnPreRender();
		}
	}

	void Widget::OnRender3D() {
		HFLOGDEBUGFIRSTRUN();
		if (decorateeWidget_)
			decorateeWidget_->OnRender3D();
		for (auto& w : children_) {
			w->OnRender3D();
		}
	}

	void Widget::OnRender2D() {
		HFLOGDEBUGFIRSTRUN();
		if (decorateeWidget_)
			decorateeWidget_->OnRender2D();
		for (auto& w : children_) {
			w->OnRender2D();
		}
	}

	void Widget::OnRenderDearImGui() {
		HFLOGDEBUGFIRSTRUN();
		if (decorateeWidget_)
			decorateeWidget_->OnRenderDearImGui();
		for (auto& w : children_) {
			w->OnRenderDearImGui();
		}
	}

	void Widget::OnPostRender() {
		HFLOGDEBUGFIRSTRUN();
		if (decorateeWidget_) {
			decorateeWidget_->OnPostRender();
		}
		for (auto& w : children_) {
			w->OnPostRender();
		}
	}

	void Widget::OnMainLoop() {
		if (decorateeWidget_)
			decorateeWidget_->OnMainLoop();
		for (auto& w : children_) {
			w->OnMainLoop();
		}
	}

	void Widget::OnLeaveMainLoop() {
		leaveMainLoop_ = true;
		if (decoraterWidget_)
			decoraterWidget_->OnLeaveMainLoop();
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
} // namespace Vf
