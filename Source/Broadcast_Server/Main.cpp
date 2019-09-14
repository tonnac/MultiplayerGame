#include "Server.h"

int main()
{
	try {
		SERVER->Initialize(ESocketType::TCP, tString(), 5959);
		SERVER->Run();
		return 0;
	}
	catch(SocketException & e)
	{
		OutputDebugString(e.ToString().c_str());
		return -1;
	}
}