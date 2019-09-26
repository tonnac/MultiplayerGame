#include "EnginePCH.h"

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

class Sample : public Engine
{
	using Super = Engine;
public:
	virtual void Initialize()override;
private:
	virtual void Update(float DeltaTimes);
	virtual void GameDraw()override;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> mVertexBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mIndexBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputlayout = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;

	SINGLETON(Sample)
};

unique_ptr<Sample> Sample::sInstance = nullptr;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd)
{
	try
	{
		Sample::StaticInit();
		Sample::sInstance->SetHInstance(hInstance);

		Sample::sInstance->Initialize();
		return Sample::sInstance->Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), TEXT("HR Failed"), MB_OK);
		return 0;
	}
}

void Sample::Initialize()
{
	Super::Initialize();

	array<Vertex, 3> triangle = {
		Vertex{{-1.f,-1.f,0.f}, Colors::Cyan},
		Vertex{{0.f,1.f,0.f}, Colors::PaleVioletRed},
		Vertex{{1.f,-1.f,0.f}, Colors::LimeGreen},
	};

	array<uint16_t, 3> indicies = { 0, 1, 2 };

	constexpr uint32_t vbByteSize = static_cast<uint32_t>(triangle.size() * sizeof(Vertex));
	constexpr uint16_t ibByteSize = static_cast<uint16_t>(indicies.size() * sizeof(uint16_t));

	D3DCreateBlob(vbByteSize, mVertexBlob.GetAddressOf());
	memcpy(mVertexBlob->GetBufferPointer(), triangle.data(), vbByteSize);
	D3DCreateBlob(ibByteSize, mIndexBlob.GetAddressOf());
	memcpy(mIndexBlob->GetBufferPointer(), indicies.data(), ibByteSize);

	D3D11_BUFFER_DESC bufferDesc{};
	D3D11_SUBRESOURCE_DATA sourceData{};

	sourceData.pSysMem = mVertexBlob->GetBufferPointer();
	
	bufferDesc.ByteWidth = (UINT)mVertexBlob->GetBufferSize();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;


	ThrowDxFail(md3dDevice->CreateBuffer(&bufferDesc, &sourceData, &mVertexBuffer));

	D3D11_BUFFER_DESC indexbufferDesc{};
	D3D11_SUBRESOURCE_DATA indexsourceData{};

	indexsourceData.pSysMem = mIndexBlob->GetBufferPointer();

	indexbufferDesc.ByteWidth = (UINT)mVertexBlob->GetBufferSize();
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.CPUAccessFlags = 0;

	ThrowDxFail(md3dDevice->CreateBuffer(&indexbufferDesc, &indexsourceData, mIndexBuffer.GetAddressOf()));

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	//PixelShader

	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errors = nullptr;

	hr = D3DCompileFromFile(L"shaders/shader.hlsl", nullptr, nullptr, "PS", "ps_5_0", dwShaderFlags, 0, byteCode.GetAddressOf(), errors.GetAddressOf());

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	ThrowDxFail(hr);

	ThrowDxFail(md3dDevice->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));

	byteCode.Reset();
	errors.Reset();

	//VertexShader

	hr = D3DCompileFromFile(L"shaders/shader.hlsl", nullptr, nullptr, "VS", "vs_5_0", dwShaderFlags, 0, byteCode.GetAddressOf(), errors.GetAddressOf());

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	ThrowDxFail(hr);

	ThrowDxFail(md3dDevice->CreateVertexShader(byteCode->GetBufferPointer(), (UINT)byteCode->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));

	//InputLayout;

	array<D3D11_INPUT_ELEMENT_DESC, 2> layout =
	{
		D3D11_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ThrowDxFail(md3dDevice->CreateInputLayout(layout.data(), (UINT)std::size(layout), byteCode->GetBufferPointer(), byteCode->GetBufferSize(), mInputlayout.GetAddressOf()));
} 

void Sample::Update(float DeltaTimes)
{
	Super::Update(DeltaTimes);
}

void Sample::GameDraw()
{
	Super::GameDraw();
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mInputlayout.Get());
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mDeviceContext->DrawIndexed((UINT)mIndexBlob->GetBufferSize(), 0, 0);
}
