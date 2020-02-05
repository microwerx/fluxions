#include "viperfish_pch.hpp"
#include <viperfish_gamepad.hpp>
#include <viperfish_utilities.hpp>
#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#endif
#include <hatchetfish.hpp>
#include <fluxions_gte_scalar_math.hpp>

namespace Vf {
	GamepadState::GamepadState() {
		Init(0);
	}

	GamepadState::~GamepadState() {}

	void GamepadState::Init(unsigned whichIndex) {
		index = whichIndex;
		isInitialized = false;
		Clear();
		connected = true;
		reset();
		isInitialized = true;
	}

	void GamepadState::Clear() {
		axes.clear();
		buttons.clear();
		deltaAxes.clear();
		deltaButtons.clear();
		lastAxes.clear();
		lastButtons.clear();
		timestamp = SteadyClockNow() / 1e-3;
	}

	void GamepadState::Poll() {
#ifdef _WIN32
		if (index < 0 || index >= XUSER_MAX_COUNT)
			return;

		timestamp = SteadyClockNow() / 1e-3;

		XINPUT_STATE state;
		DWORD result = XInputGetState(index, &state);

		if (result == ERROR_SUCCESS) {
			connected = true;
			if (!isInitialized) {
				reset();
				isInitialized = true;
			}

			lastState = curState;
			curState.bitSet((unsigned)GamePadBitNum::A, state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
			curState.bitSet((unsigned)GamePadBitNum::B, state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
			curState.bitSet((unsigned)GamePadBitNum::X, state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
			curState.bitSet((unsigned)GamePadBitNum::Y, state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
			curState.bitSet((unsigned)GamePadBitNum::LEFT, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			curState.bitSet((unsigned)GamePadBitNum::RIGHT, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			curState.bitSet((unsigned)GamePadBitNum::UP, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
			curState.bitSet((unsigned)GamePadBitNum::DOWN, state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
			curState.bitSet((unsigned)GamePadBitNum::SELECT, state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
			curState.bitSet((unsigned)GamePadBitNum::START, state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
			curState.bitSet((unsigned)GamePadBitNum::L1, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
			curState.bitSet((unsigned)GamePadBitNum::L3, state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
			curState.bitSet((unsigned)GamePadBitNum::R1, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
			curState.bitSet((unsigned)GamePadBitNum::R3, state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

			float thumbLX = std::abs(state.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbLX / 32768.0f;
			float thumbLY = std::abs(state.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbLY / 32768.0f;
			float thumbRX = std::abs(state.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbRX / 32768.0f;
			float thumbRY = std::abs(state.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? 0.0f : state.Gamepad.sThumbRY / 32768.0f;
			float leftTrigger = std::abs(state.Gamepad.bLeftTrigger) < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0.0f : state.Gamepad.bLeftTrigger / 255.0f;
			float rightTrigger = std::abs(state.Gamepad.bRightTrigger) < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0.0f : state.Gamepad.bRightTrigger / 255.0f;

			curState.bitSetf((unsigned)GamePadBitNum::LTHUMB_X, thumbLX);
			curState.bitSetf((unsigned)GamePadBitNum::LTHUMB_Y, thumbLY);
			curState.bitSetf((unsigned)GamePadBitNum::RTHUMB_X, thumbRX);
			curState.bitSetf((unsigned)GamePadBitNum::RTHUMB_Y, thumbRY);
			curState.bitSetf((unsigned)GamePadBitNum::L2, leftTrigger);
			curState.bitSetf((unsigned)GamePadBitNum::R2, rightTrigger);

			lthumbVector_.reset(thumbLX, thumbLY);
			lthumbAmount_ = lthumbVector_.length();
			lthumbVector_.normalize();
			rthumbVector_.reset(thumbRX, thumbRY);
			rthumbAmount_ = rthumbVector_.length();
			rthumbVector_.normalize();
		}
		else if (result == ERROR_DEVICE_NOT_CONNECTED) {
			if (isInitialized) {
				reset();
			}
			connected = false;
			isInitialized = false;
		}
		else {
			HFLOGERROR("XInputError: returned %d", result);
		}

		makeHexRepresentation();
#endif // _WIN32
	}

	void GamepadState::SetButton(GamePadBitNum bit, bool pressed) {
		curState.bitSet((unsigned)bit, pressed);
		makeHexRepresentation();
	}

	void GamepadState::SetButton(GamePadBitNum bit, float value) {
		curState.bitSetf((unsigned)bit, value);
		makeHexRepresentation();
	}

	void GamepadState::reset() {
		curState.bitSet(0);
		lastState.bitSet(0);
		lthumbVector_.reset();
		rthumbVector_.reset();
		lthumbAmount_ = 0.0f;
		rthumbAmount_ = 0.0f;
	}

	void GamepadState::makeHexRepresentation() {
		// The hex representation makes each bit look like 0 through f where 0 = -1 and f = 1
		hexRepresentation_.resize((int)GamePadBitNum::NUMBITS + 2);
		hexRepresentation_[0] = '0' + (char)index;

		if (connected) {
			hexRepresentation_[1] = 'Y';
		}
		else {
			hexRepresentation_[1] = 'N';
		}

		for (int i = 0; i < (int)GamePadBitNum::NUMBITS; i++) {
			float f = 15.99f * (0.5f * (1.0f + Fluxions::clamp(curState.getBitf(i), -1.0f, 1.0f)));
			char h = (char)Fluxions::clamp((int)f, 0, 15);
			char c;
			if (h < 10) {
				c = '0' + h;
			}
			else {
				c = 'A' + h - 10;
			}
			hexRepresentation_[i + 2] = c;
		}
	}
} // namespace Vf
