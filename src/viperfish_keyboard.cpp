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
#include "stdafx.h"
#include <viperfish_keyboard.hpp>


namespace Viperfish
{
	void KeyboardState::SetKey(unsigned char c, int keymod, bool pressed)
	{
		if (keymod & ShiftKeyBit)
		{
			c = toupper(c);
		}
		else
		{
			c = tolower(c);
		}
		const char s[2] = { (char)c, '\0' };
		string key = s;
		SetKey(key, pressed);
	}


	bool KeyboardState::CheckKeyPressed(vector<string> keys)
	{
		for (auto & key : keys)
		{
			if (IsPressed(key))
				return true;
		}
		return false;
	}


	int KeyboardState::CountKeysPressed(vector<string> keys)
	{
		int count = 0;
		for (auto key : keys)
		{
			if (IsPressed(key))
				count++;
		}
		return count;
	}
}