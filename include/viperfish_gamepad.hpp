#ifndef VIPERFISH_GAMEPAD_HPP
#define VIPERFISH_GAMEPAD_HPP

#include <viperfish_base.hpp>

namespace Vf {
	class AnalogBitset {
	public:
		AnalogBitset();

		bool bitTest(unsigned bitsToTest) const;
		float getBitf(unsigned which) const {
			if (within(which, 0, 31))
				return fbits[which];
			return 0.0f;
		}
		bool getBiti(unsigned which) const {
			if (within(which, 0, 31))
				return ibits & (1 << which);
			return false;
		}
		void bitOn(unsigned which);
		void bitOff(unsigned which);
		void bitSetf(unsigned which, float value);
		void bitSet(unsigned bitsToSet);
		inline void bitSet(unsigned which, bool value) { bitSetf(which, value ? 1.0f : 0.0f); }
		inline bool within(unsigned x, unsigned a, unsigned b) const { return x >= a && x <= b; }

	private:
		float fbits[32] = { 0.0f };
		unsigned ibits;
	};

	enum class GamePadBitNum {
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		R1 = 4,
		L1 = 5,
		R2 = 6,
		L2 = 7,
		SELECT = 8,
		START = 9,
		L3 = 10,
		R3 = 11,
		UP = 12,
		DOWN = 13,
		LEFT = 14,
		RIGHT = 15,
		LTHUMB_X = 16,
		LTHUMB_Y = 17,
		RTHUMB_X = 18,
		RTHUMB_Y = 19,
		LTRIGGER = 20,
		RTRIGGER = 21,
		NUMBITS = 22
	};

	enum class GamePadBits {
		A = 1 << (int)GamePadBitNum::A,
		B = 1 << (int)GamePadBitNum::B,
		X = 1 << (int)GamePadBitNum::X,
		Y = 1 << (int)GamePadBitNum::Y,
		R1 = 1 << (int)GamePadBitNum::R1,
		L1 = 1 << (int)GamePadBitNum::L1,
		R2 = 1 << (int)GamePadBitNum::R2,
		L2 = 1 << (int)GamePadBitNum::L2,
		SELECT = 1 << (int)GamePadBitNum::SELECT,
		START = 1 << (int)GamePadBitNum::START,
		L3 = 1 << (int)GamePadBitNum::L3,
		R3 = 1 << (int)GamePadBitNum::R3,
		UP = 1 << (int)GamePadBitNum::UP,
		DOWN = 1 << (int)GamePadBitNum::DOWN,
		LEFT = 1 << (int)GamePadBitNum::LEFT,
		RIGHT = 1 << (int)GamePadBitNum::RIGHT,
		LTHUMB_X = 1 << (int)GamePadBitNum::LTHUMB_X,
		LTHUMB_Y = 1 << (int)GamePadBitNum::LTHUMB_Y,
		RTHUMB_X = 1 << (int)GamePadBitNum::RTHUMB_X,
		RTHUMB_Y = 1 << (int)GamePadBitNum::RTHUMB_Y,
		LTRIGGER = 1 << (int)GamePadBitNum::LTRIGGER,
		RTRIGGER = 1 << (int)GamePadBitNum::RTRIGGER
	};

	class GamepadState {
	public:
		struct GamepadButton {
			float value;
			bool pressed;
		};

		// These match the HTML5 Gamepad API
		long index = 0;
		bool connected = false;
		std::map<int, float> axes;
		std::map<int, GamepadButton> buttons;
		double timestamp; // milliseconds

		std::map<int, float> lastAxes;
		std::map<int, GamepadButton> lastButtons;
		std::map<int, float> deltaAxes;
		std::map<int, GamepadButton> deltaButtons;
		double lasttimestamp = 0.0;
		bool isInitialized = false;

		GamepadState();
		virtual ~GamepadState();

		virtual void Init(unsigned whichStickId);
		virtual void Clear();
		virtual void Poll();
		virtual bool IsInitialized() const { return isInitialized; }
		virtual bool IsConnected() { return connected; }
		void SetButton(GamePadBitNum bit, bool pressed);
		void SetButton(GamePadBitNum bit, float value);

		inline bool aPressed() const { return curState.getBiti((int)GamePadBitNum::A); }
		inline bool bPressed() const { return curState.getBiti((int)GamePadBitNum::B); }
		inline bool xPressed() const { return curState.getBiti((int)GamePadBitNum::X); }
		inline bool yPressed() const { return curState.getBiti((int)GamePadBitNum::Y); }
		inline bool r1Pressed() const { return curState.getBiti((int)GamePadBitNum::R1); }
		inline bool l1Pressed() const { return curState.getBiti((int)GamePadBitNum::L1); }
		inline bool r2Pressed() const { return curState.getBiti((int)GamePadBitNum::R2); }
		inline bool l2Pressed() const { return curState.getBiti((int)GamePadBitNum::L2); }
		inline bool selectPressed() const { return curState.getBiti((int)GamePadBitNum::SELECT); }
		inline bool startPressed() const { return curState.getBiti((int)GamePadBitNum::START); }
		inline bool l3Pressed() const { return curState.getBiti((int)GamePadBitNum::L3); }
		inline bool r3Pressed() const { return curState.getBiti((int)GamePadBitNum::R3); }
		inline bool upPressed() const { return curState.getBiti((int)GamePadBitNum::UP); }
		inline bool downPressed() const { return curState.getBiti((int)GamePadBitNum::DOWN); }
		inline bool leftPressed() const { return curState.getBiti((int)GamePadBitNum::LEFT); }
		inline bool rightPressed() const { return curState.getBiti((int)GamePadBitNum::RIGHT); }
		inline bool lthumbDown() const { return lthumb_y() < 0.0f; }
		inline bool lthumbUp() const { return lthumb_y() > 0.0f; }
		inline bool lthumbLeft() const { return lthumb_x() < 0.0f; }
		inline bool lthumbRight() const { return lthumb_x() > 0.0f; }
		inline bool rthumbDown() const { return rthumb_y() < 0.0f; }
		inline bool rthumbUp() const { return rthumb_y() > 0.0f; }
		inline bool rthumbLeft() const { return rthumb_x() < 0.0f; }
		inline bool rthumbRight() const { return rthumb_x() > 0.0f; }

		inline float a() const { return curState.getBitf((int)GamePadBitNum::A); }
		inline float b() const { return curState.getBitf((int)GamePadBitNum::B); }
		inline float x() const { return curState.getBitf((int)GamePadBitNum::X); }
		inline float y() const { return curState.getBitf((int)GamePadBitNum::Y); }
		inline float r1() const { return curState.getBitf((int)GamePadBitNum::R1); }
		inline float l1() const { return curState.getBitf((int)GamePadBitNum::L1); }
		inline float r2() const { return curState.getBitf((int)GamePadBitNum::R2); }
		inline float l2() const { return curState.getBitf((int)GamePadBitNum::L2); }
		inline float select() const { return curState.getBitf((int)GamePadBitNum::SELECT); }
		inline float start() const { return curState.getBitf((int)GamePadBitNum::START); }
		inline float l3() const { return curState.getBitf((int)GamePadBitNum::L3); }
		inline float r3() const { return curState.getBitf((int)GamePadBitNum::R3); }
		inline float up() const { return curState.getBitf((int)GamePadBitNum::UP); }
		inline float down() const { return curState.getBitf((int)GamePadBitNum::DOWN); }
		inline float left() const { return curState.getBitf((int)GamePadBitNum::LEFT); }
		inline float right() const { return curState.getBitf((int)GamePadBitNum::RIGHT); }
		inline float lthumb_x() const { return curState.getBitf((int)GamePadBitNum::LTHUMB_X); }
		inline float lthumb_y() const { return curState.getBitf((int)GamePadBitNum::LTHUMB_Y); }
		inline float rthumb_x() const { return curState.getBitf((int)GamePadBitNum::RTHUMB_X); }
		inline float rthumb_y() const { return curState.getBitf((int)GamePadBitNum::RTHUMB_Y); }

		const std::string& GetHexRepresentation() const { return hexRepresentation_; }

		// lthumb() returns the unnormalized direction of the left thumb stick
		inline Fluxions::Vector2f lthumb() const { return Fluxions::Vector2f(lthumb_x(), lthumb_y()); }
		// lthumbVector() returns the normalized direction of the left thumb stick
		inline Fluxions::Vector2f lthumbVector() const { return lthumbVector_; }
		// lthumbAmount() returns the magnitude of the left thumb stick direction
		inline float lthumbAmount() const { return lthumbAmount_; }
		// rthumb() returns the unnormalized direction of the right thumb stick
		inline Fluxions::Vector2f rthumb() const { return Fluxions::Vector2f(rthumb_x(), rthumb_y()); }
		// rthumbVector() returns the normalized direction of the right thumb stick
		inline Fluxions::Vector2f rthumbVector() const { return rthumbVector_; }
		// rthumbAmount() returns the magnitude of the right thumb stick direction
		inline float rthumbAmount() const { return rthumbAmount_; }

	private:
		void reset();
		void makeHexRepresentation();

		AnalogBitset curState;
		AnalogBitset lastState;
		Fluxions::Vector2f lthumbVector_;
		float lthumbAmount_;
		Fluxions::Vector2f rthumbVector_;
		float rthumbAmount_;

		std::string hexRepresentation_;
		std::string binRepresentation_;
	};
} // namespace Vf

#endif
