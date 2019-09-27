#include "EnginePCH.h"

using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct CB
{
	DirectX::XMFLOAT4X4 gWorldViewProj;
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
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;

	CB mConstantData;

	float mRadius = 600;
	float mTheta = DirectX::XM_PIDIV2;
	float mPhi = 0.f;

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

	array<Vertex, 24> triangle = {
		//Front
		Vertex{{-37.5f,	-37.5f,	-37.5f}, Colors::Cyan},
		Vertex{{-37.5f,	37.5f,	-37.5f}, Colors::PaleVioletRed},
		Vertex{{37.5f,	37.5f,	-37.5f}, Colors::SandyBrown},
		Vertex{{37.5f,	-37.5f,	-37.5f}, Colors::OliveDrab},
		//Up
		Vertex{{-37.5f,	37.5f,	-37.5f}, Colors::PaleVioletRed},
		Vertex{{-37.5f,	37.5f,	37.5f}, Colors::MediumSpringGreen},
		Vertex{{37.5f,	37.5f,	37.5f}, Colors::LightCoral},
		Vertex{{37.5f,	37.5f,	-37.5f}, Colors::Fuchsia},
		//Left
		Vertex{{-37.5f,	-37.5f,	37.5f}, Colors::RosyBrown},
		Vertex{{-37.5f,	37.5f,	37.5f}, Colors::Silver},
		Vertex{{-37.5f,	37.5f,	-37.5f}, Colors::RosyBrown},
		Vertex{{-37.5f,	-37.5f,	-37.5f}, Colors::Silver},
		//Right
		Vertex{{37.5f,	-37.5f,	-37.5f}, Colors::Moccasin},
		Vertex{{37.5f,	37.5f,	-37.5f}, Colors::Honeydew},
		Vertex{{37.5f,	37.5f,	37.5f}, Colors::SlateBlue},
		Vertex{{37.5f,	-37.5f,	37.5f}, Colors::LightCoral},
		//Back
		Vertex{{-37.5f,	-37.5f,	37.5f}, Colors::Gold},
		Vertex{{37.5f,	-37.5f,	37.5f}, Colors::DarkMagenta},
		Vertex{{37.5f,	37.5f,	37.5f}, Colors::Ivory},
		Vertex{{-37.5f,	37.5f,	37.5f}, Colors::Indigo},
		//Down
		Vertex{{-37.5f,	-37.5f,	-37.5f}, Colors::White},
		Vertex{{37.5f,	-37.5f,	-37.5f}, Colors::RoyalBlue},
		Vertex{{37.5f,	-37.5f,	37.5f}, Colors::YellowGreen},
		Vertex{{-37.5f,	-37.5f,	37.5f}, Colors::WhiteSmoke},
	};

	array<uint16_t, 36> indicies = { 
		0, 1, 2 ,
		0, 2, 3 ,

		4, 5, 6 ,
		4, 6, 7 ,

		8, 9, 10 ,
		8, 10, 11 ,

		12, 13, 14 ,
		12, 14, 15 ,

		16, 17, 18 ,
		16, 18, 19 ,

		20, 21, 22 ,
		20, 22, 23
	};

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

	//Constant Buffer;

	D3D11_BUFFER_DESC constanctbufferDesc{};
	D3D11_SUBRESOURCE_DATA constantsourceData{};

	constanctbufferDesc.ByteWidth = sizeof(CB);
	constanctbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constanctbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	constantsourceData.pSysMem = &mConstantData;

	ThrowDxFail(md3dDevice->CreateBuffer(&constanctbufferDesc, &constantsourceData, mConstantBuffer.GetAddressOf())); 
}

void Sample::Update(float DeltaTimes)
{
	Super::Update(DeltaTimes);

	constexpr float radspeed = 50.f;
	constexpr float speed = 300.f;

	if (KEYSTATE(DIK_D, Keystate::KEY_HOLD))
	{
		MousePos lastpos = DirectInput::sInstance->GetLastMousePos();
		MousePos currentpos = DirectInput::sInstance->GetMousePos();

		mPhi += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_A, Keystate::KEY_HOLD))
	{
		MousePos lastpos = DirectInput::sInstance->GetLastMousePos();
		MousePos currentpos = DirectInput::sInstance->GetMousePos();

		mPhi -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_W, Keystate::KEY_HOLD))
	{
		MousePos lastpos = DirectInput::sInstance->GetLastMousePos();
		MousePos currentpos = DirectInput::sInstance->GetMousePos();

		mTheta += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
		mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	}
	if (KEYSTATE(DIK_S, Keystate::KEY_HOLD))
	{
		MousePos lastpos = DirectInput::sInstance->GetLastMousePos();
		MousePos currentpos = DirectInput::sInstance->GetMousePos();

		mTheta -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
		mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	}

	if (KEYSTATE(DIK_DOWNARROW, Keystate::KEY_HOLD))
	{
		mRadius += DeltaTimes * speed;
		mRadius = MathHelper::Clamp(mRadius, 100.f, 600.f);
	}

	if (KEYSTATE(DIK_UPARROW, Keystate::KEY_HOLD))
	{
		mRadius -= DeltaTimes * speed;
		mRadius = MathHelper::Clamp(mRadius, 100.f, 600.f);
	}

	float x = mRadius * sinf(mTheta) * cosf(mPhi);
	float y = mRadius * cosf(mTheta);
	float z = mRadius * sinf(mTheta) * sinf(mPhi);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX look = DirectX::XMMatrixLookAtLH(pos, target, up);

	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 1.33f, 1, 1000);

	DirectX::XMMATRIX worldviewproj = world * look * proj;

	DirectX::XMStoreFloat4x4(&mConstantData.gWorldViewProj, DirectX::XMMatrixTranspose(worldviewproj));

	mDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mConstantData, 0, 0);
}

void Sample::GameDraw()
{
	Super::GameDraw();
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mInputlayout.Get());
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	mDeviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mDeviceContext->DrawIndexed((UINT)mIndexBlob->GetBufferSize() / sizeof(uint16_t), 0, 0);
}
