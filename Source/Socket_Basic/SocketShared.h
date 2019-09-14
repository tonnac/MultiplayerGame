#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Socket_Basic.lib")

#include "memory"

#include "vector"
#include "unordered_map"
#include "string"
#include "list"
#include "queue"
#include "deque"
#include "unordered_set"
#include "cassert"

#include <locale>
#include <codecvt>
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

#include "SocketAddress.h"
#include "StringUtils.h"
#include "SocketAddressFactory.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "SocketUtil.h"
#include "SocketException.h"


#define CLASS_FUNC_STRING StringUtils::ClassFunctionString(typeid(this).name(), __FUNCTION__)
#define REPORT SocketUtil::ReportError(CLASS_FUNC_STRING.c_str())

#define ThrowFailed(x)												\
{																	\
	tString file = __FILEW__;										\
	tString func = __FUNCTIONW__;									\
	if(x == 0) {throw SocketException(func, file, __LINE__);}	\
}
