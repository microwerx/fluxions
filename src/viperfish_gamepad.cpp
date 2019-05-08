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
#include "pch.h"
#include <chrono>
#include <viperfish.hpp>
#include <viperfish_gamepad.hpp>

namespace Viperfish
{
GamepadState::GamepadState()
{
    Init(0);
}

GamepadState::~GamepadState()
{
}

void GamepadState::Init(unsigned whichIndex)
{
    index = whichIndex;
    isInitialized = false;
    Clear();
    connected = true;
    reset();
    isInitialized = true;
}

void GamepadState::Clear()
{
    axes.clear();
    buttons.clear();
    deltaAxes.clear();
    deltaButtons.clear();
    lastAxes.clear();
    lastButtons.clear();
    timestamp = SteadyClockNow() / 1e-3;
}

void GamepadState::Poll()
{
#ifdef WIN32
    if (index < 0 || index >= XUSER_MAX_COUNT)
        return;

    timestamp = SteadyClockNow() / 1e-3;

    XINPUT_STATE state;
    DWORD result = XInputGetState(index, &state);

    if (result == ERROR_SUCCESS)
    {
        connected = true;
        if (!isInitialized)
        {
            reset();
            isInitialized = true;
        }

        lastState = curState;
        curState.BitSet((unsigned)GamePadBitNum::A, state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
        curState.BitSet((unsigned)GamePadBitNum::B, state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
        curState.BitSet((unsigned)GamePadBitNum::X, state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
        curState.BitSet((unsigned)GamePadBitNum::Y, state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
        curState.BitSet((unsigned)GamePadBitNum::LEFT, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        curState.BitSet((unsigned)GamePadBitNum::RIGHT, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        curState.BitSet((unsigned)GamePadBitNum::UP, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        curState.BitSet((unsigned)GamePadBitNum::DOWN, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        curState.BitSet((unsigned)GamePadBitNum::SELECT, state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
        curState.BitSet((unsigned)GamePadBitNum::START, state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
        curState.BitSet((unsigned)GamePadBitNum::L1, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        curState.BitSet((unsigned)GamePadBitNum::L3, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        curState.BitSet((unsigned)GamePadBitNum::R1, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        curState.BitSet((unsigned)GamePadBitNum::R3, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

        float thumbLX = std::abs(state.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbLX / 32768.0f;
        float thumbLY = std::abs(state.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbLY / 32768.0f;
        float thumbRX = std::abs(state.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbRX / 32768.0f;
        float thumbRY = std::abs(state.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbRY / 32768.0f;
        float leftTrigger = std::abs(state.Gamepad.bLeftTrigger) < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0.0f : state.Gamepad.bLeftTrigger / 255.0f;
        float rightTrigger = std::abs(state.Gamepad.bRightTrigger) < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0.0f : state.Gamepad.bRightTrigger / 255.0f;

        curState.BitSetf((unsigned)GamePadBitNum::LTHUMB_X, thumbLX);
        curState.BitSetf((unsigned)GamePadBitNum::LTHUMB_Y, thumbLY);
        curState.BitSetf((unsigned)GamePadBitNum::RTHUMB_X, thumbRX);
        curState.BitSetf((unsigned)GamePadBitNum::RTHUMB_Y, thumbRY);
        curState.BitSetf((unsigned)GamePadBitNum::L2, leftTrigger);
        curState.BitSetf((unsigned)GamePadBitNum::R2, rightTrigger);

        lthumbVector_.reset(thumbLX, thumbLY);
        lthumbAmount_ = lthumbVector_.length();
        lthumbVector_.normalize();
        rthumbVector_.reset(thumbRX, thumbRY);
        rthumbAmount_ = rthumbVector_.length();
        rthumbVector_.normalize();
    }
    else if (result == ERROR_DEVICE_NOT_CONNECTED)
    {
        if (isInitialized)
        {
            reset();
        }
        connected = false;
        isInitialized = false;
    }
    else
    {
        hflog.info("%s(): XInputError: returned %d", __FUNCTION__, result);
    }

    makeHexRepresentation();
#endif // WIN32
}

void GamepadState::SetButton(GamePadBitNum bit, bool pressed)
{
    curState.BitSet((unsigned)bit, pressed);
    makeHexRepresentation();
}

void GamepadState::SetButton(GamePadBitNum bit, float value)
{
    curState.BitSetf((unsigned)bit, value);
    makeHexRepresentation();
}

void GamepadState::reset()
{
    curState.BitSet(0);
    lastState.BitSet(0);
    lthumbVector_.reset();
    rthumbVector_.reset();
    lthumbAmount_ = 0.0f;
    rthumbAmount_ = 0.0f;
}

void GamepadState::makeHexRepresentation()
{
    // The hex representation makes each bit look like 0 through f where 0 = -1 and f = 1
    hexRepresentation_.resize((int)GamePadBitNum::NUMBITS + 2);
    hexRepresentation_[0] = '0' + (char)index;

    if (connected)
    {
        hexRepresentation_[1] = 'Y';
    }
    else
    {
        hexRepresentation_[1] = 'N';
    }

    for (int i = 0; i < (int)GamePadBitNum::NUMBITS; i++)
    {
        float f = 15.99f * (0.5f * (1.0f + clamp(curState.GetBitf(i), -1.0f, 1.0f)));
        char h = (char)clamp((int)f, 0, 15);
        char c;
        if (h < 10)
        {
            c = '0' + h;
        }
        else
        {
            c = 'A' + h - 10;
        }
        hexRepresentation_[i + 2] = c;
    }
}
} // namespace Viperfish
