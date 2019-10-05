#include "Timing.h"

using namespace std::chrono;

Timing::Timing()
{
	mStartTime = high_resolution_clock::now();
}

void Timing::Update()
{
	double currentTime = GetTime();

	mDeltaTime = (float)(currentTime - mLastFrameStartTime);

	while (mDeltaTime < mDesiredFrameTime)
	{
		currentTime = GetTime();

		mDeltaTime = (float)(currentTime - mLastFrameStartTime);
	}

	mLastFrameStartTime = currentTime;
	mFrameStartTimef = static_cast<float>(mLastFrameStartTime);
}

double Timing::GetTime() const
{
	return duration<double>(high_resolution_clock::now() - mStartTime).count();
}