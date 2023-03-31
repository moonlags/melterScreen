#include <Windows.h>
#include <iostream>

using namespace std;

int ScreenWidth, ScreenHeight;
int Interval = 100;
int i=100;

LRESULT CALLBACK Melter(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
	{
		HDC Desktop = GetDC(HWND_DESKTOP);
		HDC Window = GetDC(hWnd);

		BitBlt(Window, 0, 0, ScreenWidth, ScreenHeight, Desktop, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, Window);
		ReleaseDC(HWND_DESKTOP, Desktop);

		SetTimer(hWnd, 0, Interval, nullptr);
		ShowWindow(hWnd, SW_SHOW);
		break;
	}
	case WM_PAINT:
	{
		ValidateRect(hWnd, nullptr);
		break;
	}
	case WM_TIMER:
	{
		cout << i << endl;
		HDC Window = GetDC(hWnd);
		int X = (rand() % ScreenWidth) - (150 / 2),
			Y = (rand() % 15),
			Width = (rand() % 150);
		BitBlt(Window, X, Y, Width, ScreenHeight, Window, X, 0, SRCCOPY);
		BitBlt(Window, X, 0, Width, Y, Window, 0, 0, BLACKNESS);
		ReleaseDC(hWnd, Window);
		i += 100;
		if (i == 20000)
		{
			PostQuitMessage(0);
		}
		break;
	}
	case WM_DESTROY:
	{
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
		return 0;
	case WM_SYSCOMMAND:
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
{
	auto m_hInstance=GetModuleHandle(nullptr);
		// Get the width & height of current display
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	const wchar_t* CLASS_NAME = L"Melter";
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.lpfnWndProc = Melter; //Windows Procedure
	wndClass.hCursor = LoadCursorW(0, IDC_ARROW);

	if (RegisterClass(&wndClass))
	{
		// Create the "melter" overlapping window.
		HWND hWnd = CreateWindowExA(WS_EX_TOPMOST, "Melter", 0, WS_POPUP,
			0, 0, ScreenWidth, ScreenHeight, HWND_DESKTOP, 0, m_hInstance, 0);
		if (hWnd)
		{
			// seed for randomization
			srand(GetTickCount());
			MSG Msg = {};
			// Run the melter loop
			while (Msg.message != WM_QUIT)
			{
				if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
		}
	}
	return 0;
}
