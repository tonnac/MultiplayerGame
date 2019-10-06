#pragma once


class SceneComponent : public ActorComponent
{
	using Vertex = Vertex;
	using IndexBuffer = uint16_t;

public:
	SceneComponent();

public:
	XMFLOAT3 GetComponentLocation() const;
	XMFLOAT3 GetComponentScale() const;
	XMFLOAT4 GetComponentQuaternion() const;
	Transform GetComponentTransform() const;

	virtual void Initialize() override;
	virtual void Update(float DeltaTimes) override;
	virtual void Draw() override;

	void SetDraw(bool isDraw) { misDraw = isDraw; }

protected:
	uint32_t mStride = sizeof(Vertex);
	uint32_t mOffset = 0;

	bool misDraw = true;

	Transform mTranform;

	Microsoft::WRL::ComPtr<ID3DBlob> mVertexBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mIndexBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mObjConstant = nullptr;

	DXGI_FORMAT mFormat = DXGI_FORMAT_R16_UINT;

	ObjectConstants mConstants;
};