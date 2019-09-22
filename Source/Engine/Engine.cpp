#include "EnginePCH.h"
#include "Timing.h"

Engine::~Engine()
{}

Engine::Engine(HINSTANCE hInst) : Device(hInst) 
{
	Direct2D::StaticInit();
}

void Engine::Initialize()
{
	Super::Initialize();
	Direct2D::sInstance->Initialize(*this);
	mTimer = std::make_unique<Timing>();

	OnResize();
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
	Direct2D::sInstance->Update(DeltaTimes);
}

void Engine::Draw()
{
	ClearBuffer();
	Direct2D::sInstance->Begin();
	GameDraw();
	Direct2D::sInstance->End();
	ThrowDxFail(mSwapChain->Present(0, 0));
}

void Engine::GameDraw()
{
	Direct2D::sInstance->Draw();
	Direct2D::sInstance->DrawText({0, 0, (float)mClientWidth, (float)mClientHeight}, D3DUtil::Printf(TEXT("%d qweq %f"), 32, 3.2f), Colors::Black);
	Direct2D::sInstance->DrawText({ 0, 20, (float)mClientWidth, (float)mClientHeight }, D3DUtil::Printf(TEXT("%d qweq %f"), 32, mTimer->GetDeltaTime()), Colors::Black);
	Direct2D::sInstance->DrawText({ 0, 40, (float)mClientWidth, (float)mClientHeight }, D3DUtil::Printf(TEXT("%d qweq %f"), 32, mTimer->GetDeltaTime()), Colors::Black);
	Direct2D::sInstance->DrawText({ 0, 60, (float)mClientWidth, (float)mClientHeight }, D3DUtil::Printf(TEXT("%d qweq %f"), 32, 3.2f), Colors::Black);
}

void Engine::OnResize()
{
	Direct2D::sInstance->Reset();
	Super::OnResize();
	Direct2D::sInstance->OnResize(mSwapChain.Get());
}

void Engine::ClearBuffer()
{
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), &mBackColor.x);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	mDeviceContext->RSSetViewports(1, &mViewPort);
}