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
#ifndef VIPERFISH_KEYBOARD_HPP
#define VIPERFISH_KEYBOARD_HPP

#include <map>
#include <vector>

constexpr unsigned VF_SHIFT_MODIFIER = 0x0001;
constexpr unsigned VF_CTRL_MODIFIER = 0x0002;
constexpr unsigned VF_ALT_MODIFIER = 0x0004;
constexpr unsigned VF_META_MODIFIER = 0x0008;
constexpr unsigned VF_CAPSLOCK_MODIFIER = 0x0010;
constexpr unsigned VF_NUMLOCK_MODIFIER = 0x0020;

constexpr unsigned VF_KEY_F1 = 0x0101;
constexpr unsigned VF_KEY_F2 = 0x0102;
constexpr unsigned VF_KEY_F3 = 0x0103;
constexpr unsigned VF_KEY_F4 = 0x0104;
constexpr unsigned VF_KEY_F5 = 0x0105;
constexpr unsigned VF_KEY_F6 = 0x0106;
constexpr unsigned VF_KEY_F7 = 0x0107;
constexpr unsigned VF_KEY_F8 = 0x0108;
constexpr unsigned VF_KEY_F9 = 0x0109;
constexpr unsigned VF_KEY_F10 = 0x010A;
constexpr unsigned VF_KEY_F11 = 0x010B;
constexpr unsigned VF_KEY_F12 = 0x010C;
constexpr unsigned VF_KEY_LEFT = 0x0164;
constexpr unsigned VF_KEY_UP = 0x0165;
constexpr unsigned VF_KEY_RIGHT = 0x0166;
constexpr unsigned VF_KEY_DOWN = 0x0167;
constexpr unsigned VF_KEY_PAGE_UP = 0x0168;
constexpr unsigned VF_KEY_PAGE_DOWN = 0x0169;
constexpr unsigned VF_KEY_HOME = 0x016A;
constexpr unsigned VF_KEY_END = 0x016B;
constexpr unsigned VF_KEY_INSERT = 0x016C;
constexpr unsigned VF_KEY_NUM_LOCK = 0x016D;
constexpr unsigned VF_KEY_BEGIN = 0x016E;
constexpr unsigned VF_KEY_DELETE = 0x016F;
constexpr unsigned VF_KEY_SHIFT_L = 0x0170;
constexpr unsigned VF_KEY_SHIFT_R = 0x0171;
constexpr unsigned VF_KEY_CTRL_L = 0x0172;
constexpr unsigned VF_KEY_CTRL_R = 0x0173;
constexpr unsigned VF_KEY_ALT_L = 0x0174;
constexpr unsigned VF_KEY_ALT_R = 0x0175;
constexpr unsigned VF_KEY_META_L = 0x0176;
constexpr unsigned VF_KEY_META_R = 0x0177;
constexpr unsigned VF_KEY_SUPER_L = 0x0176;
constexpr unsigned VF_KEY_SUPER_R = 0x0177;
constexpr unsigned VF_KEY_MENU = 0x0178;
constexpr unsigned VF_KEY_KP_0 = 0x140;
constexpr unsigned VF_KEY_KP_1 = 0x141;
constexpr unsigned VF_KEY_KP_2 = 0x142;
constexpr unsigned VF_KEY_KP_3 = 0x143;
constexpr unsigned VF_KEY_KP_4 = 0x144;
constexpr unsigned VF_KEY_KP_5 = 0x145;
constexpr unsigned VF_KEY_KP_6 = 0x146;
constexpr unsigned VF_KEY_KP_7 = 0x147;
constexpr unsigned VF_KEY_KP_8 = 0x148;
constexpr unsigned VF_KEY_KP_9 = 0x149;
constexpr unsigned VF_KEY_KP_DECIMAL = 0x150;
constexpr unsigned VF_KEY_KP_DIVIDE = 0x151;
constexpr unsigned VF_KEY_KP_MULTIPLY = 0x152;
constexpr unsigned VF_KEY_KP_SUBTRACT = 0x153;
constexpr unsigned VF_KEY_KP_ADD = 0x154;
constexpr unsigned VF_KEY_KP_ENTER = 0x155;
constexpr unsigned VF_KEY_KP_EQUAL = 0x156;

namespace Vf
{
	struct KeyboardState
	{
		std::map<std::string, int> keys;
		int modifiers = 0;

		static constexpr int ShiftKeyBit = VF_SHIFT_MODIFIER;
		static constexpr int CtrlKeyBit = VF_CTRL_MODIFIER;
		static constexpr int AltKeyBit = VF_ALT_MODIFIER;
		static constexpr int MetaKeyBit = VF_META_MODIFIER;
		static constexpr int ShiftCtrlBits = ShiftKeyBit | CtrlKeyBit;
		static constexpr int ShiftAltBits = ShiftKeyBit | AltKeyBit;
		static constexpr int CtrlAltBits = CtrlKeyBit | AltKeyBit;
		static constexpr int ShiftCtrlAltBits = ShiftKeyBit | CtrlKeyBit | AltKeyBit;

		inline bool ctrlKey() const { return modifiers & CtrlKeyBit; }
		inline bool altKey() const { return modifiers & AltKeyBit; }
		inline bool shiftKey() const { return modifiers & ShiftKeyBit; }
		inline bool ctrlAlt() const { return modifiers & (CtrlKeyBit | AltKeyBit); }
		inline bool shiftAlt() const { return modifiers & (ShiftKeyBit | AltKeyBit); }
		inline bool shiftCtrlAlt() const { return modifiers & (ShiftKeyBit | CtrlKeyBit | AltKeyBit); }

		inline void Clear() {
			keys.clear();
			modifiers = 0;
		}

		inline void reset() {
			modifiers = 0;
			for (auto& key : keys) {
				key.second = false;
			}
		}

		inline void SetKey(const std::string& key, bool state) {
			keys[key] = state;
		}

		inline void SetKey(const std::string& key, int keymod, bool state) {
			modifiers = keymod;
			keys[key] = state;
		}

		void SetKey(unsigned c, unsigned keymod, bool state);

		inline bool IsPressed(const std::string& key) const {
			std::map<std::string, int>::const_iterator it = keys.find(key);
			if (it != keys.end()) {
				return it->second;
			}
			return false;
		}

		inline bool IsPressed(const std::string& key, int keymod) const { return keymod == modifiers && IsPressed(key); }

		bool CheckKeyPressed(std::vector<std::string> keys);
		int CountKeysPressed(std::vector<std::string> keys);

		static const char* KeyToHTML5Name(int c);
		static const char* SpecialKeyToHTML5Name(int key);
		static int HTML5NameToKey(const std::string& key);
		static int GetKeyboardModifiers();
		static int SetKeyboardModifiers(bool shiftKey, bool ctrlKey, bool altKey, bool metaKey, bool capsLock, bool numLock);
	private:
		static int vf_keymod;
	};
} // namespace Vf

#endif
