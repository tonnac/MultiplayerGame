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


	TestClass e(32, 13, "Meoeeew");
	SendTestClass(mTCPSocket, &e);

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
