#include "WindowClass.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd)
{
	WindowClass wd(hInstance);
	wd.Initialize();
	wd.Run();

	return 0;
}