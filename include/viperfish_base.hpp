#ifndef VIPERFISH_BASE_HPP
#define VIPERFISH_BASE_HPP

#include <fluxions_stdcxx.hpp>
#include <viperfish_opengl.hpp>

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