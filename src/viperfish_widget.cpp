#include "viperfish_pch.hpp"
#include <viperfish_widget.hpp>
#include <viperfish_dear_imgui.hpp>
#include <viperfish_gamepad.hpp>

namespace Vf {
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
		decoratorWidget_.reset();
		decorateeWidget_.reset();
		parent_.reset();
		for (auto& w : children_) {
			w.reset();
		}
	}

	int Widget::CountVisible() const {
		if (!visible_) return 0;
		int count = 1;
		for (const auto& w : children_) {
			count += w->CountVisible();
		}
		return count;
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
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		for (int i = 0; i < 4; i++) {
			gamepads[i].Init(i);
		}

		OnInit(args);
	}

	void Widget::Kill() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		OnKill();

		// free the child, then the parent
		decoratorWidget_.reset();
		decorateeWidget_.reset();
		for (auto& w : children_) {
			w->Kill();
			w.reset();
		}
		children_.clear();
		parent_.reset();
	}

	void Widget::Hide() {
		OnWindowHidden();
	}

	void Widget::Show() {
		OnWindowVisible();
	}

	void Widget::ToggleVisible() {
		if (visible_) OnWindowHidden();
		else OnWindowVisible();
	}

	void Widget::MainLoop() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		leaveMainLoop_ = false;
		OnMainLoop();
	}

	void Widget::LeaveMainLoop() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);

		WidgetPtr p = shared_from_this();
		while (p->parent()) {
			p = p->parent();
		}

		p->leaveMainLoop_ = true;
		p->OnLeaveMainLoop();
	}

	void Widget::PollGamepads() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		for (int i = 0; i < 4; i++)
			gamepads[i].index = i;
		for (int i = 0; i < 4; i++)
			gamepads[i].Poll();
	}

	void Widget::HandleKey(const std::string& key, int keymod, bool pressed) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
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
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (pressed)
			mouse.OnButtonDown(button);
		else
			mouse.OnButtonUp(button);
	}

	void Widget::HandleMouseMove(int x, int y) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		mouse.OnMove(x, y);
		for (auto& button : mouse.buttons) {
			if (button.second) {
				OnMouseDrag(button.first, mouse.dragStates[button.first]);
			}
		}
	}

	void Widget::HandleMouseClick(const MouseClickState& mcs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		mouse.OnClick(mcs);
	}

	void Widget::HandleMouseDoubleClick(const MouseDoubleClickState& mdcs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		mouse.OnDoubleClick(mdcs);
	}

	void Widget::HandleMouseDrag(const MouseDragState& mds) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		mouse.OnDrag(mds);
	}

	bool Widget::processStyle(const std::string& style) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (style.empty()) return false;
		/*
		using namespace Df;
		JSONPtr json = JSON::MakeNull();
		if (!json->Deserialize(style))
			return false;

		if (!json->IsObject())
		{
			HFLOGERROR("style is not an JSON object");
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

	void Widget::OnInit() {
		OnInit({});
	}

	void Widget::OnInit(const std::vector<std::string>& args) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		leaveMainLoop_ = false;
		if (decoratorWidget_)
			decoratorWidget_->OnInit(args);
		for (auto& w : children_) {
			w->OnInit(args);
		}
	}

	void Widget::OnKill() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnKill();
		for (auto& w : children_) {
			w->OnKill();
		}
	}

	void Widget::OnUpdate(double timeStamp) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		t0 = t1;
		t1 = timeStamp;

		PollGamepads();
		if (decoratorWidget_)
			decoratorWidget_->OnUpdate(timeStamp);
		for (auto& w : children_) {
			w->OnUpdate(timeStamp);
		}
	}

	void Widget::OnKeyDown(const std::string& key, int keymod) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleKey(key, keymod, true);
		if (decoratorWidget_)
			decoratorWidget_->OnKeyDown(key, keymod);
		for (auto& w : children_) {
			w->OnKeyDown(key, keymod);
		}
	}

	void Widget::OnKeyUp(const std::string& key, int keymod) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleKey(key, keymod, false);
		if (decoratorWidget_)
			decoratorWidget_->OnKeyUp(key, keymod);
		for (auto& w : children_) {
			w->OnKeyUp(key, keymod);
		}
	}

	void Widget::OnMouseButtonDown(int button) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseButton(button, true);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseButtonDown(button);
		for (auto& w : children_) {
			w->OnMouseButtonDown(button);
		}
	}

	void Widget::OnMouseButtonUp(int button) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseButton(button, false);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseButtonUp(button);
		for (auto& w : children_) {
			w->OnMouseButtonUp(button);
		}
	}

	void Widget::OnMouseMove(int x, int y) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseMove(x, y);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseMove(x, y);
		for (auto& w : children_) {
			w->OnMouseMove(x, y);
		}
	}

	void Widget::OnMouseClick(int button, const MouseClickState& mcs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseClick(mcs);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseClick(button, mcs);
		for (auto& w : children_) {
			w->OnMouseClick(button, mcs);
		}
	}

	void Widget::OnMouseDoubleClick(int button, const MouseDoubleClickState& mdcs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseDoubleClick(mdcs);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseDoubleClick(button, mdcs);
		for (auto& w : children_) {
			w->OnMouseDoubleClick(button, mdcs);
		}
	}

	void Widget::OnMouseDrag(int button, const MouseDragState& mds) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		HandleMouseDrag(mds);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseDrag(button, mds);
		for (auto& w : children_) {
			w->OnMouseDrag(button, mds);
		}
	}

	void Widget::OnMouseEnter() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseEnter();
		for (auto& w : children_) {
			w->OnMouseEnter();
		}
	}

	void Widget::OnMouseLeave() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMouseLeave();
		for (auto& w : children_) {
			w->OnMouseLeave();
		}
	}

	void Widget::OnMultiEnter(int id) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMultiEnter(id);
		for (auto& w : children_) {
			w->OnMultiEnter(id);
		}
	}

	void Widget::OnMultiLeave(int id) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMultiLeave(id);
		for (auto& w : children_) {
			w->OnMultiLeave(id);
		}
	}

	void Widget::OnMultiButtonDown(int id, int button, const MouseState& ms) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMultiButtonDown(id, button, ms);
		for (auto& w : children_) {
			w->OnMultiButtonDown(id, button, ms);
		}
	}

	void Widget::OnMultiButtonUp(int id, int button, const MouseState& ms) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMultiButtonUp(id, button, ms);
		for (auto& w : children_) {
			w->OnMultiButtonUp(id, button, ms);
		}
	}

	void Widget::OnMultiMove(int x, int y, const MouseState& ms) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMultiMove(x, y, ms);
		for (auto& w : children_) {
			w->OnMultiMove(x, y, ms);
		}
	}

	void Widget::OnGainFocus() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnGainFocus();
		for (auto& w : children_) {
			w->OnGainFocus();
		}
	}

	void Widget::OnLostFocus() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnLostFocus();
		for (auto& w : children_) {
			w->OnLostFocus();
		}
	}

	void Widget::OnInitContext() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnInitContext();
		for (auto& w : children_) {
			w->OnInitContext();
		}
	}

	void Widget::OnPauseApp() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnPauseApp();
		for (auto& w : children_) {
			w->OnPauseApp();
		}
	}

	void Widget::OnResumeApp() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnResumeApp();
		for (auto& w : children_) {
			w->OnResumeApp();
		}
	}

	void Widget::OnWindowMove(int x, int y) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		windowRect_.x = (float)x;
		windowRect_.y = (float)y;
		if (decoratorWidget_)
			decoratorWidget_->OnWindowMove(x, y);
		for (auto& w : children_) {
			w->OnWindowMove(x, y);
		}
	}

	void Widget::OnWindowVisible() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		visible_ = true;
		if (decoratorWidget_)
			decoratorWidget_->OnWindowVisible();
		for (auto& w : children_) {
			w->OnWindowVisible();
		}
	}

	void Widget::OnWindowHidden() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		visible_ = false;
		if (decoratorWidget_)
			decoratorWidget_->OnWindowHidden();
		for (auto& w : children_) {
			w->OnWindowHidden();
		}
	}

	void Widget::OnGamepadAxis(int axis, float value, const GamepadState& gs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnGamepadAxis(axis, value, gs);
		for (auto& w : children_) {
			w->OnGamepadAxis(axis, value, gs);
		}
	}

	void Widget::OnGamepadButtonDown(int button, float value, const GamepadState& gs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnGamepadButtonDown(button, value, gs);
		for (auto& w : children_) {
			w->OnGamepadButtonDown(button, value, gs);
		}
	}

	void Widget::OnGamepadButtonUp(int button, float value, const GamepadState& gs) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnGamepadButtonUp(button, value, gs);
		for (auto& w : children_) {
			w->OnGamepadButtonUp(button, value, gs);
		}
	}

	void Widget::OnReshape(int width, int height) {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		windowRect_.w = (float)Fluxions::clamp(width, 1, 16384);
		windowRect_.h = (float)Fluxions::clamp(height, 1, 16384);
		if (decoratorWidget_)
			decoratorWidget_->OnReshape(width, height);
		for (auto& w : children_) {
			w->OnReshape(width, height);
		}
	}

	//void Widget::display() {
	//	HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
	//	if (decoratorWidget_) {
	//		decoratorWidget_->display();
	//	}
	//	for (auto& w : children_) {
	//		w->display();
	//	}
	//}

	void Widget::OnPreRender() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_) {
			decoratorWidget_->OnPreRender();
		}
		for (auto& w : children_) {
			w->OnPreRender();
		}
	}

	void Widget::OnRender3D() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnRender3D();
		for (auto& w : children_) {
			w->OnRender3D();
		}
	}

	void Widget::OnRender2D() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnRender2D();
		for (auto& w : children_) {
			w->OnRender2D();
		}
	}

	void Widget::OnRenderDearImGui() {
		if (!DearImGuiWidget::ContextInitialized) {
			HFLOGDEBUGFIRSTRUNCOUNTMSG(MAX_RUN_MESSAGES, "DearImGui not initialized");
			return;
		}
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnRenderDearImGui();
		for (auto& w : children_) {
			w->OnRenderDearImGui();
		}
	}

	void Widget::OnPostRender() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_) {
			decoratorWidget_->OnPostRender();
		}
		for (auto& w : children_) {
			w->OnPostRender();
		}
	}

	void Widget::OnMainLoop() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		if (decoratorWidget_)
			decoratorWidget_->OnMainLoop();
		for (auto& w : children_) {
			w->OnMainLoop();
		}
	}

	void Widget::OnLeaveMainLoop() {
		HFLOGDEBUGFIRSTRUNCOUNT(MAX_RUN_MESSAGES);
		leaveMainLoop_ = true;
		if (decorateeWidget_)
			decorateeWidget_->OnLeaveMainLoop();
	}


	//bool Widget::decorateWith(SharedPtr w)
	//{
	//	if (!w)
	//		return false;
	//	decoratorWidget_ = w;
	//	w->decorateeWidget_ = shared_from_this();
	//	return true;
	//}

	//void Widget::undecorate()
	//{
	//	if (!decorateeWidget_)
	//		return;
	//	decorateeWidget_->decoratorWidget_.reset();
	//}
} // namespace Vf
