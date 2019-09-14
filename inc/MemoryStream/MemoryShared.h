#pragma once
#include "SocketPCH.h"

#include <DirectXMath.h>

#pragma comment(lib,"MemoryStream.lib")

constexpr uint32_t kMaxPacketSize = 1470;

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return static_cast<int>((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return static_cast<float>(inNumber) * inPrecision + inMin;
}

template <int x>
struct mlog2 { enum { value = 1 + mlog2<x / 2>::value }; };

template<>
struct mlog2<1> { enum { value = 1 }; };

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "TestClass.h"
#include "ByteSwap.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"