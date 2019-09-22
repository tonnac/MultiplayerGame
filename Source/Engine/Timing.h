#pragma once
#include <chrono>

class Timing
{
public:
	Timing();

	void Update();

	float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	float GetTimef() const
	{
		return static_cast<float>(GetTime());
	}

	float GetFrameStartTime() const { return mFrameStartTimef; }

private:
	static constexpr float mDesiredFrameTime = 1.f / 60.f;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;

	float mDeltaTime = 0.f;
	float mFrameStartTimef = 0.f;

	double mLastFrameStartTime = 0.;
};