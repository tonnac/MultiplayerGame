#include "SocketPCH.h"

using namespace StringUtils;

bool SocketUtil::StaticInit()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		ReportError(L"Statring Up");
		return false;
	}

	return false;
}

void SocketUtil::CleanUp()
{
	WSACleanup();
}

void SocketUtil::ReportError(const TCHAR* inOperationDesc)
{
	LPWSTR lpMsgBuf = nullptr;
	DWORD errorNum = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, 
		errorNum, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		lpMsgBuf, 0, nullptr);

	LOG(L"Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	tString wide = ConvertToUnicode(inOperationDesc);

	LPWSTR lpMsgBuf = nullptr;
	DWORD errorNum = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		lpMsgBuf, 0, nullptr);

	LOG(L"Error %s: %d- %s", wide.c_str(), errorNum, lpMsgBuf);
	LocalFree(lpMsgBuf); 
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

int SocketUtil::Select(const vector<TCPSocketPtr>* inReadSet, vector<TCPSocketPtr>* outReadSet, const vector<TCPSocketPtr>* inWriteSet, vector<TCPSocketPtr>* outWriteSet, const vector<TCPSocketPtr>* inExceptSet, vector<TCPSocketPtr>* outExceptSet)
{
	fd_set read, write, except;

	fd_set* readPtr = FillSetFromVector(read, inReadSet);
	fd_set* writePtr = FillSetFromVector(write, inWriteSet);
	fd_set* exceptPtr = FillSetFromVector(except, inExceptSet);

	int toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);

	if (toRet > 0)
	{
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, read);
		FillVectorFromSet(outExceptSet, inExceptSet, read);
	}
	return toRet;
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);

	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}
	else
	{
#ifdef _UNICODE
		ReportError(ClassFunctionString("SocketUtil", __FUNCTION__).c_str());
#else
		ReportError(__FUNCTION__);
#endif

		return nullptr;
	}
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily, bool isBlocking)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);

	if (!isBlocking)
	{
		SetNonBlockingMode(s);
	}

	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}
	else
	{
		ReportError(__FUNCTIONW__);
		return nullptr;
	}
}

void SocketUtil::SetNonBlockingMode(SOCKET& socket)
{
	u_long arg = 1;
	if (ioctlsocket(socket, FIONBIO, &arg) == SOCKET_ERROR)
	{
		ReportError(__FUNCTIONW__);
	}
}

inline fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const vector<TCPSocketPtr>* inSockets)
{
	if (nullptr != inSockets)
	{
		FD_ZERO(&outSet);

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
		}
		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

inline void SocketUtil::FillVectorFromSet(vector<TCPSocketPtr>* outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inSet)
{
	if (nullptr != inSockets && nullptr != outSockets)
	{
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets)
		{
			if (FD_ISSET(socket->mSocket, &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}