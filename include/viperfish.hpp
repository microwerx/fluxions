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
#include <viperfish_stopwatch.hpp>
#include <viperfish_widget.hpp>
#include <viperfish_dear_imgui.hpp>

namespace Viperfish
{

template <class T, class... _Types>
static std::shared_ptr<T> MakeShared(_Types &&... _Args) { return std::shared_ptr<T>(new T(std::forward<_Types>(_Args)...)); }
template <class T, class... _Types>
static std::unique_ptr<T> MakeUnique(_Types &&... _Args) { return std::unique_ptr<T>(new T(std::forward<_Types>(_Args)...)); }
} // namespace Viperfish

#endif
