#include "EnginePCH.h"

unique_ptr<Direct2D> Direct2D::sInstance = nullptr;

void Direct2D::Initialize(const Engine& inEngine)
{
	CreateDevice(inEngine.md3dDevice.Get());
	CreateBitmapRenderTarget(inEngine.mSwapChain.Get());
	InitializeTextFormats();
}

void Direct2D::Update(float DeltaTimes)
{
}

void Direct2D::Begin()
{
	mDeviceContext->BeginDraw();
}

void Direct2D::End()
{
	mDeviceContext->EndDraw();
}

void Direct2D::Draw()
{
}

void Direct2D::OnResize(IDXGISwapChain* inSwapChain)
{
	CreateBitmapRenderTarget(inSwapChain);
}

void Direct2D::Reset()
{
	mDeviceContext->SetTarget(nullptr);
}

void Direct2D::DrawText(const D2D1_RECT_F& rt, const tstring& text, const DirectX::XMFLOAT4& color, DWRITE_TEXT_ALIGNMENT alignment)
{
	mTextFormat->SetTextAlignment(alignment);
	mColorbrush->SetColor(D2D1::ColorF(color.x, color.y, color.z, color.w));
	mDeviceContext->DrawText(text.c_str(), static_cast<UINT32>(text.length()), mTextFormat.Get(), rt, mColorbrush.Get());
}

void Direct2D::CreateDevice(ID3D11Device* inDevice)
{
	ThrowDxFail(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown * *>(mWriteFactory.GetAddressOf())));
	ThrowDxFail(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, mD2dFactory.GetAddressOf()));

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	ThrowDxFail(inDevice->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf())));

	ThrowDxFail(mD2dFactory->CreateDevice(dxgiDevice.Get(), mD2device.GetAddressOf()));
	ThrowDxFail(mD2device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, mDeviceContext.GetAddressOf()));
}

void Direct2D::CreateBitmapRenderTarget(IDXGISwapChain* inSwapChain)
{
	D2D1_BITMAP_PROPERTIES1 bp{};
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.f;
	bp.dpiY = 96.f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBuffer;
	ThrowDxFail(inSwapChain->GetBuffer(0, IID_PPV_ARGS(dxgiBuffer.GetAddressOf())));

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
	ThrowDxFail(mDeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, targetBitmap.GetAddressOf()));

	mDeviceContext->SetTarget(targetBitmap.Get());
}

void Direct2D::InitializeTextFormats()
{
	ThrowDxFail(mDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), mColorbrush.GetAddressOf()));

	ThrowDxFail(mWriteFactory->CreateTextFormat(TEXT("Consolas"), nullptr, 
		DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, 
		DWRITE_FONT_STRETCH_NORMAL, 20.0f, TEXT("en-us"), 
		mTextFormat.GetAddressOf()));

	ThrowDxFail(mTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
	ThrowDxFail(mTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));
}