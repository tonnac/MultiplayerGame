#include "Server.h"

Server::ServerPtr Server::mServer = nullptr;

void Server::Run()
{
	vector<TCPSocketPtr> readBlockSockets;

	readBlockSockets.push_back(mSocket);

	vector<TCPSocketPtr> readableSockets;

	while (1)
	{
		if (!SocketUtil::Select(&readBlockSockets, &readableSockets))
		{
			continue;
		}

		for (const TCPSocketPtr& socket : readableSockets)
		{
			if (socket == mSocket)
			{
				SocketAddress newClientAddress;
				TCPSocketPtr newSocket = mSocket->Accept(newClientAddress);
				readBlockSockets.push_back(newSocket);
				//TODO 立加 贸府;
			}
			else
			{
				char segment[BUFFER_SIZE];
				int dataReceived = socket->Receive(segment, BUFFER_SIZE);
				if (dataReceived > 0)
				{
					socket->Send(segment, dataReceived);
				}
				else
				{
					printf("user exit\n");
					UserExit(&readBlockSockets, socket);
				}
			}
		}
	}
}

void Server::UserExit(vector<TCPSocketPtr>* readSocket, const TCPSocketPtr& DelSocket)
{
	for (vector<TCPSocketPtr>::const_iterator iter = (*readSocket).cbegin();
		iter != (*readSocket).cend();)
	{
		if ((*iter) == DelSocket)
		{
			iter = readSocket->erase(iter);
		}
		else
			++iter;
	}
}

void Server::Initialize(ESocketType socketType, const tString& IPAddr, uint16_t portNum)
{
	mSockAddress = std::make_shared<SocketAddress>(IPAddr, portNum);
	mSocket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);

	ThrowFailed(mSocket->Bind(*mSockAddress) == 0);
	ThrowFailed(mSocket->Listen(SOMAXCONN) == 0);
}

Server::Server()
{
	if (WSAStartup(MAKEWORD(2, 2), &mWsadata) != 0)
	{
		SocketUtil::ReportError(__FUNCTIONW__);
		return;
	}
}


Server::~Server()
{
	WSACleanup();
}