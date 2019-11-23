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
#include "pch.hpp"
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <Xinput.h>
#include <viperfish_xinput.hpp>

namespace Vf
{
XInputGamepadState::XInputGamepadState() {}

XInputGamepadState::~XInputGamepadState() {}

void XInputGamepadState::Init(unsigned whichStickID) {
	GamepadState::Init(whichStickID);
	ZeroMemory(&gamepadState, sizeof(XINPUT_GAMEPAD));
	ZeroMemory(&vibrationState, sizeof(XINPUT_VIBRATION));
	DWORD result = XInputGetState(index, &gamepadState);
	if (result == ERROR_SUCCESS) {
		isInitialized = true;
	}
	else if (result == ERROR_DEVICE_NOT_CONNECTED) {
		isInitialized = false;
	}
	else {
		isInitialized = false;
	}
	Poll();
}

float stickToFloat(short x, short deadzone) {
	return fabs(x) < deadzone ? 0.0f : (x / 32768.0f);
}

void XInputGamepadState::Poll() {
	GamepadState::Poll();
	DWORD result = XInputGetState(index, &gamepadState);
	if (result == ERROR_DEVICE_NOT_CONNECTED)
		connected = false;
	if (result != ERROR_SUCCESS)
		return;
	if (lastPacketNumber == gamepadState.dwPacketNumber)
		return;
	lastPacketNumber = gamepadState.dwPacketNumber;
	connected = true;
	isInitialized = true;

	swap(lastAxes, axes);
	swap(lastButtons, buttons);



	// compute delta's and stick information
	axes[0] = stickToFloat(gamepadState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	axes[1] = stickToFloat(gamepadState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	axes[2] = stickToFloat(gamepadState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	axes[3] = stickToFloat(gamepadState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//float thumblx = (float)std::max<int>(0, abs(gamepadState.Gamepad.sThumbLX) - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
 //   float thumbly = (float)std::max<int>(0, abs(gamepadState.Gamepad.sThumbLY) - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
 //   float thumbrx = (float)std::max<int>(0, abs(gamepadState.Gamepad.sThumbRX) - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
 //   float thumbry = (float)std::max<int>(0, abs(gamepadState.Gamepad.sThumbRY) - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//float stlx = gamepadState.Gamepad.sThumbLX < 0 ? -1.0f : 1.0f;
	//float stly = gamepadState.Gamepad.sThumbLY < 0 ? -1.0f : 1.0f;
	//float strx = gamepadState.Gamepad.sThumbRX < 0 ? -1.0f : 1.0f;
	//float stry = gamepadState.Gamepad.sThumbRY < 0 ? -1.0f : 1.0f;
	//axes[0] = thumblx * stlx / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//axes[1] = -thumbly * stly / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//axes[2] = thumbrx * strx / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//axes[3] = -thumbry * stry / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	float leftshoulder = gamepadState.Gamepad.bLeftTrigger / 255.0f;
	float rightshoulder = gamepadState.Gamepad.bRightTrigger / 255.0f;
	buttons[6].pressed = gamepadState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	buttons[7].pressed = gamepadState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	buttons[6].value = leftshoulder;
	buttons[7].value = rightshoulder;

	buttons[0].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
	buttons[1].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
	buttons[2].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
	buttons[3].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
	buttons[4].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
	buttons[5].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	// 6 and 7 handled above
	buttons[8].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
	buttons[9].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
	buttons[10].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
	buttons[11].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
	buttons[12].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
	buttons[13].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	buttons[14].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
	buttons[15].pressed = (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

	// set floating point values except for 6 and 7 which are the shoulder triggers
	for (int i = 0; i < 16; i++) {
		if (i >= 6 && i <= 7)
			continue;
		buttons[i].value = buttons[i].pressed ? 1.0f : 0.0f;
	}

	// compute delta's
	for (int i = 0; i < 4; i++) {
		deltaAxes[i] = axes[i] - lastAxes[i];
		deltaButtons[i].pressed = buttons[i].pressed;
		deltaButtons[i].value = buttons[i].value - lastButtons[i].value;
	}
}

bool XInputGamepadState::IsConnected() {
	XINPUT_STATE xis;
	DWORD result = XInputGetState(index, &xis);
	if (result == ERROR_DEVICE_NOT_CONNECTED) {
		connected = false;
	}
	else if (result == ERROR_SUCCESS) {
		connected = true;
	}
	return GamepadState::IsConnected();
}
}

#endif // _WIN32
