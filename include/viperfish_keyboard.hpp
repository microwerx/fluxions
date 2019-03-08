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
		for (auto &key : keys)
		{
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


const char *KeyToHTML5Name(char c);
const char *SpecialKeyToHTML5Name(int key);
int HTML5NameToKey(const std::string &key);

} // namespace Viperfish

#endif
