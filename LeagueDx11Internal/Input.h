#pragma once
#include <Windows.h>

class Input
{
private:

public:

	enum mouseState
	{
		IDLE,
		PRESSED,
		HELD
	};

	struct MouseInfo
	{
		mouseState stateLeft = IDLE;
		mouseState stateRight = IDLE;
		int scroll = 0;
		int pos[2] = { 0, 0 };
	};

	inline static MouseInfo mouse;

	static bool HandleMouse(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_MOUSEMOVE:
			mouse.pos[0] = ((int)(short)LOWORD(lp));
			mouse.pos[1] = ((int)(short)HIWORD(lp));
			return true;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			if (mouse.stateLeft == mouseState::IDLE)
				mouse.stateLeft = mouseState::PRESSED;
			return true;
		case WM_LBUTTONUP:
			mouse.stateLeft = mouseState::IDLE;
			return true;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			if (mouse.stateRight == mouseState::IDLE)
				mouse.stateRight = mouseState::PRESSED;
			return true;
		case WM_RBUTTONUP:
			mouse.stateRight = mouseState::IDLE;
			return true;

		default:
			break;
		}

		if (mouse.stateLeft == mouseState::PRESSED)
			mouse.stateLeft = mouseState::HELD;

		if (mouse.stateRight == mouseState::PRESSED)
			mouse.stateRight = mouseState::HELD;

		return msg == WM_MOUSEMOVE || msg == WM_NCMOUSEMOVE;
	}
};