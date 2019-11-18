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
#ifndef VIPERFISH_XINPUT_HPP
#define VIPERFISH_XINPUT_HPP

#include <viperfish_gamepad.hpp>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

namespace Vf
{
	class XInputGamepadState : public GamepadState
	{
	public:
		XInputGamepadState();
		~XInputGamepadState();

		virtual void Init(unsigned whichStickID);
		virtual void Poll();
		virtual bool IsConnected();

	private:
		constexpr double iff(bool status, double trueValue = 1.0, double falseValue = 0.0) const noexcept
		{
			if (status)
				return trueValue;
			else
				return falseValue;
		}
		DWORD lastPacketNumber;
		XINPUT_STATE gamepadState;
		XINPUT_VIBRATION vibrationState;
	};
} // namespace Vf

#endif // _WIN32

#endif
