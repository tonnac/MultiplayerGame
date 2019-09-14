#pragma once
#include "SocketPCH.h"

enum class ESocketType : uint8_t
{
	UDP,
	TCP
};

class Server
{
public:
	using ServerPtr = shared_ptr<Server>;

	~Server();
	static ServerPtr getInstance()
	{
		if (mServer == nullptr)
		{
			mServer = ServerPtr(new Server());
		}
		return mServer;
	}

	void Initialize(ESocketType socketType, const tString& IPAddr, uint16_t portNum);
	void Run();

	void UserExit(vector<TCPSocketPtr>* readSocket, const TCPSocketPtr& DelSocket);

private:
	TCPSocketPtr mSocket;
	SocketAddressPtr mSockAddress;
	WSADATA mWsadata;

	static ServerPtr mServer;

	Server();
};


#define SERVER Server::getInstance()