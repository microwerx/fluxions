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
#ifndef VIPERFISH_KEYBOARD_HPP
#define VIPERFISH_KEYBOARD_HPP

#include <map>
#include <vector>

#define VF_SHIFT_MODIFIER		0x0001
#define VF_CTRL_MODIFIER		0x0002
#define VF_ALT_MODIFIER			0x0004
#define VF_META_MODIFIER		0x0008
#define VF_CAPSLOCK_MODIFIER	0x0010
#define VF_NUMLOCK_MODIFIER		0x0020

#define VF_KEY_F1                       0x0101
#define VF_KEY_F2                       0x0102
#define VF_KEY_F3                       0x0103
#define VF_KEY_F4                       0x0104
#define VF_KEY_F5                       0x0105
#define VF_KEY_F6                       0x0106
#define VF_KEY_F7                       0x0107
#define VF_KEY_F8                       0x0108
#define VF_KEY_F9                       0x0109
#define VF_KEY_F10                      0x010A
#define VF_KEY_F11                      0x010B
#define VF_KEY_F12                      0x010C
#define VF_KEY_LEFT                     0x0164
#define VF_KEY_UP                       0x0165
#define VF_KEY_RIGHT                    0x0166
#define VF_KEY_DOWN                     0x0167
#define VF_KEY_PAGE_UP                  0x0168
#define VF_KEY_PAGE_DOWN                0x0169
#define VF_KEY_HOME                     0x016A
#define VF_KEY_END                      0x016B
#define VF_KEY_INSERT                   0x016C
#define VF_KEY_NUM_LOCK					0x016D
#define VF_KEY_BEGIN					0x016E
#define VF_KEY_DELETE					0x016F
#define VF_KEY_SHIFT_L					0x0170
#define VF_KEY_SHIFT_R					0x0171
#define VF_KEY_CTRL_L					0x0172
#define VF_KEY_CTRL_R					0x0173
#define VF_KEY_ALT_L					0x0174
#define VF_KEY_ALT_R					0x0175
#define VF_KEY_META_L					0x0176
#define VF_KEY_META_R					0x0177
#define VF_KEY_SUPER_L					0x0176
#define VF_KEY_SUPER_R					0x0177
#define VF_KEY_MENU						0x0178
#define VF_KEY_KP_0						0x140
#define VF_KEY_KP_1						0x141
#define VF_KEY_KP_2						0x142
#define VF_KEY_KP_3						0x143
#define VF_KEY_KP_4						0x144
#define VF_KEY_KP_5						0x145
#define VF_KEY_KP_6						0x146
#define VF_KEY_KP_7						0x147
#define VF_KEY_KP_8						0x148
#define VF_KEY_KP_9						0x149
#define VF_KEY_KP_DECIMAL				0x150
#define VF_KEY_KP_DIVIDE				0x151
#define VF_KEY_KP_MULTIPLY				0x152
#define VF_KEY_KP_SUBTRACT				0x153
#define VF_KEY_KP_ADD					0x154
#define VF_KEY_KP_ENTER					0x155
#define VF_KEY_KP_EQUAL					0x156

namespace Viperfish
{

	struct KeyboardState
	{
		//map<int, bool> keys;
		//map<int, bool> modkeys;
		std::map<std::string, int> keys;
		int modifiers;

		static const int ShiftKeyBit = 1;
		static const int CtrlKeyBit = 2;
		static const int AltKeyBit = 4;
		static const int ShiftCtrlBits = 3;
		static const int ShiftAltBits = 5;
		static const int CtrlAltBits = 6;
		static const int ShiftCtrlAltBits = 7;

		inline bool ctrlKey() const { return modifiers & CtrlKeyBit; }
		inline bool altKey() const { return modifiers & AltKeyBit; }
		inline bool shiftKey() const { return modifiers & ShiftKeyBit; }
		inline bool ctrlAlt() const { return modifiers & (CtrlKeyBit | AltKeyBit); }
		inline bool shiftAlt() const { return modifiers & (ShiftKeyBit | AltKeyBit); }
		inline bool shiftCtrlAlt() const { return modifiers & (ShiftKeyBit | CtrlKeyBit | AltKeyBit); }

		inline void Clear()
		{
			keys.clear();
			modifiers = 0;
		}
		inline void Reset()
		{
			modifiers = 0;
			for (auto &key : keys) {
				key.second = false;
			}
		}
		inline void SetKey(const std::string &key, bool state) { keys[key] = state; }
		inline void SetKey(const std::string &key, int keymod, bool state)
		{
			modifiers = keymod;
			keys[key] = state;
		}
		void SetKey(unsigned char c, int keymod, bool state);
		inline bool IsPressed(const std::string &key) const
		{
			std::map<std::string, int>::const_iterator it = keys.find(key);
			if (it != keys.end())
				return it->second;
			return false;
		}
		inline bool IsPressed(const std::string &key, int keymod) const { return keymod == modifiers && IsPressed(key); }
		bool CheckKeyPressed(std::vector<std::string> keys);
		int CountKeysPressed(std::vector<std::string> keys);
	};

	int GetKeyboardModifiers();
	int SetKeyboardModifiers(bool shiftKey, bool ctrlKey, bool altKey, bool metaKey, bool capsLock, bool numLock);

	const char *KeyToHTML5Name(char c);
	const char *SpecialKeyToHTML5Name(int key);
	int HTML5NameToKey(const std::string &key);

} // namespace Viperfish

#endif
