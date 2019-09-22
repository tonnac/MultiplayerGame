#include "RobocatPCH.h"

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const tString& inString)
{
	size_t pos = inString.find_last_of(':');
	tString host, service;
	if (pos != tString::npos)
	{
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	}
	else
	{
		host = inString;
		service = L"0";
	}
	ADDRINFOW hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;

	ADDRINFOW* result = nullptr;
	int error = GetAddrInfo(host.c_str(), service.c_str(), &hint, &result);

	ADDRINFOW* initResult = result;

	if (error != 0 && result == nullptr)
	{
		FreeAddrInfoW(initResult);
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if (!result->ai_addr)
	{
		FreeAddrInfoW(initResult);
		return nullptr;
	}

	shared_ptr<SocketAddress> toRet = std::make_shared<SocketAddress>(*result->ai_addr);

	FreeAddrInfoW(initResult);
	return toRet;
}