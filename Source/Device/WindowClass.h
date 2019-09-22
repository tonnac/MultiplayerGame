#pragma once

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

	inline void GetWindowSize(int& inWidth, int& inHeight)
	{
		inWidth = mClientWidth;
		inHeight = mCliendHeight;
	}

protected:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND GetWindowHandle() const
	{
		return mhMainWnd;
	}

private:
	bool InitMainWindow();
	virtual void GameRun() {};

	void CenterWindow(HWND hwnd, const RECT& clientRect);

#pragma region WindowProperty
protected:
	int mClientWidth = 1024;
	int mCliendHeight = 768;

private:
	HWND mhMainWnd = nullptr;
	HINSTANCE mhAppInst = nullptr;
	
	std::wstring mMainWndCaption = L"d3dApp";
	static WindowClass* mWindow;

	static WindowClass* GetApp()
	{
		return mWindow;
	}

	friend LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	SINGLE(WindowClass)
#pragma endregion
};