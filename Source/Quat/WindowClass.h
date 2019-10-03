#pragma once

class WindowClass
{
public:
	virtual void Initialize();
	int Run();

	static inline void GetWindowSize(int& inWidth, int& inHeight)
	{
		inWidth = mClientWidth;
		inHeight = mClientHeight;
	}

	//getter, setter
public:
	inline static HINSTANCE GetInstance() { return mhAppInst; }
	inline static HWND GetWindowHandle() {	return mhMainWnd; }
	inline static void SetHInstance(HINSTANCE hInst) { mhAppInst = hInst; }
protected:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool InitMainWindow();
	virtual void GameRun() {};

	void CenterWindow(HWND hwnd, const RECT& clientRect);

#pragma region WindowProperty
protected:
	static int mClientWidth;
	static int mClientHeight;

private:
	static HWND mhMainWnd;
	static HINSTANCE mhAppInst;
	
	std::wstring mMainWndCaption = TEXT("d3dApp");

	friend LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	SINGLETON(WindowClass)
#pragma endregion
};