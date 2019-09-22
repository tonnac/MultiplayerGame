#include "RobocatPCH.h"
#include <SDL.h>
#include <time.h>

Engine::Engine() :
	mShouldKeepRunning(true)
{
	SocketUtil::StaticInit();


	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

Engine::~Engine()
{
	SocketUtil::CleanUp();

	SDL_Quit();
}

int Engine::Run()
{
	return 0;
}

void Engine::HandleEvent(SDL_Event* inEvent)
{
}

void Engine::DoFrame()
{
}

int Engine::DoRunLoop()
{
	return 0;
}
