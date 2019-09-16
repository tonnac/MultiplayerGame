#include "Client.h"

ClientPtr Client::mClient = nullptr;

ClientPtr Client::getInstance()
{
	if (mClient == nullptr)
	{
		mClient = ClientPtr(new Client());
	}
	return mClient;
}

void Client::Initialize(const tString& IPAddr, uint16_t portNum, SocketAddressFamily socketType)
{
	mAddress = SocketAddressPtr(new SocketAddress(IPAddr, portNum));
	mTCPSocket = SocketUtil::CreateTCPSocket(socketType);

	ThrowFailed(mTCPSocket->Connect(*mAddress) == 0);
}

void Client::Run()
{
	char Message[BUFFER_SIZE] = {};


	OutputMemoryBitStream bitStream;

	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 quat;
	DirectX::XMFLOAT3 trans;

	scale.x = scale.y = scale.z = 1.f;

	quat.x = FloatRand(1);
	quat.y = FloatRand(3);
	quat.z = FloatRand(2);
	quat.w = FloatRand(3);

	DirectX::XMStoreFloat4(&quat, DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&quat)));

	trans.x = FloatRand(2030);
	trans.y = FloatRand(7030);
	trans.z = FloatRand(4030);

	bitStream.Write(scale, quat, trans);

	send(mTCPSocket->getSocket(), bitStream.GetBufferPtr(), bitStream.GetByteLength(), 0);

	while (1)
	{

	}
}

Client::~Client()
{
	WSACleanup();
}

Client::Client()
{
	if (WSAStartup(MAKEWORD(2, 2), &mWsadata) != 0)
	{
		REPORT;
	}
}
