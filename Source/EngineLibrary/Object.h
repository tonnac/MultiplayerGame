#pragma once

struct Transform
{
	XMFLOAT3 Scale;
	XMFLOAT3 Location;
	XMFLOAT4 Quaternion;
};

class Object
{
public:
	Object()
	{
		if (mDevice == nullptr || mDeviceContext == nullptr)
		{
			mDevice = Engine::sInstance->GetDevice();
			mDeviceContext = Engine::sInstance->GetDeviceContext();
		}
	}

public:
	virtual void Initialize() {}
	virtual void Update(float DeltaTimes) {}
	virtual void Draw() {}

protected:
	static ID3D11Device* mDevice;
	static ID3D11DeviceContext* mDeviceContext;
};

ID3D11Device* Object::mDevice = nullptr;
ID3D11DeviceContext* Object::mDeviceContext = nullptr;