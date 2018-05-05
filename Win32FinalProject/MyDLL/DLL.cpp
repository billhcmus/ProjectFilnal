#include "stdafx.h"

#include "DLL.h"
static bool isShow = false;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
			PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
			if ((GetAsyncKeyState(VK_CONTROL) < 0) && pKey->vkCode == 32)
			{
				isShow = !isShow;
				if (isShow == true)
				{
					ShowWindow(hWndServer, SW_SHOWDEFAULT);
				}
				else
				{
					ShowWindow(hWndServer, SW_HIDE);
				}
			}
			break;
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

// Ham khoi tao hook
BOOL SetKeyHook(HWND hWnd)
{
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, hInstance, 0);

	if (hHook != NULL)
	{
		hWndServer = hWnd;
	}
	return TRUE;
}
// Ham huy hook
BOOL ClearKeyHook(HWND hWnd)
{
	if (hWnd != hWndServer)
	{
		return FALSE;
	}
	if (UnhookWindowsHookEx(hHook))
	{
		hWndServer = NULL;
		return TRUE;
	}
	return FALSE;
}

void setshow(bool isshow)
{
	isShow = isshow;
}