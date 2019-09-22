#include "EnginePCH.h"
#include "Timing.h"

Engine::~Engine()
{}

Engine::Engine(HINSTANCE hInst) : Device(hInst) {}

void Engine::Initialize()
{
	Super::Initialize();
	mTimer = std::make_unique<Timing>();
}

void Engine::GameRun()
{
	mTimer->Update();
	Update(mTimer->GetDeltaTime());
	if (!mMinimized)
	{
		Draw();
	}
}

void Engine::Update(float DeltaTimes)
{
	
}

void Engine::Draw()
{
	ClearBuffer();
	GameDraw();
	mSwapChain->Present(0, 0);
}

void Engine::GameDraw()
{

}

void Engine::ClearBuffer()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), &mBackColor.x);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	mDeviceContext->RSSetViewports(1, &mViewPort);
}

