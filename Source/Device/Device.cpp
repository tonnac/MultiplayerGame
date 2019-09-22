#include "DevicePCH.h"

using Microsoft::WRL::ComPtr;
using namespace std;

void Device::Initialize()
{
	Super::Initialize();
	InitDirect3D();

	OnResize();
}

void Device::InitDirect3D()
{
	UINT deviceFlags = 0; //= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	ThrowDxFail(CreateDXGIFactory(IID_PPV_ARGS(mDxgiFactory.GetAddressOf())));

	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hardwareResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		deviceFlags, featureLevel, static_cast<UINT>(std::size(featureLevel)), D3D11_SDK_VERSION,
		md3dDevice.GetAddressOf(), &mFeatureLevel, mDeviceContext.GetAddressOf());

	if (FAILED(hardwareResult))
	{
		ThrowDxFail(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 
		deviceFlags, featureLevel, static_cast<UINT>(std::size(featureLevel)),
		D3D11_SDK_VERSION, md3dDevice.GetAddressOf(),
		&mFeatureLevel, mDeviceContext.GetAddressOf()));
	}

#ifdef _DEBUG
	LogAdapters();
#endif

	CreateSwapChain();
}

void Device::CreateSwapChain()
{
	mSwapChain.Reset();

	DXGI_MODE_DESC modeDesc{};
	modeDesc.Width = mClientWidth;
	modeDesc.Height = mCliendHeight;
	modeDesc.RefreshRate.Denominator = 1;
	modeDesc.RefreshRate.Numerator = 60;
	modeDesc.Format = mBackBufferFormat;

	mSwapchainDesc.BufferDesc = modeDesc;
	mSwapchainDesc.SampleDesc.Count = 1;
	mSwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	mSwapchainDesc.BufferCount = 1;
	mSwapchainDesc.OutputWindow = GetWindowHandle();
	mSwapchainDesc.Windowed = TRUE;
	mSwapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowDxFail(mDxgiFactory->CreateSwapChain(md3dDevice.Get(), &mSwapchainDesc, mSwapChain.GetAddressOf()));
}

void Device::OnResize()
{
	assert(md3dDevice);
	assert(mSwapChain);

	mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	
	mRenderTargetView.Reset();
	mDepthStencilView.Reset();

	mSwapchainDesc.BufferDesc.Width = mClientWidth;
	mSwapchainDesc.BufferDesc.Height = mCliendHeight;

	ThrowDxFail(mSwapChain->ResizeBuffers(
		mSwapchainDesc.BufferCount,
		mSwapchainDesc.BufferDesc.Width,
		mSwapchainDesc.BufferDesc.Height,
		mSwapchainDesc.BufferDesc.Format,
		mSwapchainDesc.Flags));

	ComPtr<ID3D11Texture2D> texture = nullptr;
	ThrowDxFail(mSwapChain->GetBuffer(0, IID_PPV_ARGS(texture.GetAddressOf())));

	ThrowDxFail(md3dDevice->CreateRenderTargetView(texture.Get(), nullptr, mRenderTargetView.GetAddressOf()));
	CreateDepthStencilView();

	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = static_cast<float>(mClientWidth);
	mViewPort.Height = static_cast<float>(mCliendHeight);
	mViewPort.MinDepth = 0.f;
	mViewPort.MaxDepth = 1.f;
}

void Device::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* pAdapter = nullptr;
	vector<IDXGIAdapter*> AdapterList;
	while (mDxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc{};
		pAdapter->GetDesc(&desc);

		tstring text = L"***Adapter: " + tstring(desc.Description) + L"\n";

		OutputDebugString(text.c_str());

		AdapterList.push_back(pAdapter);

		++i;
	}

	for (IDXGIAdapter* adapter : AdapterList)
	{
		LogAdapterOutput(adapter);
		adapter->Release();
	}
}

void Device::LogAdapterOutput(IDXGIAdapter* inAdapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (inAdapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc{};
		output->GetDesc(&desc);

		tstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";

		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, mBackBufferFormat);

		output->Release();

		++i;
	}
}

void Device::LogOutputDisplayModes(IDXGIOutput* inOutput, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	inOutput->GetDisplayModeList(format, flags, &count, nullptr);

	vector<DXGI_MODE_DESC> modelist(count);
	inOutput->GetDisplayModeList(format, flags, &count, modelist.data());

	for (const DXGI_MODE_DESC& desc : modelist)
	{
		UINT n = desc.RefreshRate.Numerator;
		UINT d = desc.RefreshRate.Denominator;
		tstring text = L"Width = " + to_tstring(desc.Width) + L" Height = " + to_tstring(desc.Height)
			+ L" Refresh = " + to_tstring(n) + L"/" + to_tstring(d) + L"\n";
			
		OutputDebugString(text.c_str());
	}
}

void Device::CreateDepthStencilView()
{
	mDepthStencilView.Reset();

	ComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC descDepth = 
		CD3D11_TEXTURE2D_DESC(mDepthStencilFormat, mClientWidth, mCliendHeight);
	descDepth.MipLevels = 1;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowDxFail(md3dDevice->CreateTexture2D(&descDepth, nullptr, depthStencilTexture.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc =
		CD3D11_DEPTH_STENCIL_VIEW_DESC(
			depthStencilTexture.Get(),
			D3D11_DSV_DIMENSION_TEXTURE2D,
			descDepth.Format);

	ThrowDxFail(md3dDevice->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc,
		mDepthStencilView.GetAddressOf()));
}

LRESULT Device::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		mClientWidth = LOWORD(lParam);
		mCliendHeight = HIWORD(lParam);
		if (nullptr != md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mMaximized = true;
				mMinimized = false;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mMinimized == true)
				{
					mMinimized = false;
					OnResize();
				}
				else if (mMaximized == true)
				{
					mMaximized = false;
					OnResize();
				}
				else if (mResizing == true)
				{

				}
				else
				{
					OnResize();
				}
			}
		}

	case WM_ENTERSIZEMOVE:
		mResizing = true;
		return 0;

	case WM_EXITSIZEMOVE:
		mResizing = false;
		OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		return 0;

	default:
		break;
	}

	return Super::MsgProc(hwnd, msg, wParam, lParam);
}