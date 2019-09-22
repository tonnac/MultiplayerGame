#pragma once

class Direct2D
{
	SINGLETON(Direct2D)

public:
	void Initialize(const Engine& inEngine);
	void Update(float DeltaTimes);

	void Begin();
	void End();

	void Draw();

	void OnResize(IDXGISwapChain* inSwapChain);
	void Reset();

	void DrawText(const D2D1_RECT_F& rt, const tstring& text, const DirectX::XMFLOAT4& color, DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_LEADING);

private:
	void CreateDevice(ID3D11Device * inDevice);
	void CreateBitmapRenderTarget(IDXGISwapChain* inSwapChain);
	void InitializeTextFormats();

private:
	Microsoft::WRL::ComPtr<IDWriteFactory2> mWriteFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Factory2> mD2dFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Device1> mD2device = nullptr;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> mDeviceContext = nullptr;

	Microsoft::WRL::ComPtr<IDWriteTextFormat> mTextFormat = nullptr;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> mTextLayOut = nullptr;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mColorbrush = nullptr;

	float mDpiX = 96.f;
	float mDpiY = 96.f;
};