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
#ifndef VIPERFISH_GAMEPAD_HPP
#define VIPERFISH_GAMEPAD_HPP

#include <map>
#include <string>
#include <fluxions_gte_vector2.hpp>

namespace Vf
{

	class AnalogBitset
	{
	public:
		AnalogBitset();

		bool BitTest(unsigned bitsToTest) const;
		float GetBitf(unsigned which) const
		{
			if (within(which, 0, 31))
				return fbits[which];
			return 0.0f;
		}
		bool GetBiti(unsigned which) const
		{
			if (within(which, 0, 31))
				return ibits & (1 << which);
			return false;
		}
		void BitOn(unsigned which);
		void BitOff(unsigned which);
		void BitSetf(unsigned which, float value);
		void BitSet(unsigned bitsToSet);
		inline void BitSet(unsigned which, bool value) { BitSetf(which, value ? 1.0f : 0.0f); }
		inline bool within(unsigned x, unsigned a, unsigned b) const { return x >= a && x <= b; }

	private:
		float fbits[32] = { 0.0f };
		unsigned ibits;
	};

	enum class GamePadBitNum
	{
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

	enum class GamePadBits
	{
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

	class GamepadState
	{
	public:
		struct GamepadButton
		{
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

		inline bool aPressed() const { return curState.GetBiti((int)GamePadBitNum::A); }
		inline bool bPressed() const { return curState.GetBiti((int)GamePadBitNum::B); }
		inline bool xPressed() const { return curState.GetBiti((int)GamePadBitNum::X); }
		inline bool yPressed() const { return curState.GetBiti((int)GamePadBitNum::Y); }
		inline bool r1Pressed() const { return curState.GetBiti((int)GamePadBitNum::R1); }
		inline bool l1Pressed() const { return curState.GetBiti((int)GamePadBitNum::L1); }
		inline bool r2Pressed() const { return curState.GetBiti((int)GamePadBitNum::R2); }
		inline bool l2Pressed() const { return curState.GetBiti((int)GamePadBitNum::L2); }
		inline bool selectPressed() const { return curState.GetBiti((int)GamePadBitNum::SELECT); }
		inline bool startPressed() const { return curState.GetBiti((int)GamePadBitNum::START); }
		inline bool l3Pressed() const { return curState.GetBiti((int)GamePadBitNum::L3); }
		inline bool r3Pressed() const { return curState.GetBiti((int)GamePadBitNum::R3); }
		inline bool upPressed() const { return curState.GetBiti((int)GamePadBitNum::UP); }
		inline bool downPressed() const { return curState.GetBiti((int)GamePadBitNum::DOWN); }
		inline bool leftPressed() const { return curState.GetBiti((int)GamePadBitNum::LEFT); }
		inline bool rightPressed() const { return curState.GetBiti((int)GamePadBitNum::RIGHT); }
		inline bool lthumbDown() const { return lthumb_y() < 0.0f; }
		inline bool lthumbUp() const { return lthumb_y() > 0.0f; }
		inline bool lthumbLeft() const { return lthumb_x() < 0.0f; }
		inline bool lthumbRight() const { return lthumb_x() > 0.0f; }
		inline bool rthumbDown() const { return rthumb_y() < 0.0f; }
		inline bool rthumbUp() const { return rthumb_y() > 0.0f; }
		inline bool rthumbLeft() const { return rthumb_x() < 0.0f; }
		inline bool rthumbRight() const { return rthumb_x() > 0.0f; }

		inline float a() const { return curState.GetBitf((int)GamePadBitNum::A); }
		inline float b() const { return curState.GetBitf((int)GamePadBitNum::B); }
		inline float x() const { return curState.GetBitf((int)GamePadBitNum::X); }
		inline float y() const { return curState.GetBitf((int)GamePadBitNum::Y); }
		inline float r1() const { return curState.GetBitf((int)GamePadBitNum::R1); }
		inline float l1() const { return curState.GetBitf((int)GamePadBitNum::L1); }
		inline float r2() const { return curState.GetBitf((int)GamePadBitNum::R2); }
		inline float l2() const { return curState.GetBitf((int)GamePadBitNum::L2); }
		inline float select() const { return curState.GetBitf((int)GamePadBitNum::SELECT); }
		inline float start() const { return curState.GetBitf((int)GamePadBitNum::START); }
		inline float l3() const { return curState.GetBitf((int)GamePadBitNum::L3); }
		inline float r3() const { return curState.GetBitf((int)GamePadBitNum::R3); }
		inline float up() const { return curState.GetBitf((int)GamePadBitNum::UP); }
		inline float down() const { return curState.GetBitf((int)GamePadBitNum::DOWN); }
		inline float left() const { return curState.GetBitf((int)GamePadBitNum::LEFT); }
		inline float right() const { return curState.GetBitf((int)GamePadBitNum::RIGHT); }
		inline float lthumb_x() const { return curState.GetBitf((int)GamePadBitNum::LTHUMB_X); }
		inline float lthumb_y() const { return curState.GetBitf((int)GamePadBitNum::LTHUMB_Y); }
		inline float rthumb_x() const { return curState.GetBitf((int)GamePadBitNum::RTHUMB_X); }
		inline float rthumb_y() const { return curState.GetBitf((int)GamePadBitNum::RTHUMB_Y); }

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
