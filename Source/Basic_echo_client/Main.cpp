#include "Client.h"


int main()
{
	try
	{
		CHAT_CLIENT->Initialize(L"219.254.48.7", 5959, SocketAddressFamily::INET);
		CHAT_CLIENT->Run();
		return 0;
	}
	catch (SocketException& e)
	{
		OutputDebugString(e.ToString().c_str());
		return -1;
	}
}