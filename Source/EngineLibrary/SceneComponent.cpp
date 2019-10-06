#include "EnginePCH.h"

SceneComponent::SceneComponent()
{
	if (sizeof(IndexBuffer) == sizeof(uint32_t))
	{
		mFormat = DXGI_FORMAT_R32_UINT;
	}
}

XMFLOAT3 SceneComponent::GetComponentLocation() const
{
	return mTranform.Location;
}

XMFLOAT3 SceneComponent::GetComponentScale() const
{
	return mTranform.Scale;
}

XMFLOAT4 SceneComponent::GetComponentQuaternion() const
{
	return mTranform.Quaternion;
}

Transform SceneComponent::GetComponentTransform() const
{
	return mTranform;
}

void SceneComponent::Initialize()
{
}

void SceneComponent::Update(float DeltaTimes)
{

}

void SceneComponent::Draw()
{
	if (misDraw)
	{
		mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &mOffset);
		mDeviceContext->VSSetConstantBuffers(1, 1, mObjConstant.GetAddressOf());
		mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		mDeviceContext->DrawIndexed((UINT)(mIndexBlob->GetBufferSize() / sizeof(IndexBuffer)), 0, 0);
	}
}
