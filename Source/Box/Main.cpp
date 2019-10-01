#include "EnginePCH.h"


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
	virtual void Tick(float DeltaTimes);
	virtual void GameDraw()override;

	void CreateVBIB();
	void CreateVSPSIL();
	void CreateCB();

private:
	void SubDivideGeometry(vector<Vertex>& inVertex, vector<uint16_t>& inIndices);
	Vertex MidPoint(const Vertex& inLeft, const Vertex& inRight);

private:
	Microsoft::WRL::ComPtr<ID3DBlob> mVertexBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mIndexBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputlayout = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mWireframe = nullptr;

	ID3D11RasterizerState* mRaster = nullptr;

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

	CreateVBIB();
	CreateVSPSIL();
	CreateCB();

	D3D11_RASTERIZER_DESC rsDesc = CD3D11_RASTERIZER_DESC(D3D11_DEFAULT);

	ThrowDxFail(md3dDevice->CreateRasterizerState(&rsDesc, mRasterizer.GetAddressOf()));
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowDxFail(md3dDevice->CreateRasterizerState(&rsDesc, mWireframe.GetAddressOf()));
}

void Sample::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

	constexpr float radspeed = 50.f;
	constexpr float speed = 300.f;

	if (KEYSTATE(DIK_D, Keystate::KEY_HOLD))
	{
		mPhi += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_LBUTTON, Keystate::KEY_HOLD))
	{
		MousePos cPos = DirectInput::sInstance->GetMousePos();
		MousePos lPos = DirectInput::sInstance->GetLastMousePos();

		mPhi += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lX - lPos.lX);
		mTheta += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lY - lPos.lY);
		mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	}
	if (KEYSTATE(DIK_A, Keystate::KEY_HOLD))
	{
		mPhi -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_W, Keystate::KEY_HOLD))
	{
		mTheta += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
		mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	}
	if (KEYSTATE(DIK_S, Keystate::KEY_HOLD))
	{
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
	if (KEYSTATE(DIK_M, Keystate::KEY_PUSH))
	{
		if (mRaster == mWireframe.Get())
		{
			mRaster = mRasterizer.Get();
		}
		else
		{
			mRaster = mWireframe.Get();
		}
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
	mDeviceContext->VSSetConstantBuffers(1, 1, mConstantBuffer.GetAddressOf());
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mDeviceContext->RSSetState(mRaster);
	mDeviceContext->DrawIndexed((UINT)mIndexBlob->GetBufferSize() / sizeof(uint16_t), 0, 0);
}

void Sample::CreateVBIB()
{
	array<Vertex, 24> triangle = {
		//Front
		Vertex{{-75.0f,	-75.0f,	-75.0f}, Colors::Cyan},
		Vertex{{-75.0f,	75.0f,	-75.0f}, Colors::PaleVioletRed},
		Vertex{{75.0f,	75.0f,	-75.0f}, Colors::SandyBrown},
		Vertex{{75.0f,	-75.0f,	-75.0f}, Colors::OliveDrab},
		//Up
		Vertex{{-75.0f,	75.0f,	-75.0f}, Colors::PaleVioletRed},
		Vertex{{-75.0f,	75.0f,	75.0f}, Colors::MediumSpringGreen},
		Vertex{{75.0f,	75.0f,	75.0f}, Colors::LightCoral},
		Vertex{{75.0f,	75.0f,	-75.0f}, Colors::Fuchsia},
		//Left
		Vertex{{-75.0f,	-75.0f,	75.0f}, Colors::RosyBrown},
		Vertex{{-75.0f,	75.0f,	75.0f}, Colors::Silver},
		Vertex{{-75.0f,	75.0f,	-75.0f}, Colors::RosyBrown},
		Vertex{{-75.0f,	-75.0f,	-75.0f}, Colors::Silver},
		//Right
		Vertex{{75.0f,	-75.0f,	-75.0f}, Colors::Moccasin},
		Vertex{{75.0f,	75.0f,	-75.0f}, Colors::Honeydew},
		Vertex{{75.0f,	75.0f,	75.0f}, Colors::SlateBlue},
		Vertex{{75.0f,	-75.0f,	75.0f}, Colors::LightCoral},
		//Back
		Vertex{{-75.0f,	-75.0f,	75.0f}, Colors::Gold},
		Vertex{{75.0f,	-75.0f,	75.0f}, Colors::DarkMagenta},
		Vertex{{75.0f,	75.0f,	75.0f}, Colors::Ivory},
		Vertex{{-75.0f,	75.0f,	75.0f}, Colors::Indigo},
		//Down
		Vertex{{-75.0f,	-75.0f,	-75.0f}, Colors::White},
		Vertex{{75.0f,	-75.0f,	-75.0f}, Colors::RoyalBlue},
		Vertex{{75.0f,	-75.0f,	75.0f}, Colors::YellowGreen},
		Vertex{{-75.0f,	-75.0f,	75.0f}, Colors::WhiteSmoke},
	};

	vector<Vertex> v;
	v.assign(&(triangle.data())[0], &(triangle.data())[triangle.size()]);

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

	vector<uint16_t> i;
	i.assign(&(indicies.data())[0], &(indicies.data())[indicies.size()]);

	for (int p = 0; p < 5; ++p)
	{
		SubDivideGeometry(v, i);
	}

	const uint32_t vbByteSize = static_cast<uint32_t>(v.size() * sizeof(Vertex));
	const uint32_t ibByteSize = static_cast<uint32_t>(i.size() * sizeof(uint16_t));

	D3DCreateBlob(vbByteSize, mVertexBlob.GetAddressOf());
	memcpy(mVertexBlob->GetBufferPointer(), v.data(), vbByteSize);
	D3DCreateBlob(ibByteSize, mIndexBlob.GetAddressOf());
	memcpy(mIndexBlob->GetBufferPointer(), i.data(), ibByteSize);

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

	indexbufferDesc.ByteWidth = (UINT)mIndexBlob->GetBufferSize();
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.CPUAccessFlags = 0;

	ThrowDxFail(md3dDevice->CreateBuffer(&indexbufferDesc, &indexsourceData, mIndexBuffer.GetAddressOf()));

}

void Sample::CreateVSPSIL()
{
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

void Sample::CreateCB()
{
	//Constant Buffer;

	D3D11_BUFFER_DESC constanctbufferDesc{};
	D3D11_SUBRESOURCE_DATA constantsourceData{};

	constanctbufferDesc.ByteWidth = sizeof(CB);
	constanctbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constanctbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	constantsourceData.pSysMem = &mConstantData;

	ThrowDxFail(md3dDevice->CreateBuffer(&constanctbufferDesc, &constantsourceData, mConstantBuffer.GetAddressOf()));
}

void Sample::SubDivideGeometry(vector<Vertex>& inVertex, vector<uint16_t>& inIndices)
{
	vector<Vertex> tempVertex = inVertex;
	vector<uint16_t> tempIndex = inIndices;

	inVertex.resize(0);
	inIndices.resize(0);

	int triNum = static_cast<int>(tempIndex.size() / 3);

	for (int i = 0; i < triNum; ++i)
	{
		Vertex v0 = tempVertex[tempIndex[(size_t)i * 3 + 0]];
		Vertex v1 = tempVertex[tempIndex[(size_t)i * 3 + 1]];
		Vertex v2 = tempVertex[tempIndex[(size_t)i * 3 + 2]];

		Vertex m0 = MidPoint(v0, v1);
		Vertex m1 = MidPoint(v1, v2);
		Vertex m2 = MidPoint(v0, v2);

		inVertex.push_back(v0);
		inVertex.push_back(m0);
		inVertex.push_back(m2);
		inVertex.push_back(v1);
		inVertex.push_back(m1);
		inVertex.push_back(v2);

		inIndices.push_back(i * 6 + 0);
		inIndices.push_back(i * 6 + 1);
		inIndices.push_back(i * 6 + 2);
								
		inIndices.push_back(i * 6 + 1);
		inIndices.push_back(i * 6 + 4);
		inIndices.push_back(i * 6 + 2);
								
		inIndices.push_back(i * 6 + 1);
		inIndices.push_back(i * 6 + 3);
		inIndices.push_back(i * 6 + 4);
								
		inIndices.push_back(i * 6 + 2);
		inIndices.push_back(i * 6 + 4);
		inIndices.push_back(i * 6 + 5);
	}
}

Vertex Sample::MidPoint(const Vertex& inLeft, const Vertex& inRight)
{
	using namespace DirectX;

	XMVECTOR lPosVec = DirectX::XMLoadFloat3(&inLeft.Pos);
	XMVECTOR rPosVec = DirectX::XMLoadFloat3(&inRight.Pos);

	XMVECTOR midPos = (lPosVec + rPosVec) * 0.5f;

	XMFLOAT3 Pos;

	DirectX::XMStoreFloat3(&Pos, midPos);

	XMVECTOR lColVec = DirectX::XMLoadFloat4(&inLeft.Color);
	XMVECTOR rColVec = DirectX::XMLoadFloat4(&inRight.Color);

	XMVECTOR midColor = (lColVec + rColVec) * 0.5f;

	XMFLOAT4 color;

	DirectX::XMStoreFloat4(&color, midColor);

	return { Pos , color};
}
