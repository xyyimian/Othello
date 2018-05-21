#pragma once

#include<windows.h>
#include"Board.h"

#include<string>

class Window {
public:
	Window(HINSTANCE);
	~Window();

	void run();


	void drawText(HDC hDC,std::wstring,RECT,UINT format = DT_LEFT);

	Board *m_board;
private:
	HWND m_hWnd;
	WNDCLASSEX wndClass;

	HFONT t_hFont;
	HFONT m_hFont;
	HBITMAP m_hBitmap;
	HBRUSH m_grayBrush;
	HBRUSH m_whiteBrush;
	HBRUSH m_blackBrush;
	HBRUSH m_greenBrush;
	HBRUSH m_lightBlueBrush;
};