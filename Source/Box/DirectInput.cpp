#include "EnginePCH.h"


InputPtr DirectInput::sInstance = nullptr;

void DirectInput::Initialize()
{
	ThrowDxFail(DirectInput8Create(WindowClass::GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(mDirectInput.GetAddressOf()), nullptr));

	ThrowDxFail(mDirectInput->CreateDevice(GUID_SysKeyboard, mKeyboardDevice.GetAddressOf(), nullptr));
	ThrowDxFail(mKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	ThrowDxFail(mKeyboardDevice->SetCooperativeLevel(WindowClass::GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY));
	while (mKeyboardDevice->Acquire() == DIERR_INPUTLOST);


	ThrowDxFail(mDirectInput->CreateDevice(GUID_SysMouse, mMouseDevice.GetAddressOf(), nullptr));
	ThrowDxFail(mMouseDevice->SetDataFormat(&c_dfDIMouse));
	ThrowDxFail(mMouseDevice->SetCooperativeLevel(WindowClass::GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));
	while (mMouseDevice->Acquire() == DIERR_INPUTLOST);
}

void DirectInput::Update(float DeltaTimes)
{
	if (FAILED(mKeyboardDevice->GetDeviceState(sizeof(uint8_t) * MAXKEYNUM, mCurrentKeystate)))
	{
		while (mKeyboardDevice->Acquire() == DIERR_INPUTLOST);
		mKeyboardDevice->Acquire();
	}

	if (FAILED(mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mCurrentMouseState)))
	{
		while (mMouseDevice->Acquire() == DIERR_INPUTLOST);
		mMouseDevice->Acquire();
	}
}

void DirectInput::PostUpdate(float DeltaTimes)
{
	memcpy(mBeforeKeystate, mCurrentKeystate, sizeof(uint8_t) * MAXKEYNUM);
	memcpy(&mBeforeMouseState, &mCurrentMouseState, sizeof(DIMOUSESTATE));
}

MousePos DirectInput::GetMousePos() const
{
	return MousePos{ mCurrentMouseState.lX, mCurrentMouseState.lY, mCurrentMouseState.lZ };
}

MousePos DirectInput::GetLastMousePos() const
{
	return MousePos{ mBeforeMouseState.lX, mBeforeMouseState.lY, mBeforeMouseState.lZ };
}

Keystate DirectInput::GetKeystate(DWORD dwKey) const
{
	if (dwKey & 0x100)
	{
		int buttonNum = 0;
		switch (dwKey)
		{
		case DIK_LBUTTON:
			buttonNum = 0;
			break;
		case DIK_RBUTTON:
			buttonNum = 1;
			break;
		case DIK_MBUTTON:
			buttonNum = 2;
			break;
		}

		if (KEYDOWN(mBeforeMouseState.rgbButtons[buttonNum]))
		{
			if (KEYDOWN(mCurrentMouseState.rgbButtons[buttonNum]))
			{
				return Keystate::KEY_HOLD;
			}
			else
			{
				return Keystate::KEY_UP;
			}
		}
		else
		{
			if (KEYDOWN(mCurrentMouseState.rgbButtons[buttonNum]))
			{
				return Keystate::KEY_PUSH;
			}
			else
			{
				return Keystate::KEY_FREE;
			}
		}
	}
	else
	{
		if (KEYDOWN(mBeforeKeystate[dwKey]))
		{
			if (KEYDOWN(mCurrentKeystate[dwKey]))
			{
				return Keystate::KEY_HOLD;
			}
			else
			{
				return Keystate::KEY_UP;
			}
		}
		else
		{
			if (KEYDOWN(mCurrentKeystate[dwKey]))
			{
				return Keystate::KEY_PUSH;
			}
			else
			{
				return Keystate::KEY_FREE;
			}
		}
	}
}
