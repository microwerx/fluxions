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
#endif

#ifdef _WIN32
namespace Vf {
	class XInputGamepadState : public GamepadState {
	public:
		XInputGamepadState();
		~XInputGamepadState();

		virtual void Init(unsigned whichStickID);
		virtual void Poll();
		virtual bool IsConnected();

	private:
		constexpr double iff(bool status, double trueValue = 1.0, double falseValue = 0.0) const noexcept {
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
