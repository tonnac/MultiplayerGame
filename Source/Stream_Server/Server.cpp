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
				//TODO ���� ó��;
			}
			else
			{
				char buffer[4096] = {};

				int receiveData = recv(socket->getSocket(), buffer, _countof(buffer), 0);

				if (receiveData > 0)
				{
					DirectX::XMFLOAT3 scale;
					DirectX::XMFLOAT4 quat;
					DirectX::XMFLOAT3 trans;
					InputMemoryBitStream iMS(buffer, receiveData);

					iMS.Read(scale, quat, trans);

					int b = 123;
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