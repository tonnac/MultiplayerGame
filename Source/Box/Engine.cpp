#include "EnginePCH.h"
#include "Timing.h"
#include "Direct2D.h"

EnginePtr Engine::sInstance = nullptr;

Engine::~Engine()
{}

void Engine::Initialize()
{
#pragma region Singleton Instance
	Super::StaticInit();
	Direct2D::StaticInit();
	DirectInput::StaticInit();
#pragma endregion

#pragma region Singleton Initialize
	Super::Initialize();
	Direct2D::sInstance->Initialize(*this);
	DirectInput::sInstance->Initialize();
#pragma endregion

//Timer
	mTimer = std::make_shared<Timing>();

	OnResize();
}

void Engine::GameRun()
{
	mTimer->Update();

	CalculateFrameStats();

	Update(mTimer->GetDeltaTime());
	if (!mMinimized)
	{
		Draw();
	}
}

void Engine::Update(float DeltaTimes)
{
	Direct2D::sInstance->Update(DeltaTimes);
	DirectInput::sInstance->Update(DeltaTimes);

	{
		if ((KEYSTATE(DIK_LCONTROL, Keystate::KEY_HOLD)) && (KEYSTATE(DIK_F, Keystate::KEY_PUSH)))
		{
			mShowFPS = !mShowFPS;
		}

		if (KEYSTATE(DIK_ESCAPE, Keystate::KEY_PUSH))
		{
			PostQuitMessage(0);
		}
	}

	Tick(DeltaTimes);

	DirectInput::sInstance->PostUpdate(DeltaTimes);
}

void Engine::Draw()
{
	ClearBuffer();
	GameDraw();
	ThrowDxFail(mSwapChain->Present(0, 0));
}

void Engine::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.f;
	static tstring frameText;

	++frameCnt;
	if (mTimer->GetFrameStartTime() - timeElapsed > 1.f)
	{
		float fps = static_cast<float>(frameCnt);
		float mspf = 1 / fps;

		frameText = D3DUtil::Printf(TEXT("FPS: %d mspf: %.6f"), frameCnt, mspf);

		frameCnt = 0;
		timeElapsed += 1;
	}
	if (mShowFPS)
	{
		Direct2D::sInstance->AddPermanentText({ float(mClientWidth - 280), 0.f, (float)mClientWidth, (float)mClientHeight }, Colors::Black,
			frameText);
	}
}

void Engine::GameDraw()
{
	Direct2D::sInstance->Draw();
}

void Engine::OnResize()
{
	Direct2D::sInstance->Reset();
	Super::OnResize();
	Direct2D::sInstance->OnResize(mSwapChain.Get());
}

void Engine::Tick(float DeltaTimes)
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