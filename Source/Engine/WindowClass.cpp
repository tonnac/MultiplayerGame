#include "EnginePCH.h"

WindowClass* WindowClass::mWindow = nullptr;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WindowClass::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
}

void WindowClass::Initialize()
{
	InitMainWindow();
}

bool WindowClass::InitMainWindow()
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Register Failed", 0, 0);
		return false;
	}

	RECT R = { 0, 0, mClientWidth, mCliendHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, FALSE);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(wc.lpszClassName, mMainWndCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);

	if (mhMainWnd == nullptr)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	RECT clientRect{};
	RECT WindowRect{};

	GetWindowRect(mhMainWnd, &WindowRect);
	GetClientRect(mhMainWnd, &clientRect);

	CenterWindow(mhMainWnd, clientRect);

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
	return true;
}

int WindowClass::Run()
{
	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameRun();
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT WindowClass::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void WindowClass::CenterWindow(HWND hwnd, const RECT& clientRect)
{
	int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (screenWidth - (clientRect.right - clientRect.left)) >> 1;
	int y = (screenHeight - (clientRect.bottom - clientRect.top)) >> 1;
	MoveWindow(hwnd, x, y, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, TRUE);
}