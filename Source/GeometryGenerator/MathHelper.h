#pragma once

class MathHelper
{
public:
	inline static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	inline static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	inline static int Rand(int a, int b)
	{
		return a + rand() % ((b - a) + 1);
	}

	template <typename T>
	inline static T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	inline static T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	inline static T Clamp(T value, T min, T max)
	{
		if (value > max)
		{
			return max;
		}
		else if (value < min)
		{
			return min;
		}
		return value;
	}

	template <typename T>
	inline static T Lerp(T min, T max, float t)
	{
		return min + (max - min) * t;
	}

	inline static bool CompareFloat(float a, float b)
	{
		if (abs(a - b) <= Epsilon)
		{
			return true;
		}
		return false;
	}

private:
	static constexpr float Epsilon = 0.01f;
};