#include "viperfish_pch.hpp"
#include <viperfish_keyboard.hpp>

namespace Vf {
	int KeyboardState::vf_keymod = 0;

	int KeyboardState::GetKeyboardModifiers() {
		return vf_keymod;
	}

	int KeyboardState::SetKeyboardModifiers(bool shiftKey, bool ctrlKey, bool altKey, bool metaKey, bool capsLock, bool numLock) {
		vf_keymod = 0;
		vf_keymod |= shiftKey ? VF_SHIFT_MODIFIER : 0;
		vf_keymod |= ctrlKey ? VF_CTRL_MODIFIER : 0;
		vf_keymod |= altKey ? VF_ALT_MODIFIER : 0;
		vf_keymod |= metaKey ? VF_META_MODIFIER : 0;
		vf_keymod |= capsLock ? VF_CAPSLOCK_MODIFIER : 0;
		vf_keymod |= numLock ? VF_NUMLOCK_MODIFIER : 0;
		return vf_keymod;
	}

	void KeyboardState::SetKey(unsigned c, unsigned keymod, bool pressed) {
		if (keymod & ShiftKeyBit) {
			c = (unsigned char)toupper(c);
		}
		else {
			c = (unsigned char)tolower(c);
		}
		const char s[2] = { (char)c, '\0' };
		std::string key = s;
		SetKey(key, pressed);
	}

	bool KeyboardState::CheckKeyPressed(std::vector<std::string> keysToCheck) {
		for (auto& key : keysToCheck) {
			if (IsPressed(key))
				return true;
		}
		return false;
	}

	int KeyboardState::CountKeysPressed(std::vector<std::string> keysToCheck) {
		int count = 0;
		for (auto key : keysToCheck) {
			if (IsPressed(key))
				count++;
		}
		return count;
	}

	const char* KeyboardState::KeyToHTML5Name(int c) {
		static char s[10];

		if (c <= 0x20) {
			switch (c) {
			case 0x09:
				return "Tab";
			case 0x0D:
				return "Enter";
			case 0x1B:
				return "Escape";
			case 0x20:
				return " ";
			}
		}

		int keymod = GetKeyboardModifiers();
		if (keymod & VF_SHIFT_MODIFIER) {
			const char* lowers = "`1234567890-=[]\\;',./";
			const char* uppers = "~!@#$%^&*()_+{}|:\"<>?";
			c = (char)toupper(c);
			const char* lc = lowers;
			const char* uc = uppers;
			while (*lc != 0) {
				if (c == *lc) {
					c = *uc;
					break;
				}
				lc++;
				uc++;
			}
		}
		else {
			c = (char)tolower(c);
		}
		s[0] = (char)c;
		s[1] = 0;

		return s;
	}

	int KeyboardState::HTML5NameToKey(const std::string& key) {
		if (key == "Tab")
			return 0x09;
		if (key == "Enter")
			return 0x0D;
		if (key == "Escape")
			return 0x1B;
		if (key == " ")
			return 0x20;
		if (key.size() == 1 && key.front() <= 0x7f)
			return key.front();// tolower(key.front());
		if (key == "F1")
			return VF_KEY_F1;
		if (key == "F2")
			return VF_KEY_F2;
		if (key == "F3")
			return VF_KEY_F3;
		if (key == "F4")
			return VF_KEY_F4;
		if (key == "F5")
			return VF_KEY_F5;
		if (key == "F6")
			return VF_KEY_F6;
		if (key == "F7")
			return VF_KEY_F7;
		if (key == "F8")
			return VF_KEY_F8;
		if (key == "F9")
			return VF_KEY_F9;
		if (key == "F10")
			return VF_KEY_F10;
		if (key == "F11")
			return VF_KEY_F11;
		if (key == "F12")
			return VF_KEY_F12;
		if (key == "Left")
			return VF_KEY_LEFT;
		if (key == "Up")
			return VF_KEY_UP;
		if (key == "Right")
			return VF_KEY_RIGHT;
		if (key == "Down")
			return VF_KEY_DOWN;
		if (key == "PageUp")
			return VF_KEY_PAGE_UP;
		if (key == "PageDown")
			return VF_KEY_PAGE_DOWN;
		if (key == "Home")
			return VF_KEY_HOME;
		if (key == "End")
			return VF_KEY_END;
		if (key == "Insert")
			return VF_KEY_INSERT;
		if (key == "NumLock")
			return VF_KEY_NUM_LOCK;
		if (key == "Begin")
			return VF_KEY_BEGIN;
		if (key == "Delete")
			return VF_KEY_DELETE;
		if (key == "Shift")
			return VF_KEY_SHIFT_L;
		if (key == "Ctrl")
			return VF_KEY_CTRL_L;
		if (key == "Alt")
			return VF_KEY_ALT_L;
		return 0;
	}

	const char* KeyboardState::SpecialKeyToHTML5Name(int key) {
		if (key >= 0x100)
			key -= 0x100;

		switch (key) {
		case 0x0001: // GLUT_KEY_F1
			return "F1";
			break;
		case 0x0002: // GLUT_KEY_F2
			return "F2";
			break;
		case 0x0003: // GLUT_KEY_F3
			return "F3";
			break;
		case 0x0004: // GLUT_KEY_F4
			return "F4";
			break;
		case 0x0005: // GLUT_KEY_F5
			return "F5";
			break;
		case 0x0006: // GLUT_KEY_F6
			return "F6";
			break;
		case 0x0007: // GLUT_KEY_F7
			return "F7";
			break;
		case 0x0008: // GLUT_KEY_F8
			return "F8";
			break;
		case 0x0009: // GLUT_KEY_F9
			return "F9";
			break;
		case 0x000A: // GLUT_KEY_F10
			return "F10";
			break;
		case 0x000B: // GLUT_KEY_F11
			return "F11";
			break;
		case 0x000C: // GLUT_KEY_F12
			return "F12";
			break;
		case 0x0064: // GLUT_KEY_LEFT
			return "Left";
			break;
		case 0x0065: // GLUT_KEY_UP
			return "Up";
			break;
		case 0x0066: // GLUT_KEY_RIGHT
			return "Right";
			break;
		case 0x0067: // GLUT_KEY_DOWN
			return "Down";
			break;
		case 0x0068: // GLUT_KEY_PAGE_UP
			return "PageUp";
			break;
		case 0x0069: // GLUT_KEY_PAGE_DOWN
			return "PageDown";
			break;
		case 0x006A: // GLUT_KEY_HOME
			return "Home";
			break;
		case 0x006B: // GLUT_KEY_END
			return "End";
			break;
		case 0x006C: // GLUT_KEY_INSERT
			return "Insert";
			break;
		case 0x006D: // GLUT_KEY_NUM_LOCK
			return "NumLock";
			break;
		case 0x006E: // GLUT_KEY_BEGIN
			return "Begin";
			break;
		case 0x006F: // GLUT_KEY_DELETE
			return "Delete";
			break;
		case 0x0070: // GLUT_KEY_SHIFT_L
			return "Shift";
			break;
		case 0x0071: // GLUT_KEY_SHIFT_R
			return "Shift";
			break;
		case 0x0072: // GLUT_KEY_CTRL_L
			return "Ctrl";
			break;
		case 0x0073: // GLUT_KEY_CTRL_R
			return "Ctrl";
			break;
		case 0x0074: // GLUT_KEY_ALT_L
			return "Alt";
			break;
		case 0x0075: // GLUT_KEY_ALT_R
			return "Alt";
			break;
			// TODO CHECK THESE
		case 0x0076:
			return "Meta";
			break;
		case 0x077:
			return "Meta";
			break;
		case 0x078:
			return "Menu";
			break;
		}
		return "Undefined";
	}
} // namespace Vf
