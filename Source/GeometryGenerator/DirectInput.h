#pragma once

#define KEYDOWN(x) (x) & 0x80 ? true : false
#define KEYSTATE(x, y) DirectInput::sInstance->GetKeystate((x)) == (y) ? true : false

constexpr uint16_t MAXKEYNUM = 256;
constexpr uint32_t DIK_LBUTTON = 0x100;
constexpr uint32_t DIK_RBUTTON = 0x101;
constexpr uint32_t DIK_MBUTTON = 0x102;


enum class Keystate : uint8_t
{
	KEY_FREE,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP,
};

struct MousePos
{
	LONG lX, lY, lZ;
};

class DirectInput
{
public:
	void Initialize();
	void Update(float DeltaTimes);
	void PostUpdate(float DeltaTimes);

public:
	Keystate GetKeystate(DWORD dwKey) const;
	MousePos GetMousePos() const;
	MousePos GetLastMousePos() const;

private:
	Microsoft::WRL::ComPtr<IDirectInput8> mDirectInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mKeyboardDevice = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mMouseDevice = nullptr;

	uint8_t mCurrentKeystate[MAXKEYNUM] = {};
	uint8_t mBeforeKeystate[MAXKEYNUM] = {};

	DIMOUSESTATE mCurrentMouseState = {};
	DIMOUSESTATE mBeforeMouseState = {};

	SINGLETON(DirectInput)
};

using InputPtr = unique_ptr<DirectInput>;