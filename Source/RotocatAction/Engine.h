#pragma once
#include "SDL_events.h"

class Engine
{
public:

	virtual ~Engine();
	static unique_ptr<Engine> sIntance;

	virtual int Run();
	void SetShouldKeepRunning(bool inShouldKeepRunning) { mShouldKeepRunning = inShouldKeepRunning; }
	virtual void HandleEvent(SDL_Event* inEvent);

protected:

	Engine();

	virtual void DoFrame();

private:

	int DoRunLoop();

	bool mShouldKeepRunning;
};