#ifndef VIPERFISH_HPP
#define VIPERFISH_HPP

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>
#include <functional>
#include <chrono>

#include <viperfish_keyboard.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_gamepad.hpp>
#include <viperfish_xinput.hpp>
#include <viperfish_utilities.hpp>
#include <viperfish_widget.hpp>
#include <viperfish_dear_imgui.hpp>

#define VF_MOUSE_BUTTON_LEFT	0x0000;
#define VF_MOUSE_BUTTON_MIDDLE	0x0001;
#define VF_MOUSE_BUTTON_RIGHT	0x0002;

namespace Vf
{
	template <class T, class... _Types>
	static std::shared_ptr<T> MakeShared(_Types&&... _Args) {
		return std::shared_ptr<T>(new T(std::forward<_Types>(_Args)...));
	}

	template <class T, class... _Types>
	static std::unique_ptr<T> MakeUnique(_Types&&... _Args) {
		return std::unique_ptr<T>(new T(std::forward<_Types>(_Args)...));
	}

	template <class T, class... _Types>
	static std::shared_ptr<T> MakeSharedChild(WidgetPtr& parent, _Types&&... _Args) {
		auto child = std::shared_ptr<T>(new T(std::forward<_Types>(_Args)...));
		parent->push_back(child);
		return child;
	}

	template <class T, class... _Types>
	static std::shared_ptr<T> MakeSharedDecorator(WidgetPtr& decoratee, _Types&&... _Args) {
		auto decorator = std::shared_ptr<T>(new T(std::forward<_Types>(_Args)...));
		decoratee->decorateWith(std::dynamic_pointer_cast<Widget>(decorator));
		return decorator;
	}

} // namespace Vf

#endif
