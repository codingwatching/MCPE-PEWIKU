#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include <vector>

/** A keyboard action; key-down or key-up */
class KeyboardAction
{
public:
	static const int KEYUP = 0;
	static const int KEYDOWN = 1;

	KeyboardAction(unsigned char keyCode, int state) {
		this->key = keyCode;
		this->state = state;
	}

	int state;
	unsigned char key;
};

/* Iterators */
typedef std::vector<KeyboardAction> KeyboardActionVec;
typedef KeyboardActionVec::iterator KeyboardActionIt;
typedef KeyboardActionVec::const_iterator KeyboardActionCIt;

/** A static keyboard class, written to resemble the one in lwjgl somewhat */
class Keyboard
{
public:
	static const int KEY_A = 97;
	static const int KEY_B = 98;
	static const int KEY_C = 99;
	static const int KEY_D = 100;
	static const int KEY_E = 101;
	static const int KEY_F = 102;
	static const int KEY_G = 103;
	static const int KEY_H = 104;
	static const int KEY_I = 105;
	static const int KEY_J = 106;
	static const int KEY_K = 107;
	static const int KEY_L = 108;
	static const int KEY_M = 109;
	static const int KEY_N = 110;
	static const int KEY_O = 111;
	static const int KEY_P = 112;
	static const int KEY_Q = 113;
	static const int KEY_R = 114;
	static const int KEY_S = 115;
	static const int KEY_T = 116;
	static const int KEY_U = 117;
	static const int KEY_V = 118;
	static const int KEY_W = 119;
	static const int KEY_X = 120;
	static const int KEY_Y = 121;
	static const int KEY_Z = 122;

	static const int KEY_BACKSPACE = 8;
	static const int KEY_RETURN = 13;

	static const int KEY_F1 = 128;
	static const int KEY_F2 = 129;
	static const int KEY_F3 = 130;
	static const int KEY_F4 = 131;
	static const int KEY_F5 = 132;
	static const int KEY_F6 = 133;
	static const int KEY_F7 = 134;
	static const int KEY_F8 = 135;
	static const int KEY_F9 = 136;
	static const int KEY_F10 = 137;
	static const int KEY_F11 = 138;
	static const int KEY_F12 = 139;

	static const int KEY_ESCAPE = 27;
	static const int KEY_SPACE = 32;
	static const int KEY_LCTRL  = 17;
	static const int KEY_LSHIFT = 10;

	static bool isKeyDown(int keyCode) {
		if (keyCode < 0 || keyCode >= 256) {
			return false;
		}
		return _states[keyCode] == KeyboardAction::KEYDOWN;
	}

	static void reset() {
        _inputs.clear();
		_inputText.clear();
        _index = -1;
		_textIndex = -1;
    }

	static void feed(unsigned char keyCode, int state) {
		_inputs.push_back(KeyboardAction(keyCode, state));

		_states[keyCode] = state;
	}
	static void feedText(char character) {
		_inputText.push_back(character);
	}

	static bool next() {
        if (_index + 1 >= (int)_inputs.size())
            return false;

        ++_index;
        return true;
    }

	static bool nextTextChar() {
		if(_textIndex + 1 >= (int)_inputText.size())
			return false;
		++_textIndex;
		return true;
	}

	static void rewind() {
		_index = -1;
		_textIndex = -1;
	}

    static int getEventKey() {
        return _inputs[_index].key;
    }

    static int getEventKeyState() {
        return _inputs[_index].state;
    }
	static char getChar() {
		return _inputText[_textIndex];
	}
private:
    static int _index;
	static int _textIndex;
	static int _states[256];
	static std::vector<KeyboardAction> _inputs;
	static std::vector<char> _inputText;
	static bool _inited;
};

#endif//KEYBOARD_H__
