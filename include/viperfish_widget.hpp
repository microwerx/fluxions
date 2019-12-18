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
#ifndef VIPERFISH_WIDGET_HPP
#define VIPERFISH_WIDGET_HPP

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <fluxions_gte_rect.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_keyboard.hpp>
#include <viperfish_gamepad.hpp>
//#include <kasl_json.hpp>
#include <hatchetfish.hpp>

namespace Vf
{
	// Use the decorator pattern
	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		using SharedPtr = std::shared_ptr<Widget>;
		using UniquePtr = std::unique_ptr<Widget>;

		inline SharedPtr GetSharedPtr() noexcept { return shared_from_this(); }

		// this is required to enable shared_from_this() to work properly
		Widget();
		Widget(const Widget &) = default;	// default copy constructor
		Widget(Widget &&) = default;		// default move constructor

		// Constructors
		explicit Widget(const std::string &name) noexcept;
		virtual ~Widget();

		// Imperative functions

		void Init(int argc, char **argv);
		void Init(std::vector<std::string> args);
		void Kill();
		void MainLoop();
		void LeaveMainLoop();

		// Child container stuff (adapted from Stroustrup's book)
		using value_type = SharedPtr;
		using iterator = std::vector<SharedPtr>::iterator;
		using const_iterator = std::vector<SharedPtr>::const_iterator;

		// Child container stuff dependent on whether leaf_ is true or not.
		inline bool isLeaf() const { return leaf_; }
		inline void clear() noexcept { children_.clear(); }
		inline size_t size() const noexcept { if (leaf_) return 0; return children_.size(); }
		inline bool empty() const noexcept { if (leaf_) return true; return children_.empty(); }
		inline void push_back(const SharedPtr &child_) noexcept { if (leaf_) return; children_.push_back(child_); child_->parent_ = shared_from_this(); }
		inline void emplace_back(SharedPtr &&child_) noexcept { if (leaf_) return; children_.emplace_back(child_); child_->parent_ = shared_from_this(); }
		inline void pop_back() noexcept { if (!empty()) children_.pop_back(); }
		inline SharedPtr operator[](size_t _Pos) noexcept { if (leaf_) return SharedPtr(nullptr); return children_.at(_Pos); }
		inline const SharedPtr operator[](size_t _Pos) const noexcept { if (leaf_) return SharedPtr(nullptr); return children_.at(_Pos); }
		inline iterator begin() noexcept { if (leaf_) return children_.end(); return children_.begin(); }
		inline const_iterator begin() const noexcept { if (leaf_) return children_.end(); return children_.begin(); }
		inline iterator end() noexcept { return children_.end(); }
		inline const_iterator end() const noexcept { return children_.end(); }
		inline const_iterator cbegin() const noexcept { if (leaf_) return children_.cend(); return children_.cbegin(); }
		inline const_iterator cend() const noexcept { return children_.cend(); }
		inline const value_type &at(const size_t _Pos) const { throwIfLeaf(); return children_.at(_Pos); }
		inline value_type &at(const size_t _Pos) { throwIfLeaf(); return children_.at(_Pos); }
		inline value_type *data() noexcept { if (leaf_) return nullptr; return children_.data(); }
		inline const value_type *data() const noexcept { if (leaf_) return nullptr; return children_.data(); }

		// Properties
		inline const SharedPtr &decoratee() const noexcept { return decoratorWidget_; }
		inline SharedPtr &decoratee() noexcept { return decoratorWidget_; }
		inline const SharedPtr &decorator() const noexcept { return decorateeWidget_; }
		inline SharedPtr &decorator() noexcept { return decorateeWidget_; }
		inline const SharedPtr &parent() const noexcept { return parent_; }
		inline SharedPtr &parent() noexcept { return parent_; }

		inline bool decorateWith(std::shared_ptr<Widget> decorator) noexcept {
			if (!decorator) return false;
			decoratorWidget_ = decorator;
			decorator->decorateeWidget_ = shared_from_this();
			return true;
		}

		inline void undecorate() noexcept {
			if (!decoratorWidget_) return;
			decoratorWidget_->decorateeWidget_.reset();
		}

		inline bool is_decorated() const noexcept { return (bool)decoratorWidget_; }

		inline const std::string &getName() const noexcept { return name_; }

		inline float getX() const { return windowRect_.x; }
		inline float getY() const { return windowRect_.y; }
		inline float getHeight() const { return windowRect_.w; }
		inline float getWidth() const { return windowRect_.h; }
		inline const Rectf &windowRect() const { return windowRect_; }
		inline Rectf &windowRect() { return windowRect_; }

		inline bool isVisible() const { return visible_; }
		inline bool isOrphan() const { return !parent_; }
		inline bool leaveMainLoop() const { return leaveMainLoop_; }

		int CountVisible() const;

		inline operator bool() { return isVisible(); }

		inline double GetElapsedTime() const { return t1 - baseTime; }
		inline double GetFrameTime() const { return t1 - t0; }
		inline double GetStopwatchTime() const { return t1 - stopwatchTimeT0; }
		inline void ResetStopwatch() { stopwatchTimeT0 = t1; }
		inline void ResetClock() { baseTime = t1; t0 = t1; }

		inline const std::string &caption() const noexcept { return caption_; }
		inline std::string &caption() noexcept { return caption_; }
		inline const std::string &style() const noexcept { return style_; }
		// inline const Df::JSONPtr jsonStyle() const noexcept { return jsonStyle_; }
		// inline Df::JSONPtr jsonStyle() noexcept { return jsonStyle_; }
		// inline Df::JSONPtr setStyle(const std::string &style) noexcept { processStyle(style); return jsonStyle_; }

		virtual void OnInit(const std::vector<std::string> &args);
		virtual void OnKill();
		virtual void OnUpdate(double timeStamp);
		virtual void OnKeyDown(const std::string &key, int keymod);
		virtual void OnKeyUp(const std::string &key, int keymod);
		virtual void OnMouseButtonDown(int button);
		virtual void OnMouseButtonUp(int button);
		virtual void OnMouseMove(int x, int y);
		virtual void OnMouseClick(int button, const MouseClickState &mcs);
		virtual void OnMouseDoubleClick(int button, const MouseDoubleClickState &mdcs);
		virtual void OnMouseDrag(int button, const MouseDragState &mds);
		virtual void OnMouseEnter();
		virtual void OnMouseLeave();
		virtual void OnMultiEnter(int id);
		virtual void OnMultiLeave(int id);
		virtual void OnMultiButtonDown(int id, int button, const MouseState &ms);
		virtual void OnMultiButtonUp(int id, int button, const MouseState &ms);
		virtual void OnMultiMove(int x, int y, const MouseState &ms);
		virtual void OnGainFocus();
		virtual void OnLostFocus();
		virtual void OnInitContext();
		virtual void OnPauseApp();
		virtual void OnResumeApp();
		virtual void OnWindowMove(int x, int y);
		virtual void OnWindowVisible();
		virtual void OnWindowHidden();
		virtual void OnGamepadAxis(int axis, float value, const GamepadState &gs);
		virtual void OnGamepadButtonDown(int button, float value, const GamepadState &gs);
		virtual void OnGamepadButtonUp(int button, float value, const GamepadState &gs);
		virtual void OnReshape(int width, int height);

		//virtual void display();
		virtual void OnPreRender();
		virtual void OnRender3D();
		virtual void OnRender2D();
		virtual void OnRenderDearImGui();
		virtual void OnPostRender();

		virtual void OnMainLoop();
		virtual void OnLeaveMainLoop();
	protected:
		bool* getVisiblePtr() { return &visible_; }
	private:
		SharedPtr decoratorWidget_;
		SharedPtr decorateeWidget_;
		SharedPtr parent_;
		std::vector<SharedPtr> children_;

		std::string name_;
		void common_constructor(const std::string &name) noexcept;

		bool leaveMainLoop_ = false;

		Rectf windowRect_;
		bool visible_ = true;

		// A class should set leaf_ to false if it's okay to be a container, true if not
		bool leaf_ = false;

		inline bool throwIfLeaf() const { if (!leaf_) { return false; } throw std::out_of_range("this class instance is a leaf, not a container"); return true; }

		double stopwatchTimeT0 = 0.0;
		double baseTime = 0.0;
		double t0 = 0.0;
		double t1 = 0.0;

		void PollGamepads();
		void HandleKey(const std::string &key, int keymod, bool pressed);
		void HandleMouseButton(int button, bool pressed);
		void HandleMouseMove(int x, int y);
		void HandleMouseClick(const MouseClickState &mcs);
		void HandleMouseDoubleClick(const MouseDoubleClickState &mdcs);
		void HandleMouseDrag(const MouseDragState &mds);

		virtual bool processStyle(const std::string &style);

	protected:
		GamepadState gamepads[4];
		GamepadState kbgamepad;
		KeyboardState keyboard;
		MouseState mouse;
		// Df::JSONPtr jsonStyle_;
		std::string style_;
		std::string caption_;
		void makeLeaf(bool state) { leaf_ = state; }

		const double getT0() const { return t0; }
		const double getT1() const { return t1; }
	};

	using WidgetPtr = std::shared_ptr<Widget>;
} // namespace Vf

#endif
