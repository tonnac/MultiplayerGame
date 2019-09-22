#pragma once
#include <Windows.h>
#include <string>

#define SINGLE(x)						\
public:									\
x(const x& rhs) = delete;				\
x operator = (const x& rhs) = delete;

class WindowClass
{
public:
	WindowClass(HINSTANCE hInst) : mhAppInst(hInst) 
	{
		if (mWindow == nullptr)
		{
			mWindow = this;
		}
	}

public:
	virtual void Initialize();
	int Run();

private:
	bool InitMainWindow();
	virtual void GameRun() {};
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
#pragma region WindowProperty
	HWND mhMainWnd = nullptr;
	HINSTANCE mhAppInst = nullptr;
	bool mMinimized = false;
	bool mMaximized = false;
	bool mResizing = false;
	bool mFullscreenState = false;

	int mClientWidth = 1024;
	int mCliendHeight = 768;
	
	std::wstring mMainWndCaption = L"d3d App";
	static WindowClass* mWindow;

	static WindowClass* GetApp()
	{
		return mWindow;
	}

	friend LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	SINGLE(WindowClass)
#pragma endregion


};