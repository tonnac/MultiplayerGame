#pragma once

class Timing;
struct PermanentText;

class Engine : public Device
{
	using Super = Device;

public:
	virtual ~Engine();						

public:
	virtual void Initialize() override;

protected:
	virtual void OnResize() override;
	virtual void Tick(float DeltaTimes);
	virtual void GameDraw();

private:
	virtual void GameRun()override;
	void Update(float DeltaTimes);

	void ClearBuffer();
	void Draw();

	void CalculateFrameStats();

private:
	shared_ptr<Timing> mTimer;
	static constexpr DirectX::XMFLOAT4 mBackColor = Colors::LightBlue;

	bool mShowFPS = true;

	friend class Direct2D;

	SINGLETON(Engine)
};

#define ENGINE Engine::sInstance
using EnginePtr = unique_ptr<Engine>;