#include "stdafx.h"
#include "PieChart.h"


void drawPieChart(HWND hWnd, HDC hdc, vector<FreqProgram> lfq)
{
	if (lfq.size() == 0)
	{
		MessageBox(0, L"Chưa có ứng dụng khởi chạy nào", L"Notice", MB_OK);
		return;
	}
	vector<FreqProgram> ListFreqProgram;
	ListFreqProgram = lfq;
	// Sắp xếp tần số từ lớn đến bé
	for (int i = 0; i < ListFreqProgram.size() - 1; i++)
	{
		for (int j = i + 1; j < ListFreqProgram.size(); j++)
		{
			if (ListFreqProgram[i].freq < ListFreqProgram[j].freq)
			{
				FreqProgram fp;
				fp = ListFreqProgram[i];
				ListFreqProgram[i] = ListFreqProgram[j];
				ListFreqProgram[j] = fp;
			}
		}
	}

	int countType = 7;
	int margin = 50;
	int width = 420;
	int height = 420;
	int Total = 0;

	if (ListFreqProgram.size() <= 6)
	{
		countType = ListFreqProgram.size();
	}
	// Tính tổng các lần mở chương trình
	for (int i = 0; i < ListFreqProgram.size(); i++)
	{
		Total += ListFreqProgram[i].freq;
	}
	// Chia thành 7 phần nếu size > 6
	if (ListFreqProgram.size() > 6)
	{
		for (int i = 7; i < ListFreqProgram.size(); i++)
		{
			ListFreqProgram[6].freq += ListFreqProgram[i].freq;
		}
		ListFreqProgram.resize(7);
	}
	RECT rect;
	rect.left = margin / 2;
	rect.top = margin / 2;
	rect.right = width;
	rect.bottom = height;

	PieInfo thePieInfo[7];

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // bút có nét màu
	SelectObject(hdc, pen);
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

	double xCenter = (rect.left + rect.right) / 2;
	double yCenter = (rect.top + rect.bottom) / 2;
	static const double PI = 3.1415926535;
	double radius = (rect.bottom - rect.top) / 2; // bán kính

	thePieInfo[0].color = RGB(0, 255, 0);
	thePieInfo[1].color = RGB(255, 0, 0);
	thePieInfo[2].color = RGB(0, 0, 255);
	thePieInfo[3].color = RGB(128, 128, 128);
	thePieInfo[4].color = RGB(255, 255, 0);
	thePieInfo[5].color = RGB(0, 255, 255);
	thePieInfo[6].color = RGB(255, 0, 255);

	// Tính phần trăm mỗi loại
	float totalpercent = 0;
		for (int i = 0; i < ListFreqProgram.size(); i++)
		{
			thePieInfo[i].percentage = ((ListFreqProgram[i].freq * 100.0) / Total);
			thePieInfo[i].FileName = ListFreqProgram[i].FileName;
			thePieInfo[i].percentage = (ceilf(thePieInfo[i].percentage * 100)) / 100;
			if (i == 6)
			{
				thePieInfo[6].FileName = L"Khác";
			}
			if (i == ListFreqProgram.size() - 1)
			{
				thePieInfo[i].percentage = 100 - totalpercent;
			}
			totalpercent += thePieInfo[i].percentage;
		}

		double percent = 0;
		for (int i = 0; i < countType; i++)
		{
			percent += thePieInfo[i].percentage;

			if (percent > 100)
				percent = 100;
			double angle = (percent / 100) * 360; // góc ở tâm đơn vị độ
			double x = radius * cos(angle * PI / 180);
			double y = radius * sin(angle * PI / 180);
			MoveToEx(hdc, xCenter, yCenter, NULL);
			LineTo(hdc, xCenter + x, yCenter + y);
		}
		radius = radius / 2;
		percent = 0;
		for (int i = 0; i < countType; i++)
		{
			percent += thePieInfo[i].percentage;
			if (percent > 100)
				percent = 100;
			double angle = (percent - thePieInfo[i].percentage / 2) / 100 * 360;
			double x = radius * cos(angle * PI / 180);
			double y = radius * sin(angle * PI / 180);
			HBRUSH br;
			br = CreateSolidBrush(thePieInfo[i].color);
			SelectObject(hdc, br);
			FloodFill(hdc, xCenter + x, yCenter + y, RGB(0, 0, 255)); // tham số cuối là màu biên để đổ
		}
	// Chú thích
	int xPos = 445;
	int yPos = 100;
	for (int i = 0; i < countType; i++)
	{
		HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // tạo bút màu đen
		HBRUSH hbr = CreateSolidBrush(thePieInfo[i].color); // tạo chổi màu từng loại
		SelectObject(hdc, pen);
		Rectangle(hdc, xPos, yPos, xPos + 30, yPos + 25);
		RECT rect;
		rect.left = xPos;
		rect.top = yPos;
		rect.right = xPos + 30;
		rect.bottom = yPos + 25;
		FillRect(hdc, &rect, hbr);
		HFONT hFont = CreateFont(20, 0, 0, 0, FW_LIGHT, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		HWND hNote;
		wstring NoteInfo;
		wstring wspercent = to_wstring(thePieInfo[i].percentage);
		int pos = wspercent.find(L".", 1);
		wspercent = wspercent.substr(0, pos + 3);
		NoteInfo = thePieInfo[i].FileName + L" (" + wspercent + L" %)";
		hNote = CreateWindow(L"STATIC", NoteInfo.c_str(), WS_CHILD | WS_VISIBLE, xPos + 40, yPos, 250, 30, hWnd, NULL, NULL, NULL);
		SendMessage(hNote, WM_SETFONT, WPARAM(hFont), true);
		yPos += 40;
	}
}