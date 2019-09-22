#pragma once

class Device : public WindowClass
{
	using Super = WindowClass;

public:
	Device(HINSTANCE hInst) : WindowClass(hInst) 
	{
		memset(&mSwapchainDesc, 0, sizeof(mSwapchainDesc));
	}
	virtual void Initialize() override;

protected:
	//override
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)override;
private:
	//DirectX 11 Function
	void InitDirect3D();
	void CreateSwapChain();
	virtual void OnResize();

	void LogAdapters();
	void LogAdapterOutput(IDXGIAdapter* inAdapter);
	void LogOutputDisplayModes(IDXGIOutput* inOutput, DXGI_FORMAT format);

	void CreateDepthStencilView();

protected:
	bool mMinimized = false;
	bool mMaximized = false;
	bool mResizing = false;
	bool mFullscreenState = false;

protected:
	Microsoft::WRL::ComPtr<ID3D11Device> md3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;

	Microsoft::WRL::ComPtr<IDXGIFactory> mDxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;

	D3D11_VIEWPORT mViewPort = {};

	DXGI_SWAP_CHAIN_DESC mSwapchainDesc = {};
	D3D_FEATURE_LEVEL mFeatureLevel = {};

	static constexpr DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	static constexpr DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	SINGLE(Device)
};