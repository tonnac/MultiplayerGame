#pragma once

class Timing;
struct PermanentText;

class Engine : public Device
{
	using Super = Device;

public:
	Engine(HINSTANCE hInst);
	virtual ~Engine();

public:
	virtual void Initialize() override;

protected:
	virtual void OnResize() override;

private:
	virtual void GameRun()override;
	virtual void Update(float DeltaTimes);
	virtual void GameDraw();

	void ClearBuffer();
	void Draw();

	void CalculateFrameStats();

private:
	unique_ptr<Timing> mTimer;
	static constexpr DirectX::XMFLOAT4 mBackColor = Colors::LightBlue;

	friend class Direct2D;
};