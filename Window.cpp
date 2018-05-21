#include"Window.h"
static Window *window = nullptr;

VOID CALLBACK TimerProc(
	HWND hwnd,
	UINT uMsg,
	UINT idEvent,
	DWORD dwTime
)
{
	switch (uMsg)
	{
	case WM_TIMER:
		window->m_board->robot();
		window->m_board->setAvailable();
		break;
	default:
		break;
	}
	KillTimer(hwnd, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_LBUTTONDOWN:
		{
			auto Pos = MAKEPOINTS(lParam);
			int xPos = Pos.x;
			int yPos = Pos.y;
			if (window->m_board->isnoAvailable && (xPos >= 375 && xPos <= 425) && (yPos >= 450 && yPos < 500)) {	//confirm button
				window->m_board->robot();
			}
			if ((xPos>=25 && xPos<=425) && (yPos>=25 && yPos<=425) ) {
				// in rect
				xPos -= 25;
				xPos /= 50;
				yPos -= 25;
				yPos /= 50;
				if (window->m_board->getBoard()[yPos][xPos] != BlockStatus::AVAILABLE) {
					return 0;
				}
				window->m_board->mouseClick(yPos, xPos);

				SetTimer(hWnd, 0, 100, (TIMERPROC)TimerProc);
			//	window->m_board->robot();
			//	window->m_board->setAvailable();
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			auto Pos = MAKEPOINTS(lParam);
			int xPos = Pos.x;
			int yPos = Pos.y;
			if ((xPos >= 25 && xPos <= 425) && (yPos >= 25 && yPos <= 425)) {
				// in rect
				xPos -= 25;
				xPos /= 50;
				yPos -= 25;
				yPos /= 50;
				if (window->m_board->getBoard()[yPos][xPos] == BlockStatus::EMPTY || window->m_board->getBoard()[yPos][xPos] ==BlockStatus::AVAILABLE) {
					HCURSOR cur = LoadCursor(NULL, IDC_HAND);
					SetCursor(cur);
				}
			}
			else {
				HCURSOR cur = LoadCursor(NULL, IDC_ARROW);
				SetCursor(cur);
			}
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_CHAR:
		if (wParam == 'R' || wParam == 'r') {
			window->m_board->restart();
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


Window::Window(HINSTANCE hInstance) {

	// Firstly, Window

	wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"Othello";
	RegisterClassEx(&wndClass);

	m_hWnd = CreateWindow(
		L"Othello",
		L"Othello",
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);


	// Secondly, Font
	t_hFont = CreateFont(
		20, // logical height of font height
		0, // logical average character width
		0, // angle of escapement
		0, // base-line orientation angle
		FW_LIGHT, // font weight
		0, // italic attribute flag
		0, // underline attribute flag
		0, // strikeout attribute flag
		DEFAULT_CHARSET, // character set identifier
		OUT_DEFAULT_PRECIS, // output precision
		CLIP_CHARACTER_PRECIS, // clipping precision
		DEFAULT_QUALITY, // output quality
		DEFAULT_PITCH | FF_DONTCARE, // pitch and family
		L"¿¬Ìå" // pointer to typeface name string
	);

	m_hFont = CreateFont(
		50, // logical height of font height
		0, // logical average character width
		0, // angle of escapement
		0, // base-line orientation angle
		FW_LIGHT, // font weight
		0, // italic attribute flag
		0, // underline attribute flag
		0, // strikeout attribute flag
		DEFAULT_CHARSET, // character set identifier
		OUT_DEFAULT_PRECIS, // output precision
		CLIP_CHARACTER_PRECIS, // clipping precision
		DEFAULT_QUALITY, // output quality
		DEFAULT_PITCH | FF_DONTCARE, // pitch and family
		L"Î¢ÈíÑÅºÚ" // pointer to typeface name string
	);

	m_whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	m_blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	m_grayBrush = CreateSolidBrush(RGB(128, 128, 128));
	m_greenBrush = CreateSolidBrush(RGB(0, 255, 00));
	m_lightBlueBrush = CreateSolidBrush(RGB(0xBE, 0xEF, 0xFF));

	m_board = new Board;
}
Window::~Window() {
	delete m_board;
	DeleteObject(m_hFont);
	DeleteObject(t_hFont);
	DeleteObject(m_blackBrush);
	DeleteObject(m_whiteBrush);
	DeleteObject(m_grayBrush);
	DeleteObject(m_lightBlueBrush);
	DeleteObject(m_greenBrush);
	UnregisterClass(L"Othello", wndClass.hInstance);
}
void Window::drawText(HDC hDC,std::wstring str, RECT rc,UINT format) {
	DrawText(hDC, str.c_str(), str.length(), &rc, format);
}

void Window::run() {

	DWORD nowTime,prevRenderTime = GetTickCount();


	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			nowTime = GetTickCount();
			if (nowTime - prevRenderTime >= 100) {
				prevRenderTime = nowTime;
				

				// Get DC and create Buffer
				HDC wDC = GetDC(m_hWnd);
				HDC tDC = CreateCompatibleDC(wDC);
				HBITMAP bufferBMP = CreateCompatibleBitmap(wDC, 800, 600);
				RECT rc = { 0,0,800,600 };
				SelectObject(tDC, bufferBMP);
				FillRect(tDC, &rc, m_grayBrush);


				// Start rendering
				SelectObject(tDC, m_hFont);
				SetBkMode(tDC, TRANSPARENT);
				for (int i = 0; i < 9; ++i) {
					MoveToEx(tDC, i * 50 + 25, 25, NULL);
					LineTo(tDC, i * 50 + 25, 425);
				}
				for (int i = 0; i < 9; ++i) {
					MoveToEx(tDC, 25, i * 50 + 25, NULL);
					LineTo(tDC, 425, i * 50 + 25);
				}

				auto board = m_board->getBoard();
				int blackCount = 0, whiteCount = 0;

				SelectObject(tDC, m_blackBrush);
				for (int i = 0; i < 8; ++i) {
					for (int j = 0; j < 8; ++j) {
						if (board[i][j] == BlockStatus::BLACK) {
							Ellipse(tDC, j * 50 + 25, i * 50 + 25, j * 50 + 75, i * 50 + 75);
							++blackCount;
						}
					}
				}
				SelectObject(tDC, m_whiteBrush);
				for (int i = 0; i < 8; ++i) {
					for (int j = 0; j < 8; ++j) {
						if (board[i][j] == BlockStatus::WHITE) {
							Ellipse(tDC, j * 50 + 25, i * 50 + 25, j * 50 + 75, i * 50 + 75);
							++whiteCount;
						}
					}
				}
				SelectObject(tDC, m_lightBlueBrush);
				for (int i = 0; i < 8; ++i) {
					for (int j = 0; j < 8; ++j) {
						if (board[i][j] == BlockStatus::AVAILABLE) {
							Ellipse(tDC, j * 50 + 40, i * 50 + 40, j * 50 + 60, i * 50 + 60);
						}
					}
				}
				
				SetTextColor(tDC, RGB(0, 0, 0));
				drawText(tDC, std::to_wstring(blackCount),{490,25,545,75}, DT_CENTER);
				SetTextColor(tDC, RGB(0, 255, 0));
				drawText(tDC, L":", { 545,25,555,75 }, DT_CENTER);
				SetTextColor(tDC, RGB(255, 255, 255));
				drawText(tDC, std::to_wstring(whiteCount), { 555,25,610,75 }, DT_CENTER);

				if (m_board->isGameOver) {
					SetTextColor(tDC, RGB(255, 0, 0));
					drawText(tDC, L"Game Over!\nPress R to restart!", { 425,100,800,200 }, DT_CENTER);
					if (m_board->isFull && whiteCount < blackCount || whiteCount == 0) {
						SetTextColor(tDC, RGB(0, 0, 0));
						drawText(tDC, L"Black Win!", { 500,200,700,300 }, DT_CENTER);
					}
					else if ( m_board->isFull && whiteCount == blackCount ) {
						SetTextColor(tDC, RGB(80, 80, 80));
						drawText(tDC, L"Draw!", { 500,200,700,300 }, DT_CENTER);
					}
					else if((m_board->isFull && whiteCount > blackCount) || blackCount == 0){
						SetTextColor(tDC, RGB(255, 255, 255));
						drawText(tDC, L"White Win!", { 500,200,700,300 }, DT_CENTER);
					}
				}

				else if (m_board->isnoAvailable) {
					SelectObject(tDC, t_hFont);
					SetTextColor(tDC, RGB(255, 255, 255));
					drawText(tDC, L"AI GO AGAIN: Confirm", { 200,450,425,500 }, DT_CENTER);
				}
				else {
					drawText(tDC, L"", { 200,450,425,500 }, DT_CENTER);
				}

				// Blit buffer to screen
				BitBlt(wDC, 0, 0,
					800, 600, tDC, 0, 0, SRCCOPY);
				DeleteObject(tDC);
				DeleteObject(bufferBMP);
				ReleaseDC(m_hWnd, wDC);
			}
		}
	}

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
	window = new Window(hInstance);
	window->run();
	delete window;
	return 0;
}