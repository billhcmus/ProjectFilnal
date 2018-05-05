#pragma once
#include "stdafx.h"
HWND hWndServer;
HHOOK hHook;
extern HINSTANCE hInstance;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

// Ham khoi tao hook
BOOL SetKeyHook(HWND hWnd);
// Ham huy hook
BOOL ClearKeyHook(HWND hWnd);

void setshow(bool);