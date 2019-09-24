#pragma once
#pragma warning(disable : 4005)
#pragma warning(disable : 26495)

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <Windows.h>
#include <wrl.h>
#pragma region DirectX 11

#include <D3D11.h>
#include <DXGI.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
//#pragma comment(lib, "d3dx11")
#pragma comment(lib, "d3dcompiler")

#pragma region DirectWrite
#include <d2d1_3.h>
#include <dwrite_3.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma endregion

#pragma region DirectInput
#include <dinput.h>
#pragma comment(lib, "dxguid")
#pragma comment(lib, "dinput8")
#pragma endregion

#pragma endregion

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_set>
#include <cassert>
#include <functional>
#include <locale>
#include <codecvt>


#define SINGLE(x)						\
public:									\
x(const x& rhs) = delete;				\
x operator = (const x& rhs) = delete;

namespace std
{
	using tstring = std::basic_string<TCHAR>;
	template <typename T>
	inline tstring to_tstring(T value)
#ifdef _UNICODE
	{
		return to_wstring(value);
	}
#else
	{
		return to_string(value);
	}
#endif
}

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;
using std::tstring;
using std::stack;

using std::wstring_convert;
using std::codecvt_utf8_utf16;

#ifndef SINGLETON_DEF

#define SINGLETON(x)						\
public:										\
	static std::unique_ptr<x> sInstance;	\
	static void StaticInit()				\
	{										\
		sInstance.reset(new x());			\
	}										\
SINGLE(x)									\
protected:									\
x() {}
#endif

#include "d3dUtil.h"
#include "MathHelper.h"
#include "WindowClass.h"
#include "Device.h"
#include "Engine.h"
#include "Direct2D.h"