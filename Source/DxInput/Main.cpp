#include "EnginePCH.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd)
{
	try
	{
		Engine::StaticInit();
		ENGINE->SetHInstance(hInstance);

		ENGINE->Initialize();
		return ENGINE->Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), TEXT("HR Failed"), MB_OK);
		return 0;
	}
}