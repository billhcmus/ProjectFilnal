
#include "stdafx.h"
#include "QuickLaunch.h"
#include <windowsx.h>
#include <tchar.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <codecvt>
#include "Search.h"
#include "PieChart.h"
#include <commctrl.h>
#include <commoncontrols.h>
#include <codecvt>
#include <fstream>
#include "Commdlg.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define _CRT_SECURE_NO_WARNINGS
#define MAX_LOADSTRING 100

#define TRAYICONID 1
#define SWM_TRAYMSG WM_APP

#define SWM_SCAN WM_APP + 1
#define SWM_VIEWSTATISTICS WM_APP + 2
#define SWM_EXIT WM_APP + 3

using namespace Gdiplus;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
NOTIFYICONDATA nid; // for notification
HWND g_hwnd;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUNCH));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			//if (!IsDialogMessage(g_hwnd, &msg))
			//{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			//}
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKLAUNCH);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 600, 700, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

HINSTANCE hDLL;
typedef BOOL(*Proc)(HWND);
typedef BOOL(*SetProc)(bool);
HWND hTextBox;
HWND hListView;
HWND hDialog;
Proc InstallHookProc;
Proc UninstallHookProc;
SetProc SetShowProc;
vector<Program> gListProg;
vector<Program> gCandidateProg;
WCHAR keyword[255]; // tu khoa tim kiem
HFONT hFont;
vector<FreqProgram> gListFreq;
int curSelItem = -1;
HWND hLabelName, hLabelPath;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusStartupToken;
HIMAGELIST hImg;

// Function Prototype
ULONGLONG GetDllVersion(LPCTSTR lpszDllName);
void ShowContextMenu(HWND hWnd);
void ScanProgramFileFolder(LPCWSTR path, int);
void InitBasicView(HWND hWnd);
void doUninstallHook(HWND hWnd);
void doInstallHook(HWND hWnd);
HWND CreateListView(long lExtStyle, HWND hWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
void LoadDataToListView(vector<Program> ListProg);
HICON GetIcon(LPTSTR pszPath); // Ham lay icon he thong
void SearchProgram(WCHAR keyword[]);
INT_PTR CALLBACK ScanProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DrawPieChartProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void LoadIconToListView(vector<Program> &ListProg);
void LoadInstalledApp();
wstring QueryValue(HKEY hKey, LPCTSTR szName);
bool isExecFile(TCHAR* fileName);
void ExecProgram(int id);
void WriteFreqOpenFile();
void ReadFreqOpenFile();
void SaveOpenedProgram(Program p, vector<FreqProgram> &lfp);
void UpdateIndexAndFreqProgramToDB(vector<Program> &ListProg);
TCHAR* GetFolder(HWND hWnd);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		g_hwnd = hWnd;
		InitCommonControls();
		GdiplusStartup(&gdiplusStartupToken, &gdiplusStartupInput, NULL);
		doInstallHook(hWnd);
		// Load nhung file co ban o 3 lop folder
		ScanProgramFileFolder(L"C:\\Program Files", 0);
		ScanProgramFileFolder(L"C:\\Program Files (x86)", 0);
		ReadFreqOpenFile();
		UpdateIndexAndFreqProgramToDB(gListProg);
		InitBasicView(hWnd);
		LoadIconToListView(gListProg);
		LoadDataToListView(gListProg);
		gCandidateProg = gListProg;
		break;
	case SWM_TRAYMSG:
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
			break;
		}
	}
	break;
	case WM_CTLCOLORSTATIC:
	{
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		if ((HWND)lParam == hTextBox)
		{
			if (HIWORD(wParam) == EN_CHANGE) // co su thay doi o text box
			{
				gCandidateProg.clear();
				GetWindowText(hTextBox, keyword, 255);
				SearchProgram(keyword);
				LoadDataToListView(gCandidateProg);
			}
		}
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			WriteFreqOpenFile();
			DestroyWindow(hWnd);
			break;
		case SWM_EXIT:
			WriteFreqOpenFile();
			DestroyWindow(hWnd);
			break;
		case SWM_SCAN:
			SetWindowText(hLabelName, L"");
			SetWindowText(hLabelPath, L"");
			hImg = NULL;
			InvalidateRect(hWnd, NULL, true);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROLS_DLG), hWnd, ScanProc);
			break;
		case SWM_VIEWSTATISTICS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PIECHART), hWnd, DrawPieChartProc);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_NOTIFY:
		NMHDR * notifyMessage;
		LPNMLISTVIEW lpnmList;
		notifyMessage = (NMHDR*)lParam;
		lpnmList = (LPNMLISTVIEW)notifyMessage;
		switch (notifyMessage->code)
		{
		case NM_DBLCLK:
			if (notifyMessage->hwndFrom == hListView)
			{
				int index;
				index = ListView_GetSelectionMark(hListView);
				ExecProgram(index);
			}
			break;
		case NM_RETURN:
			if (notifyMessage->hwndFrom == hListView)
			{
				int index;
				index = ListView_GetSelectionMark(hListView);
				ExecProgram(index);
			}
			break;
		case NM_CLICK:
		{
			if (notifyMessage->hwndFrom == hListView)
			{
				SetWindowText(hLabelName, L"");
				SetWindowText(hLabelPath, L"");
				InvalidateRect(hWnd, NULL, true);
				curSelItem = ListView_GetNextItem(hListView, -1, LVNI_FOCUSED);
				if (curSelItem != -1)
				{
					wstring temp1, temp2;
					temp1 = L"File Name: " + gCandidateProg[curSelItem].FileName;
					SetWindowText(hLabelName, temp1.c_str());
					temp2 = L"Path: " + gCandidateProg[curSelItem].FilePath;
					SetWindowText(hLabelPath, temp2.c_str());
					HICON hIcon;

					hImg = ImageList_Create(32, 32,
						ILC_COLOR32, 1, 1);
					hIcon = GetIcon((LPTSTR)gCandidateProg[curSelItem].FilePath.c_str());
					ImageList_AddIcon(hImg, hIcon);
					InvalidateRect(hWnd, NULL, false);
				}
			}

		}
		break;
		case LVN_KEYDOWN:
		{

			if (notifyMessage->hwndFrom == hListView)
			{
				SetWindowText(hLabelName, L"");
				SetWindowText(hLabelPath, L"");
				InvalidateRect(hWnd, NULL, true);

				if (LPNMLVKEYDOWN(lParam)->wVKey == VK_UP || LPNMLVKEYDOWN(lParam)->wVKey == VK_DOWN)
				{
					curSelItem = ListView_GetNextItem(hListView, -1, LVNI_FOCUSED);
					if (LPNMLVKEYDOWN(lParam)->wVKey == VK_UP)
					{
						curSelItem -= 1;
						if (curSelItem == -1)
						{
							curSelItem = 0;
						}
					}
					else
					{
						curSelItem += 1;
						if (curSelItem == gCandidateProg.size())
						{
							curSelItem = gCandidateProg.size() - 1;
						}
					}
					if (curSelItem != -1)
					{
						wstring temp1, temp2;
						temp1 = L"File Name: " + gCandidateProg[curSelItem].FileName;
						SetWindowText(hLabelName, temp1.c_str());
						temp2 = L"Path: " + gCandidateProg[curSelItem].FilePath;
						SetWindowText(hLabelPath, temp2.c_str());
						HICON hIcon;

						hImg = ImageList_Create(32, 32,
							ILC_COLOR32, 1, 1);
						hIcon = GetIcon((LPTSTR)gCandidateProg[curSelItem].FilePath.c_str());
						ImageList_AddIcon(hImg, hIcon);
						InvalidateRect(hWnd, NULL, false);
					}
				}
				else if (LPNMLVKEYDOWN(lParam)->wVKey == VK_DELETE)
				{
					curSelItem = ListView_GetNextItem(hListView, -1, LVNI_FOCUSED);
					if (curSelItem >= 0)
					{
						ListView_DeleteItem(hListView, curSelItem);
						int index;
						index = gCandidateProg[curSelItem].index;
						gListProg.erase(gListProg.begin() + index);
						gCandidateProg.erase(gCandidateProg.begin() + curSelItem);
						UpdateIndexAndFreqProgramToDB(gListProg);
					}
				}
			}
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		ImageList_Draw(hImg, 0, hdc, 40, 35, ILD_NORMAL);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		SetShowProc(false);
		break;
	case WM_DESTROY:
		doUninstallHook(hWnd);
		GdiplusShutdown(gdiplusStartupToken);
		FreeLibrary(hDLL);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Thong diep cho dialog Scan to build database
INT_PTR CALLBACK ScanProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_HIDE);
		SendDlgItemMessage(hDlg, IDC_RADIOPROGRAM, BM_SETCHECK, 1, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_BTNCHOOSE:
		{
			wstring folderpath;
			TCHAR* buf = new TCHAR[MAX_PATH];
			buf = GetFolder(hDlg);
			SetWindowText(GetDlgItem(hDlg, IDC_FOLDERPATH), buf);
		}
		break;	
		case IDC_RADIOPROGRAM:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hDlg, IDC_BTNCHOOSE), FALSE);
				break;
			}
			break;
		case IDC_RADIOCHOOSE:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hDlg, IDC_BTNCHOOSE), TRUE);
				break;
			}
			break;
		case IDC_BTNLOAD:
			INT pb_pos;
			TCHAR folderPath[MAX_PATH];
			//gListProg.clear();
			//gCandidateProg.clear();

			ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_SHOWNORMAL);
			UpdateWindow(GetDlgItem(hDlg, IDC_PROGRESS1));
			SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETRANGE, 0, MAKELPARAM(0, 100));
			pb_pos = SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_GETPOS, 0, 0);

			if (IsDlgButtonChecked(hDlg, IDC_RADIOPROGRAM))
			{
				SetWindowText(GetDlgItem(hDlg, IDC_STATIC), L"Loading Program Files");
				ScanProgramFileFolder(L"C:\\Program Files", -1);

				pb_pos += 50;
				SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, pb_pos, 0);

				SetWindowText(GetDlgItem(hDlg, IDC_STATIC), L"Loading Program Files (x86)");
				ScanProgramFileFolder(L"C:\\Program Files (x86)", -1);

				pb_pos += 35;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIOCHOOSE))
			{
				GetWindowText(GetDlgItem(hDlg, IDC_FOLDERPATH), folderPath, MAX_PATH);
				if (wcslen(folderPath) != 0)
				{
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC), L"Loading File...");
					ScanProgramFileFolder(folderPath, -1);
					pb_pos += 85;
				}
				else
				{

				}
			}

			SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, pb_pos, 0);

			SetWindowText(GetDlgItem(hDlg, IDC_STATIC), L"Loading Icon...");
			//LoadInstalledApp();
			LoadIconToListView(gListProg);

			// Sau khi load hết thì cập nhật lại tần suất mở chương trình
			UpdateIndexAndFreqProgramToDB(gListProg);
			gCandidateProg = gListProg; // 1 bản tạm thời để xuất ra list view
			LoadDataToListView(gListProg);

			pb_pos += 15;
			SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, pb_pos, 0);

			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}

// Vẽ biểu đồ
INT_PTR CALLBACK DrawPieChartProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		InvalidateRect(hDlg, NULL, TRUE);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		// TODO: Add any drawing code that uses hdc here...

		drawPieChart(hDlg, hdc, gListFreq);

		EndPaint(hDlg, &ps);
	}
	break;
	}
	return (INT_PTR)FALSE;
}

// Get dll version number
ULONGLONG GetDllVersion(LPCTSTR lpszDllName)
{
	ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
	hinstDll = LoadLibrary(lpszDllName);
	if (hinstDll)
	{

		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if (SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion, 0, 0);
		}
		FreeLibrary(hinstDll);
	}
	return ullVersion;
}


void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SCAN, L"Scan to build database");
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_VIEWSTATISTICS, L"View Statistics");
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, L"Exit");

		SetForegroundWindow(hWnd);
		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

// Khoi tao mot so chuc nang giao dien
void InitBasicView(HWND hWnd)
{
	// Show icon in notification area
	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	if (ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
	{
		nid.cbSize = sizeof(NOTIFYICONDATA);
	}
	else
	{
		nid.cbSize = NOTIFYICONDATAA_V2_SIZE;
	}

	// ID
	nid.uID = TRAYICONID;

	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// load icon
	nid.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_QUICKLAUNCH),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	nid.hWnd = hWnd;
	nid.uCallbackMessage = SWM_TRAYMSG; // de gui thong diep cho cua so chinh

	lstrcpyn(nid.szTip, _T("Quick Launch"), sizeof(nid.szTip) / sizeof(TCHAR));

	Shell_NotifyIcon(NIM_ADD, &nid);

	nid.uVersion = NOTIFYICON_VERSION;
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
	// display the balloon
	nid.uFlags = NIF_INFO;
	_tcscpy(nid.szInfo, _T("Press Ctrl + Space to open"));
	_tcscpy(nid.szInfoTitle, _T("Quick Launch"));
	nid.uTimeout = 10000;
	nid.dwInfoFlags = NIF_INFO;
	Shell_NotifyIcon(NIM_MODIFY, &nid);
	Shell_NotifyIcon(NIM_ADD, &nid);
	if (nid.hIcon && DestroyIcon(nid.hIcon))
	{
		nid.hIcon = NULL;
	}

	hFont = CreateFont(20, 0, 0, 0, FW_LIGHT, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	// List view
	hListView = CreateListView(WS_EX_CLIENTEDGE, hWnd, ID_LISTVIEW, hInst, 40, 170, 500, 450, LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	SendMessage(hListView, WM_SETFONT, WPARAM(hFont), true);
	// TextBox
	hTextBox = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 40, 120, 500, 30, hWnd, NULL, hInst, NULL);
	SendMessage(hTextBox, WM_SETFONT, WPARAM(hFont), true);
	//
	hLabelName = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 80, 35, 500, 30, hWnd, NULL, hInst, NULL);
	hLabelPath = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 80, 50, 500, 30, hWnd, NULL, hInst, NULL);
	hFont = CreateFont(16, 0, 0, 0, FW_LIGHT, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	SendMessage(hLabelName, WM_SETFONT, WPARAM(hFont), true);
	SendMessage(hLabelPath, WM_SETFONT, WPARAM(hFont), true);
}

bool isExecFile(TCHAR* fileName)
{
	TCHAR* extension = new TCHAR[5];
	fileName = _wcsrev(fileName);
	for (int i = 0; i < 4; i++)
	{
		extension[i] = fileName[i];
	}
	extension[4] = L'\0';
	fileName = _wcsrev(fileName);
	if (StrCmp(extension, _T("exe.")) == 0)
	{
		delete[] extension;
		return true;
	}
	delete[] extension;
	return false;
}


// Quet thu muc program file
void ScanProgramFileFolder(LPCWSTR FolderPath, int level)
{

	if (level > 2)
	{
		return;
	}
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	TCHAR* path = new TCHAR[wcslen(FolderPath) + 3];
	StrCpy(path, FolderPath);
	if (wcslen(FolderPath) == 3) // nếu là ổ đĩa
	{
		StrCat(path, _T("*"));
	}
	else
	{
		StrCat(path, _T("\\*"));
	}
	bool isFound = true;

	hFind = FindFirstFileW(path, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		isFound = false;
	}
	while (isFound == true)
	{

		TCHAR* childPath;
		if ((wcscmp(fd.cFileName, L".") != 0) && wcscmp(fd.cFileName, L"..") != 0)
		{

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				childPath = new TCHAR[wcslen(FolderPath) + wcslen(fd.cFileName) + 2];
				StrCpy(childPath, FolderPath);
				StrCat(childPath, L"\\");
				StrCat(childPath, fd.cFileName);
				if (level == -1)
				{
					ScanProgramFileFolder(childPath, -1);
				}
				else
				{
					ScanProgramFileFolder(childPath, level + 1);
				}
			}
			else
			{
				if (isExecFile(fd.cFileName))
				{
					TCHAR* filePath = new TCHAR[wcslen(FolderPath) + wcslen(fd.cFileName) + 2];
					StrCpy(filePath, FolderPath);
					StrCat(filePath, L"\\");
					StrCat(filePath, fd.cFileName);

					Program prog;
					prog.FilePath = filePath;
					prog.FileName = fd.cFileName;
					prog.InstallLocation = FolderPath;
					prog.freq = 0;
					gListProg.push_back(prog);
					delete[] filePath;
				}
			}
		}
		isFound = FindNextFileW(hFind, &fd);
	}
}


void doInstallHook(HWND hWnd)
{
	hDLL = LoadLibrary(L"MyDLL.dll");
	if (hDLL == NULL)
	{
		MessageBox(hWnd, L"Can't load MyDLL.dll", L"Error", MB_OK);
	}
	else
	{
		InstallHookProc = (Proc)GetProcAddress(hDLL, "InstallHook");
		SetShowProc = (SetProc)GetProcAddress(hDLL, "SetShow");
		InstallHookProc(hWnd);
	}
}

void doUninstallHook(HWND hWnd)
{
	hDLL = LoadLibrary(L"MyDLL.dll");
	if (hDLL == NULL)
	{
		MessageBox(hWnd, L"Can't load MyDLL.dll", L"Error", MB_OK);
	}
	else
	{
		UninstallHookProc = (Proc)GetProcAddress(hDLL, "UninstallHook");
		UninstallHookProc(hWnd);
	}
}

HWND CreateListView(long lExtStyle, HWND hWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	HWND hList = CreateWindowEx(lExtStyle, WC_LISTVIEW, L"ListView", WS_CHILD | WS_VISIBLE | WS_VSCROLL
		| WS_TABSTOP | lStyle, x, y, nWidth, nHeight, hWnd, (HMENU)ID, hParentInst, NULL);

	//Init 2 columns
	LVCOLUMN lvCol;

	//Let the LVCOLUMN know that we will set the format, header text and width of it
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 300;
	TCHAR temp[30];
	StrCpy(temp, L"Name");
	lvCol.pszText = temp;
	ListView_InsertColumn(hList, 0, &lvCol);
	return hList;
}


void LoadIconToListView(vector<Program> &ListProg)
{
	HIMAGELIST hLarge, hSmall;
	HICON hIcon;

	hLarge = ImageList_Create(32, 32,
		ILC_COLOR32, 1, 1);

	hSmall = ImageList_Create(32, 32,
		ILC_COLOR32, 1, 1);

	ListView_SetImageList(hListView, hLarge, LVSIL_NORMAL);
	ListView_SetImageList(hListView, hSmall, LVSIL_SMALL);

	for (int i = 0; i < ListProg.size(); i++)
	{
		hIcon = GetIcon((LPWSTR)ListProg[i].FilePath.c_str());
		ImageList_AddIcon(hSmall, hIcon);
		ImageList_AddIcon(hLarge, hIcon);
		ListProg[i].nIcon = i;
		DestroyIcon(hIcon);
	}
}
void LoadDataToListView(vector<Program> ListProg)
{
	ListView_DeleteAllItems(hListView);

	for (int i = 0; i < ListProg.size(); i++)
	{
		LV_ITEM lvi;

		lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

		// Cot dau tien
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPWSTR)ListProg[i].FileName.c_str();
		lvi.iImage = ListProg[i].nIcon;
		ListView_InsertItem(hListView, &lvi);
	}
}

HICON GetIcon(LPTSTR pszPath)
{
	// Get the image list index of the icon
	SHFILEINFO sfi;
	if (!SHGetFileInfo(pszPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX)) return NULL;

	// Get the jumbo image list
	IImageList *piml;
	if (FAILED(SHGetImageList(SHIL_JUMBO, IID_PPV_ARGS(&piml))))
		return NULL;

	// Extract an icon
	HICON hico;
	piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hico);

	// Clean up
	piml->Release();

	// Return the icon
	return hico;
}

void SearchProgram(WCHAR keyword[])
{
	for (int i = 0; i < gListProg.size(); i++)
	{
		int pos = -1;
		if ((pos = KMPSearch(keyword, (LPWSTR)gListProg[i].FileName.c_str())) != -1)
		{
			Program tmpProg;
			tmpProg = gListProg[i];
			tmpProg.posMatch = pos;
			gCandidateProg.push_back(tmpProg);
		}
	}

	if (gCandidateProg.size() > 0)
	{
		SortProgram(gCandidateProg);
	}
}

//
wstring QueryValue(HKEY hKey,
	LPCTSTR szName)
{
	wstring value;

	DWORD dwType;
	DWORD dwSize = 0;

	if (::RegQueryValueEx(
		hKey,                   // key handle
		szName,                 // item name
		NULL,                   // reserved
		&dwType,                // type of data stored
		NULL,                   // no data buffer
		&dwSize                 // required buffer size
	) == ERROR_SUCCESS && dwSize > 0)
	{
		value.resize(dwSize);

		::RegQueryValueEx(
			hKey,                   // key handle
			szName,                 // item name
			NULL,                   // reserved
			&dwType,                // type of data stored
			(LPBYTE)&value[0],      // data buffer
			&dwSize                 // available buffer size
		);
	}

	return value;
}

// Tham khao code trên CodeProject
void LoadInstalledApp()
{
	int a = 0;
	HKEY hKey;
	LONG ret = ::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,     // local machine hive
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), // uninstall key
		0,                      // reserved
		KEY_READ,               // desired access
		&hKey                   // handle to the open key
	);

	if (ret != ERROR_SUCCESS)
		return;

	DWORD dwIndex = 0;
	DWORD cbName = 1024;
	TCHAR szSubKeyName[1024];

	while ((ret = ::RegEnumKeyEx(
		hKey,
		dwIndex,
		szSubKeyName,
		&cbName,
		NULL,
		NULL,
		NULL,
		NULL)) != ERROR_NO_MORE_ITEMS)
	{

		if (ret == ERROR_SUCCESS)
		{
			HKEY hItem;
			if (::RegOpenKeyEx(hKey, szSubKeyName, 0, KEY_READ, &hItem) != ERROR_SUCCESS)
				continue;

			wstring name = QueryValue(hItem, _T("DisplayName"));
			wstring filepath = QueryValue(hItem, _T("DisplayIcon"));
			wstring filelocation = QueryValue(hItem, _T("InstallLocation"));
			if (!name.empty())
			{
				Program p;
				int pos = filepath.find(L',', 1);
				if (pos != -1)
				{
					filepath.insert(filepath.begin() + pos, L'\0');
				}
				p.FilePath = filepath;
				p.FileName = name;
				p.InstallLocation = filelocation;
				p.freq = 0;
				gListProg.push_back(p);
			}

			::RegCloseKey(hItem);
		}
		dwIndex++;
		cbName = 1024;
	}
	::RegCloseKey(hKey);
}


void ExecProgram(int id)
{
	wstring filePath = gCandidateProg[id].FilePath;
	if (!filePath.empty())
	{
		gListProg[gCandidateProg[id].index].freq++; // Tang frequence của program mỗi khi nó được thực thi
		SaveOpenedProgram(gListProg[gCandidateProg[id].index], gListFreq);
		ShellExecute(NULL, _T("open"), filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		MessageBox(0, L"Can't open file", L"Notice", MB_OK);
	}
}

void WriteFreqOpenFile()
{
	wofstream fileOut(L"freqOpenProgram.txt");
	for (int i = 0; i < gListProg.size(); i++)
	{
		if (gListProg[i].freq > 0)
		{
			fileOut << gListProg[i].FileName << endl;
			fileOut << gListProg[i].freq << endl;
		}
	}
	fileOut.close();
}

void ReadFreqOpenFile()
{
	wifstream fileIn(L"freqOpenProgram.txt");
	wstring buf;
	if (!fileIn)
	{
		return;
	}
	if (fileIn.is_open())
	{
		while (getline(fileIn, buf))
		{
			FreqProgram fp;
			fp.FileName = buf;
			getline(fileIn, buf);
			fp.freq = _wtoi64(buf.c_str());
			gListFreq.push_back(fp);
		}
	}
	fileIn.close();
}

void SaveOpenedProgram(Program p, vector<FreqProgram> &lfp)
{
	if (lfp.size() == 0)
	{
		FreqProgram fp;
		fp.FileName = p.FileName;
		fp.freq = p.freq;
		lfp.push_back(fp);
	}
	else
	{
		bool isexist = false;
		for (int i = 0; i < lfp.size(); i++)
		{
			if (lfp[i].FileName == p.FileName)
			{
				lfp[i].freq = p.freq;
				isexist = true;
			}
		}
		if (isexist == false)
		{
			FreqProgram fp;
			fp.FileName = p.FileName;
			fp.freq = p.freq;
			lfp.push_back(fp);
		}
	}
}
void UpdateIndexAndFreqProgramToDB(vector<Program> &ListProg)
{
	for (int i = 0; i < ListProg.size(); i++)
	{
		gListProg[i].index = i; // so thu tu cua 1 program
		for (int j = 0; j < gListFreq.size(); j++)
		{
			if (ListProg[i].FileName == gListFreq[j].FileName)
			{
				ListProg[i].freq = gListFreq[j].freq;
				break;
			}
		}
	}
}

TCHAR* GetFolder(HWND hWnd)
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = L"Chọn một folder";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = -1;

	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
	if (pIDL != NULL)
	{
		::SHGetPathFromIDList(pIDL, szDir);
		return szDir;
	}
	return NULL;
}