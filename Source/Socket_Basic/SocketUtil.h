#pragma once

enum SocketAddressFamily : uint8_t
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:
	static bool StaticInit();
	static void CleanUp();

	static void ReportError(const TCHAR* inOperationDesc);
	static void ReportError(const char* inOperationDesc);
	static int GetLastError();

	static int Select(	const vector<TCPSocketPtr>* inReadSet,
						vector<TCPSocketPtr>* outReadSet,
						const vector<TCPSocketPtr>* inWriteSet = nullptr,
						vector<TCPSocketPtr>* outWriteSet = nullptr,
						const vector<TCPSocketPtr>* inExceptSet = nullptr,
						vector<TCPSocketPtr>* outExceptSet = nullptr);

	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily, bool isBlocking = true);

private:
	static void SetNonBlockingMode(SOCKET& socket);

	inline static fd_set* FillSetFromVector(fd_set& outSet, const vector<TCPSocketPtr>* inSockets);
	inline static void FillVectorFromSet(vector<TCPSocketPtr>* outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inSet);
};