#pragma once

#define SCREENLOG(time, color, ...)													\
{																					\
	Direct2D::sInstance->AddText(time, color, D3DUtil::Printf(__VA_ARGS__));		\
}

// 기본값3초 검정색
#define DLOG(...) SCREENLOG(3.f, Colors::Black, __VA_ARGS__)


struct TextStructure
{
	TextStructure(float inDuration, const tstring& text, const DirectX::XMFLOAT4& color)
		: Duration(inDuration), Text(text), Color(color) {}
	float ElapsedTime = 0.f;
	float Duration = 0.f;
	DirectX::XMFLOAT4 Color = {};
	tstring Text = {};

	bool Update(float DeltaTimes)
	{
		ElapsedTime += DeltaTimes;
		if (ElapsedTime >= Duration)
		{
			return true;
		}
		return false;
	}
};

struct PermanentText
{
	PermanentText(const D2D1_RECT_F& rt, const DirectX::XMFLOAT4& color, const tstring& inText)
		: Rt(rt), Color(color), Text(inText)
	{}

	D2D1_RECT_F Rt = {};
	DirectX::XMFLOAT4 Color = {};
	tstring Text = tstring();
};

class Direct2D
{
	SINGLETON(Direct2D)

public:
	void Initialize(const Engine& inEngine);
	void Update(float DeltaTimes);

	void Draw();

	void AddText(float inDuration, const DirectX::XMFLOAT4 inColor, const tstring& inText);
	void AddPermanentText(const D2D1_RECT_F& rt, const DirectX::XMFLOAT4 inColor, const tstring& inText);

	void OnResize(IDXGISwapChain* inSwapChain);
	//스왑체인에 연결되어있는 장치들을 먼저 제거
	void Reset();

	void DrawText(const D2D1_RECT_F& rt, const tstring& text, const DirectX::XMFLOAT4& color, DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_LEADING);

private:
	void CreateDevice(ID3D11Device * inDevice);
	void CreateBitmapRenderTarget(IDXGISwapChain* inSwapChain);
	void InitializeTextFormats();

private:
	list<TextStructure> mTextlist;
	stack<PermanentText> mPermanentText;

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