#pragma once
#include "SocketPCH.h"

#include <DirectXMath.h>

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

#pragma comment(lib,"MemoryStream.lib")

constexpr uint32_t kMaxPacketSize = 1470;
constexpr float FLOAT_EPSILON = 1.e-3F;

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return static_cast<int>((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return static_cast<float>(inNumber) * inPrecision + inMin;
}

template<typename T>
T Clamp(T value, T min, T max)
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

inline bool CompareFloat(float lhs, float rhs)
{
	return fabsf(lhs - rhs) <= FLOAT_EPSILON ? true : false;
}

constexpr int32_t mLog(int32_t x)
{
	int32_t Ret = 0;
	while (x >= 1)
	{
		++Ret;
		x >>= 1;
	}
	return Ret;
}

#include "MemberVariable.h"
#include "ByteSwap.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "TestClass.h"
