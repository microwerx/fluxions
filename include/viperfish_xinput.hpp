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
#ifndef VIPERFISH_XINPUT_HPP
#define VIPERFISH_XINPUT_HPP

#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>

#ifdef WIN32
#define NOMINMAX
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

namespace Viperfish
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
} // namespace Viperfish

#endif // WIN32

#endif
