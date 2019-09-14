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
	while (1)
	{
		fgets(Message, BUFFER_SIZE, stdin);
		if (_stricmp(Message, "q\n") == 0) break;
		mTCPSocket->Send(Message, strlen(Message));
		int receiveSize = mTCPSocket->Receive(Message, BUFFER_SIZE);
		if (receiveSize > 0)
		{
			printf("Message from Server: %s", Message);
		}
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
