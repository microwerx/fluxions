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
#ifndef VIPERFISH_WIDGET_HPP
#define VIPERFISH_WIDGET_HPP

#include <memory>
#include <vector>
#include <string>
#include <fluxions_gte_rect.hpp>
#include <viperfish.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_keyboard.hpp>
#include <viperfish_gamepad.hpp>
#include <kasl_json.hpp>
#include <hatchetfish.hpp>

namespace Viperfish
{
// Use the decorator pattern
class Widget : public std::enable_shared_from_this<Widget>
{
  public:
	// this is required to enable shared_from_this() to work properly
	Widget();
	Widget(const Widget &) = default;

	std::string name_;

	void common_constructor(const std::string &name)
	{
		name_ = name;
		hflog.infofn(__FUNCTION__, "Creating Widget '%s'", name_.c_str());
	}

  public:
	using SharedPtr = std::shared_ptr<Widget>;
	using UniquePtr = std::unique_ptr<Widget>;

	explicit Widget(const std::string &name) noexcept { common_constructor(name); }
	explicit Widget(const std::string &name, SharedPtr &decorateeWidget) noexcept
	{
		common_constructor(name);
		decorate(decorateeWidget);
	}

	template <class... _Types>
	static SharedPtr MakeShared(const std::string &name, _Types &&... _Args)
	{
		Widget *w = new Widget(name, std::forward<_Types>(_Args)...);
		return SharedPtr(w);
	}
	template <class... _Types>
	static UniquePtr MakeUnique(const std::string &name, _Types &&... _Args)
	{
		Widget *w = new Widget(name, std::forward<_Types>(_Args)...);
		return UniquePtr(w);
	}

	//Widget(SharedPtr && decorateeWidget) { decorateeWidget_ = decorateeWidget; }

	//Widget(int X, int y, int width, int height, const std::string & caption, const std::string & jsonStyle)
	//{
	//	windowRect_.SetFromPoints(Vector2i(X, y), Vector2i(X + width, y + height));
	//	caption_ = caption;
	//	this->setStyle(jsonStyle);
	//}

	explicit Widget(std::string &name, std::initializer_list<SharedPtr> childWidgets) noexcept
	{
		common_constructor(name);
		for (auto child = childWidgets.begin(); child != childWidgets.end(); child++)
		{
			push_back(*child);
		}
	}

	virtual ~Widget()
	{
		hflog.infofn(__FUNCTION__, "Destroying Widget '%s'", name_.c_str());
		decorateeWidget_.reset();
		decoraterWidget_.reset();
		parent_.reset();
		for (auto &w : children_)
		{
			w.reset();
		}
	}

	inline SharedPtr GetWidgetPtr() noexcept { return shared_from_this(); }

	virtual void OnInit(const std::vector<std::string> &args)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnInit(args);
		for (auto &w : children_)
		{
			w->OnInit(args);
		}
	}

	virtual void OnKill()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnKill();
		for (auto &w : children_)
		{
			w->OnKill();
		}
	}
	virtual void OnUpdate(double timeStamp);

	virtual void OnKeyDown(const std::string &key, int keymod)
	{
		HandleKey(key, keymod, true);
		if (decorateeWidget_)
			decorateeWidget_->OnKeyDown(key, keymod);
		for (auto &w : children_)
		{
			w->OnKeyDown(key, keymod);
		}
	}
	virtual void OnKeyUp(const std::string &key, int keymod)
	{
		HandleKey(key, keymod, false);
		if (decorateeWidget_)
			decorateeWidget_->OnKeyUp(key, keymod);
		for (auto &w : children_)
		{
			w->OnKeyUp(key, keymod);
		}
	}

	virtual void OnMouseButtonDown(int button)
	{
		HandleMouseButton(button, true);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseButtonDown(button);
		for (auto &w : children_)
		{
			w->OnMouseButtonDown(button);
		}
	}
	virtual void OnMouseButtonUp(int button)
	{
		HandleMouseButton(button, false);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseButtonUp(button);
		for (auto &w : children_)
		{
			w->OnMouseButtonUp(button);
		}
	}
	virtual void OnMouseMove(int x, int y)
	{
		HandleMouseMove(x, y);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseMove(x, y);
		for (auto &w : children_)
		{
			w->OnMouseMove(x, y);
		}
	}
	virtual void OnMouseClick(int button, const MouseClickState &mcs)
	{
		HandleMouseClick(mcs);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseClick(button, mcs);
		for (auto &w : children_)
		{
			w->OnMouseClick(button, mcs);
		}
	}
	virtual void OnMouseDoubleClick(int button, const MouseDoubleClickState &mdcs)
	{
		HandleMouseDoubleClick(mdcs);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseDoubleClick(button, mdcs);
		for (auto &w : children_)
		{
			w->OnMouseDoubleClick(button, mdcs);
		}
	}
	virtual void OnMouseDrag(int button, const MouseDragState &mds)
	{
		HandleMouseDrag(mds);
		if (decorateeWidget_)
			decorateeWidget_->OnMouseDrag(button, mds);
		for (auto &w : children_)
		{
			w->OnMouseDrag(button, mds);
		}
	}

	virtual void OnMouseEnter()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMouseEnter();
		for (auto &w : children_)
		{
			w->OnMouseEnter();
		}
	}
	virtual void OnMouseLeave()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMouseLeave();
		for (auto &w : children_)
		{
			w->OnMouseLeave();
		}
	}

	virtual void OnMultiEnter(int id)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMultiEnter(id);
		for (auto &w : children_)
		{
			w->OnMultiEnter(id);
		}
	}
	virtual void OnMultiLeave(int id)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMultiLeave(id);
		for (auto &w : children_)
		{
			w->OnMultiLeave(id);
		}
	}
	virtual void OnMultiButtonDown(int id, int button, const MouseState &ms)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMultiButtonDown(id, button, ms);
		for (auto &w : children_)
		{
			w->OnMultiButtonDown(id, button, ms);
		}
	}
	virtual void OnMultiButtonUp(int id, int button, const MouseState &ms)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMultiButtonUp(id, button, ms);
		for (auto &w : children_)
		{
			w->OnMultiButtonUp(id, button, ms);
		}
	}
	virtual void OnMultiMove(int x, int y, const MouseState &ms)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMultiMove(x, y, ms);
		for (auto &w : children_)
		{
			w->OnMultiMove(x, y, ms);
		}
	}

	virtual void OnGainFocus()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnGainFocus();
		for (auto &w : children_)
		{
			w->OnGainFocus();
		}
	}
	virtual void OnLostFocus()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnLostFocus();
		for (auto &w : children_)
		{
			w->OnLostFocus();
		}
	}

	virtual void OnInitContext()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnInitContext();
		for (auto &w : children_)
		{
			w->OnInitContext();
		}
	}
	virtual void OnPauseApp()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnPauseApp();
		for (auto &w : children_)
		{
			w->OnPauseApp();
		}
	}
	virtual void OnResumeApp()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnResumeApp();
		for (auto &w : children_)
		{
			w->OnResumeApp();
		}
	}

	virtual void OnWindowMove(int x, int y)
	{
		windowRect_.x = x;
		windowRect_.y = y;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowMove(x, y);
		for (auto &w : children_)
		{
			w->OnWindowMove(x, y);
		}
	}
	virtual void OnWindowVisible()
	{
		visible_ = true;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowVisible();
		for (auto &w : children_)
		{
			w->OnWindowVisible();
		}
	}
	virtual void OnWindowHidden()
	{
		visible_ = false;
		if (decorateeWidget_)
			decorateeWidget_->OnWindowHidden();
		for (auto &w : children_)
		{
			w->OnWindowHidden();
		}
	}

	virtual void OnGamepadAxis(int axis, float value, const GamepadState &gs)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadAxis(axis, value, gs);
		for (auto &w : children_)
		{
			w->OnGamepadAxis(axis, value, gs);
		}
	}
	virtual void OnGamepadButtonDown(int button, float value, const GamepadState &gs)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadButtonDown(button, value, gs);
		for (auto &w : children_)
		{
			w->OnGamepadButtonDown(button, value, gs);
		}
	}
	virtual void OnGamepadButtonUp(int button, float value, const GamepadState &gs)
	{
		if (decorateeWidget_)
			decorateeWidget_->OnGamepadButtonUp(button, value, gs);
		for (auto &w : children_)
		{
			w->OnGamepadButtonUp(button, value, gs);
		}
	}

	virtual void OnReshape(int width, int height)
	{
		windowRect_.w = width;
		windowRect_.h = height;
		if (decorateeWidget_)
			decorateeWidget_->OnReshape(width, height);
		for (auto &w : children_)
		{
			w->OnReshape(width, height);
		}
	}
	virtual void OnRender()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnRender();
		for (auto &w : children_)
		{
			w->OnRender();
		}
	}
	virtual void OnRenderOverlay()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnRenderOverlay();
		for (auto &w : children_)
		{
			w->OnRenderOverlay();
		}
	}

	virtual void OnMainLoop()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnMainLoop();
		for (auto &w : children_)
		{
			w->OnMainLoop();
		}
	}
	virtual void OnLeaveMainLoop()
	{
		if (decorateeWidget_)
			decorateeWidget_->OnLeaveMainLoop();
		for (auto &w : children_)
		{
			w->OnLeaveMainLoop();
		}
	}

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

	inline bool is_leaf() const { return leaf_; }
	inline void clear()
	{
		throwIfLeaf();
		children_.clear();
	}
	inline size_t size() const
	{
		if (leaf_)
			return 0;
		return children_.size();
	}
	inline bool empty() const
	{
		if (leaf_)
			return true;
		return children_.empty();
	}
	inline void push_back(const SharedPtr &child_)
	{
		throwIfLeaf();
		children_.push_back(child_);
		child_->parent_ = shared_from_this();
	}
	inline void emplace_back(SharedPtr &&child_)
	{
		throwIfLeaf();
		children_.emplace_back(child_);
		child_->parent_ = shared_from_this();
	}
	inline void pop_back()
	{
		throwIfLeaf();
		if (!empty())
			children_.pop_back();
	}
	inline SharedPtr operator[](size_t _Pos)
	{
		if (throwIfLeaf())
			return SharedPtr(nullptr);
		return children_.at(_Pos);
	}
	inline const SharedPtr operator[](size_t _Pos) const
	{
		if (throwIfLeaf())
			return SharedPtr(nullptr);
		return children_.at(_Pos);
	}

	inline iterator begin()
	{
		throwIfLeaf();
		return children_.begin();
	}
	inline const_iterator begin() const
	{
		throwIfLeaf();
		return children_.begin();
	}
	inline iterator end()
	{
		throwIfLeaf();
		return children_.end();
	}
	inline const_iterator end() const
	{
		throwIfLeaf();
		return children_.end();
	}
	inline const_iterator cbegin() const
	{
		throwIfLeaf();
		return children_.cbegin();
	}
	inline const_iterator cend() const
	{
		throwIfLeaf();
		return children_.cend();
	}
	inline const value_type &at(const size_t _Pos) const
	{
		throwIfLeaf();
		return children_.at(_Pos);
	}
	inline value_type &at(const size_t _Pos)
	{
		throwIfLeaf();
		return children_.at(_Pos);
	}
	inline value_type *data()
	{
		throwIfLeaf();
		return children_.data();
	}
	inline const value_type *data() const
	{
		throwIfLeaf();
		return children_.data();
	}

	// Properties
	inline const SharedPtr &decoratee() const { return decorateeWidget_; }
	inline SharedPtr &decoratee() { return decorateeWidget_; }
	inline const SharedPtr &decorator() const { return decoraterWidget_; }
	inline SharedPtr &decorator() { return decoraterWidget_; }
	inline const SharedPtr &parent() const { return parent_; }
	inline SharedPtr &parent() { return parent_; }

	inline bool decorate(SharedPtr w) noexcept
	{
		if (!w)
			return false;
		decorateeWidget_ = w;
		w->decoraterWidget_ = shared_from_this();
		return true;
	}

	inline void undecorate()
	{
		if (!decoraterWidget_)
			return;
		decoraterWidget_->decorateeWidget_.reset();
	}

	inline bool decorating() const
	{
		if (decorateeWidget_)
			return true;
		return false;
	}

	inline int getX() const { return windowRect_.x; }
	inline int getY() const { return windowRect_.y; }
	inline int getHeight() const { return windowRect_.w; }
	inline int getWidth() const { return windowRect_.h; }
	inline const Recti &windowRect() const { return windowRect_; }
	inline Recti &windowRect() { return windowRect_; }
	inline bool visible() const { return visible_; }
	inline bool is_orpha() const { return !parent_; }

	inline double GetElapsedTime() const { return t1 - baseTime; }
	inline double GetFrameTime() const { return t1 - t0; }
	inline double GetStopwatchTime() const { return t1 - stopwatchTimeT0; }
	inline void ResetStopwatch() { stopwatchTimeT0 = t1; }
	inline void ResetClock()
	{
		baseTime = t1;
		t0 = t1;
	}

	inline const std::string &caption() const noexcept { return caption_; }
	inline std::string &caption() noexcept { return caption_; }
	inline const std::string &style() const noexcept { return style_; }
	inline const KASL::JSONPtr &jsonStyle() const noexcept { return jsonStyle_; }
	inline KASL::JSONPtr jsonStyle() noexcept { return jsonStyle_; }

	inline KASL::JSONPtr setStyle(const std::string &style) noexcept
	{
		processStyle(style);
		return jsonStyle_;
	}

  private:
	SharedPtr decorateeWidget_;
	SharedPtr decoraterWidget_;
	SharedPtr parent_;
	std::vector<SharedPtr> children_;

	Recti windowRect_;
	bool visible_ = true;

	// A class should set leaf_ to false if it's okay to be a container, true if not
	bool leaf_ = false;
	inline bool throwIfLeaf() const
	{
		if (!leaf_)
			return false;
		throw std::out_of_range("this class instance is a leaf, not a container");
		return true;
	}

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
	KASL::JSONPtr jsonStyle_;
	std::string style_;
	std::string caption_;
	void makeLeaf(bool state) { leaf_ = state; }

	const double getT0() const { return t0; }
	const double getT1() const { return t1; }
};

//template <typename T, typename T2>
//std::shared_ptr<T> Make(std::shared_ptr<T2> & decoratee) {
//	if (decoratee) return make_shared<T>((std::shared_ptr<Widget>)(decoratee));
//	return make_shared<T>();
//}

//template <typename T>
//std::shared_ptr<T> Make() {
//	return make_shared<T>();
//}
} // namespace Viperfish

#endif
