#pragma once

#include "Includes.h"

class Mouse
{
private:

	static inline DWORD lastLeft = 0;
	static inline DWORD lastRight = 0;
	static inline const DWORD delay = 30;

	static inline POINT lastPos;
	static inline DWORD lastMove = 0;
	static inline const DWORD delayMove = 40;

	static inline INPUT input = { 0 };

	// todo relative mouse movement
#define SENDINPUTX(x) (x * 65536 / (1920)+1)
#define SENDINPUTY(y) (y * 65536 / (1080)+1)

public:

	static void Move(const Vector2& pos)
	{
		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
		input.mi.dx = SENDINPUTX(pos.x);
		input.mi.dy = SENDINPUTY(pos.y);
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		FNtUserSendInput(1, &input, sizeof(input));
	}

	static void MouseBack(const Vector2& pos)
	{
		if (lastMove)
			return;

		POINT curMouse;
		bool getMouse = LI_FN(GetCursorPos).get()(&curMouse);
		if (!getMouse)
			return;

		Move(pos);
		lastPos = curMouse;
		lastMove = FGetTickCount();
	}

	// call in main loop
	static void Update(const DWORD& now)
	{
		if ((now - lastLeft > delay) && lastLeft)
		{
			LeftUp();
		}
		if ((now - lastRight > delay) && lastRight)
		{
			RightUp();
		}

		if ((now - lastMove > delayMove) && lastMove)
		{
			Move(lastPos);
			lastMove = 0;
		}
	}

	static void LeftDown()
	{
		if (lastLeft)
			return;

		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		FNtUserSendInput(1, &input, sizeof(input));

		lastLeft = FGetTickCount();
	}

	static void LeftUp()
	{
		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		FNtUserSendInput(1, &input, sizeof(input));

		lastLeft = 0;
	}

	static void RightDown()
	{
		if (lastRight)
			return;

		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		FNtUserSendInput(1, &input, sizeof(input));

		lastRight = FGetTickCount();
	}

	static void RightUp()
	{
		::ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		FNtUserSendInput(1, &input, sizeof(input));

		lastRight = 0;
	}
};