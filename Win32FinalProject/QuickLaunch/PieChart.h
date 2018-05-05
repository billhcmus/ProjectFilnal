#pragma once
#include "stdafx.h"
#include "Program.h"
// Cấu trúc cho biểu đồ
struct PieInfo
{
	wstring FileName;
	float percentage;
	COLORREF color; // màu của 1 phần
	PieInfo() {} // khởi tạo mặc định
	PieInfo(COLORREF c)
	{
		this->color = c;
	}
};

void drawPieChart(HWND hWnd, HDC hdc, vector<FreqProgram> ListFreqProgram);
