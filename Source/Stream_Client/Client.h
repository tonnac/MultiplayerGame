#pragma once
#include "MemoryPCH.h"
#include <io.h>
#include <fcntl.h>

class Client;
using ClientPtr = shared_ptr<Client>;

class Client
{
public:
	static ClientPtr getInstance();

	void Initialize(const tString& IPAddr, uint16_t portNum, SocketAddressFamily socketType);
	void Run();

	~Client();

	string RandomString()
	{
		char ch[5] = {};
		for (int i = 0; i < 4; ++i)
		{
			ch[i] = GetRandRange(65, 90);
		}
		return string(ch);
	}

	int GetRandRange(int min, int max)
	{
		return static_cast<int>(min + ((max - min) * rand() / (float)RAND_MAX));
	}

	float FloatRand()
	{
		return (float)rand() / RAND_MAX;
	}

	float FloatRand(int min)
	{
		return min + (float)rand() / RAND_MAX;
	}


private:
	static ClientPtr mClient;
	Client();

	WSADATA mWsadata;
	SocketAddressPtr mAddress;
	TCPSocketPtr mTCPSocket;
	UDPSocketPtr mUDPSocket;
};

#define CLIENT Client::getInstance()