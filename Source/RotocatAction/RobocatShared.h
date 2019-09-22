#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "SDL2.lib")

#include "memory"

#include "vector"
#include "unordered_map"
#include "string"
#include "list"
#include "queue"
#include "deque"
#include "unordered_set"
#include "cassert"
#include "functional"
#include "chrono"

#include <locale>
#include <codecvt>
#include <DirectXMath.h>

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0
using tString = std::basic_string<TCHAR>;

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;

using std::wstring_convert;
using std::codecvt_utf8_utf16;

constexpr uint32_t BUFFER_SIZE = 512;

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

#define CLASS_FUNC_STRING StringUtils::ClassFunctionString(typeid(this).name(), __FUNCTION__)
#define REPORT SocketUtil::ReportError(CLASS_FUNC_STRING.c_str())

#define ThrowFailed(x)												\
{																	\
	tString file = __FILEW__;										\
	tString func = __FUNCTIONW__;									\
	if(x == 0) {throw SocketException(func, file, __LINE__);}	\
}

#include "SocketAddress.h"
#include "StringUtils.h"
#include "SocketAddressFactory.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "SocketUtil.h"
#include "SocketException.h"
#include "MemberVariable.h"
#include "ByteSwap.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "LinkingContext.h"
#include "GameObj.h"
#include "ReplicationHeader.h"
#include "ReplicationManager.h"
#include "ObjectCreationRegistry.h"
#include "RPCManager.h"

#include "Engine.h"
