#include "EnginePCH.h"


struct Vertex
{
	Vertex() {}
	Vertex(const XMFLOAT3& inPos, const XMFLOAT4& inColor, const XMFLOAT2& inTexCoords)
		: Pos(inPos), Color(inColor), TexCoords(inTexCoords)
	{}

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 TexCoords;
};

struct CB
{
	DirectX::XMFLOAT4X4 gViewProj;
};

struct CBObj
{
	DirectX::XMFLOAT4X4 gWorld;
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

	void CreateSamplerState();
	void CreateSRV();

	void CameraUpdate(float DeltaTimes);
	void ObjectUpdate(float DeltaTimes);

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
	Microsoft::WRL::ComPtr<ID3D11Buffer> mObjConstant = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mWireframe = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSampler = nullptr;

	ID3D11RasterizerState* mRaster = nullptr;

	CB mConstantData;
	CBObj mObjConstantData;

	float mRadius = 600;
	float mTheta = DirectX::XM_PIDIV2 * 3.f;
	float mPhi = 0.f;

	XMFLOAT3 mLocation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 mScale = XMFLOAT3(1.f, 1.f, 1.f);

	float mYaw = 0.f;
	float mPitch = 0.f;
	float mRoll = 0.f;

	XMFLOAT3 mFowardVector = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3 mRightVector = XMFLOAT3(1.f, 0.f, 0.f);
	XMFLOAT3 mUpVector = XMFLOAT3(0.f, 1.f, 0.f);

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
	CreateSRV();
	CreateSamplerState();

	D3D11_RASTERIZER_DESC rsDesc = CD3D11_RASTERIZER_DESC(D3D11_DEFAULT);

	ThrowDxFail(md3dDevice->CreateRasterizerState(&rsDesc, mRasterizer.GetAddressOf()));
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowDxFail(md3dDevice->CreateRasterizerState(&rsDesc, mWireframe.GetAddressOf()));
}

void Sample::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);

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

	CameraUpdate(DeltaTimes);
	ObjectUpdate(DeltaTimes);

	mDeviceContext->UpdateSubresource(mObjConstant.Get(), 0, nullptr, &mObjConstantData, 0, 0);
	mDeviceContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &mConstantData, 0, 0);
	
	tstring Forward = D3DUtil::Printf(TEXT("Forward x: %.2f, y: %.2f, z: %.2f"), mFowardVector.x, mFowardVector.y, mFowardVector.z);
	tstring Right = D3DUtil::Printf(TEXT("Right x: %.2f, y: %.2f, z: %.2f"), mRightVector.x, mRightVector.y, mRightVector.z);
	tstring Up = D3DUtil::Printf(TEXT("Up x: %.2f, y: %.2f, z: %.2f"), mUpVector.x, mUpVector.y, mUpVector.z);

	Direct2D::sInstance->AddPermanentText({ 0.f, 0.f, 1024.f, 768.f }, Colors::Black, Forward);
	Direct2D::sInstance->AddPermanentText({ 0.f, 20.f, 1024.f, 768.f }, Colors::Black, Right);
	Direct2D::sInstance->AddPermanentText({ 0.f, 40.f, 1024.f, 768.f }, Colors::Black, Up);
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
	mDeviceContext->VSSetConstantBuffers(1, 1, mObjConstant.GetAddressOf());
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	mDeviceContext->RSSetState(mRaster);
	mDeviceContext->PSSetShaderResources(0, 1, mSRV.GetAddressOf());
	mDeviceContext->PSSetSamplers(0, 1, mSampler.GetAddressOf());
	mDeviceContext->DrawIndexed((UINT)mIndexBlob->GetBufferSize() / sizeof(uint16_t), 0, 0);
}

void Sample::CreateVBIB()
{
	array<Vertex, 24> triangle = {
		//Front
		Vertex(XMFLOAT3(-75.0f,	-75.0f, -75.0f), XMFLOAT4(Colors::Cyan), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-75.0f, 75.0f,	-75.0f), XMFLOAT4(Colors::PaleVioletRed), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f,	-75.0f), XMFLOAT4(Colors::SandyBrown), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(75.0f, -75.0f,	-75.0f), XMFLOAT4(Colors::OliveDrab), XMFLOAT2(1.f, 1.f)),
		//Up
		Vertex(XMFLOAT3(-75.0f, 75.0f, -75.0f), XMFLOAT4(Colors::PaleVioletRed), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-75.0f, 75.0f,	75.0f), XMFLOAT4(Colors::MediumSpringGreen), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f,	75.0f), XMFLOAT4(Colors::LightCoral), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f, -75.0f), XMFLOAT4(Colors::Fuchsia), XMFLOAT2(1.f, 1.f)),
		//Left
		Vertex(XMFLOAT3(-75.0f, -75.0f,	75.0f), XMFLOAT4(Colors::RosyBrown), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-75.0f, 75.0f,	75.0f), XMFLOAT4(Colors::Silver), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(-75.0f, 75.0f,	-75.0f), XMFLOAT4(Colors::RosyBrown), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-75.0f, -75.0f,	-75.0f), XMFLOAT4(Colors::Silver), XMFLOAT2(1.f, 1.f)),
		//Right
		Vertex(XMFLOAT3(75.0f, -75.0f,	-75.0f), XMFLOAT4(Colors::Moccasin), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f,	-75.0f), XMFLOAT4(Colors::Honeydew), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f,	75.0f), XMFLOAT4(Colors::SlateBlue), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(75.0f, -75.0f,	75.0f), XMFLOAT4(Colors::LightCoral), XMFLOAT2(1.f, 1.f)),
		//Back
		Vertex(XMFLOAT3(-75.0f,	-75.0f,	75.0f), XMFLOAT4(Colors::Gold), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(75.0f,	-75.0f, 75.0f), XMFLOAT4(Colors::DarkMagenta), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	75.0f,	75.0f), XMFLOAT4(Colors::Ivory), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-75.0f, 75.0f,	75.0f), XMFLOAT4(Colors::Indigo), XMFLOAT2(1.f, 1.f)),
		//Down
		Vertex(XMFLOAT3(-75.0f,	-75.0f,	-75.0f), XMFLOAT4(Colors::White), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(75.0f,	-75.0f,	-75.0f), XMFLOAT4(Colors::RoyalBlue), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(75.0f,	-75.0f,	75.0f), XMFLOAT4(Colors::YellowGreen), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-75.0f,	-75.0f,	75.0f), XMFLOAT4(Colors::WhiteSmoke), XMFLOAT2(1.f, 1.f))
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

	array<D3D11_INPUT_ELEMENT_DESC, 3> layout =
	{
		D3D11_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{"TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0}
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

	constanctbufferDesc.ByteWidth = sizeof(CBObj);
	constantsourceData.pSysMem = &mObjConstantData;

	ThrowDxFail(md3dDevice->CreateBuffer(&constanctbufferDesc, &constantsourceData, mObjConstant.GetAddressOf()));
}

void Sample::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sd{};
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxLOD = FLT_MAX;
	sd.MinLOD = FLT_MIN;

	ThrowDxFail(md3dDevice->CreateSamplerState(&sd, mSampler.GetAddressOf()));
}

void Sample::CreateSRV()
{
	D3DX11_IMAGE_LOAD_INFO loadInfo{};
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;

	tstring filePath = SolutionDir;
	filePath += TEXT("Texture/WoodCrate01.dds");
	
	HRESULT hr;
	D3DX11CreateShaderResourceViewFromFile(md3dDevice.Get(), filePath.c_str(), &loadInfo, nullptr, mSRV.GetAddressOf(), &hr);
	ThrowDxFail(hr);
}

void Sample::CameraUpdate(float DeltaTimes)
{
	constexpr float radspeed = 50.f;
	constexpr float speed = 300.f;

	//if (KEYSTATE(DIK_D, Keystate::KEY_HOLD))
	//{
	//	mPhi += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	//}
	//if (KEYSTATE(DIK_LBUTTON, Keystate::KEY_HOLD))
	//{
	//	MousePos cPos = DirectInput::sInstance->GetMousePos();
	//	MousePos lPos = DirectInput::sInstance->GetLastMousePos();

	//	mPhi += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lX - lPos.lX);
	//	mTheta += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lY - lPos.lY);
	//	mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	//}
	//if (KEYSTATE(DIK_A, Keystate::KEY_HOLD))
	//{
	//	mPhi -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	//}
	//if (KEYSTATE(DIK_W, Keystate::KEY_HOLD))
	//{
	//	mTheta += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	//	mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	//}
	//if (KEYSTATE(DIK_S, Keystate::KEY_HOLD))
	//{
	//	mTheta -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	//	mTheta = MathHelper::Clamp(mTheta, 0.1f, DirectX::XM_PI - 0.1f);
	//}

	//if (KEYSTATE(DIK_DOWNARROW, Keystate::KEY_HOLD))
	//{
	//	mRadius += DeltaTimes * speed;
	//	mRadius = MathHelper::Clamp(mRadius, 100.f, 600.f);
	//}
	//if (KEYSTATE(DIK_UPARROW, Keystate::KEY_HOLD))
	//{
	//	mRadius -= DeltaTimes * speed;
	//	mRadius = MathHelper::Clamp(mRadius, 100.f, 600.f);
	//}

	float z = mRadius * sinf(mTheta) * cosf(mPhi);
	float y = mRadius * cosf(mTheta);
	float x = mRadius * sinf(mTheta) * sinf(mPhi);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DirectX::XMMATRIX look = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 1.33f, 10, 5000);
	DirectX::XMMATRIX viewproj = look * proj;

	DirectX::XMStoreFloat4x4(&mConstantData.gViewProj, DirectX::XMMatrixTranspose(viewproj));
}

void Sample::ObjectUpdate(float DeltaTimes)
{
	using namespace DirectX;
	constexpr float radspeed = 150.f;

	if (KEYSTATE(DIK_D, Keystate::KEY_HOLD))
	{
		mYaw += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}

	if (KEYSTATE(DIK_A, Keystate::KEY_HOLD))
	{
		mYaw -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}

	if (KEYSTATE(DIK_W, Keystate::KEY_HOLD))
	{
		mPitch += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_S, Keystate::KEY_HOLD))
	{
		mPitch -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}

	if (KEYSTATE(DIK_Q, Keystate::KEY_HOLD))
	{
		mRoll += DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}
	if (KEYSTATE(DIK_E, Keystate::KEY_HOLD))
	{
		mRoll -= DirectX::XMConvertToRadians(DeltaTimes * radspeed);
	}

	if (KEYSTATE(DIK_LBUTTON, Keystate::KEY_HOLD))
	{
		MousePos cPos = DirectInput::sInstance->GetMousePos();
		MousePos lPos = DirectInput::sInstance->GetLastMousePos();

		mYaw += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lX - lPos.lX);
		mPitch += DirectX::XMConvertToRadians(DeltaTimes * radspeed * cPos.lY - lPos.lY);
	}
	
	if (KEYSTATE(DIK_UPARROW, Keystate::KEY_HOLD))
	{
		XMVECTOR Location = XMLoadFloat3(&mLocation);
		XMVECTOR Forward = DirectX::XMLoadFloat3(&mFowardVector);
		XMVECTOR S = XMVectorReplicate(DeltaTimes * radspeed);

		DirectX::XMStoreFloat3(&mLocation, XMVectorMultiplyAdd(S, Forward, Location));
	}
	if (KEYSTATE(DIK_DOWNARROW, Keystate::KEY_HOLD))
	{
		XMVECTOR Location = XMLoadFloat3(&mLocation);
		XMVECTOR Forward = DirectX::XMLoadFloat3(&mFowardVector);
		XMVECTOR S = XMVectorReplicate(DeltaTimes * -radspeed);

		DirectX::XMStoreFloat3(&mLocation, XMVectorMultiplyAdd(S, Forward, Location));
	}

	if (KEYSTATE(DIK_RIGHTARROW, Keystate::KEY_HOLD))
	{
		XMVECTOR Location = XMLoadFloat3(&mLocation);
		XMVECTOR Right = DirectX::XMLoadFloat3(&mRightVector);
		XMVECTOR S = XMVectorReplicate(DeltaTimes * radspeed);

		DirectX::XMStoreFloat3(&mLocation, XMVectorMultiplyAdd(S, Right, Location));
	}
	if (KEYSTATE(DIK_LEFTARROW, Keystate::KEY_HOLD))
	{
		XMVECTOR Location = XMLoadFloat3(&mLocation);
		XMVECTOR Right = DirectX::XMLoadFloat3(&mRightVector);
		XMVECTOR S = XMVectorReplicate(DeltaTimes * -radspeed);

		DirectX::XMStoreFloat3(&mLocation, XMVectorMultiplyAdd(S, Right, Location));
	}

	XMVECTOR S = XMLoadFloat3(&mScale);
	XMVECTOR T = XMLoadFloat3(&mLocation);
	XMVECTOR zero = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR R = DirectX::XMQuaternionRotationRollPitchYaw(mPitch, mYaw, mRoll);

	DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation(S, zero, R, T);

	DirectX::XMStoreFloat4x4(&mObjConstantData.gWorld, DirectX::XMMatrixTranspose(M));
	XMStoreFloat3(&mRightVector, M.r[0]);
	XMStoreFloat3(&mUpVector, M.r[1]);
	XMStoreFloat3(&mFowardVector, M.r[2]);
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

	XMVECTOR lTexVec = XMLoadFloat2(&inLeft.TexCoords);
	XMVECTOR rTexVec = XMLoadFloat2(&inRight.TexCoords);

	XMFLOAT2 texCoords;

	DirectX::XMStoreFloat2(&texCoords, (lTexVec + rTexVec) * 0.5f);

	return { Pos , color, texCoords };
}
