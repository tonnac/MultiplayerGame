#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <Windows.h>


int GetVarArgs(TCHAR* Dest, int DestSize, const TCHAR*& Fmt, va_list ArgPtr)
{
	int Result = vswprintf(Dest, DestSize, Fmt, ArgPtr);
	va_end(ArgPtr);
	return Result;
}


#define GET_VARARGS_RESULT(msg, msgsize, len, lastarg, fmt, result) \
	{																\
		va_list ap;													\
		va_start(ap, lastarg);										\
		result = GetVarArgs(msg, msgsize, fmt, ap);					\
		if (result >= msgsize)										\
		{															\
			result = -1;											\
		}															\
		va_end(ap);													\
	}

constexpr int SSS = 512;

namespace std
{
	using tstring = std::basic_string<TCHAR>;

	tstring PrintfImpl(const TCHAR* msg, ...)
	{
		int		BufferSize = 10;
		TCHAR	StartingBuffer[10];
		TCHAR* Buffer = StartingBuffer;
		int		Result = -1;

		// First try to print to a stack allocated location 
		GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, msg, msg, Result);

		// If that fails, start allocating regular memory
		if (Result == -1)
		{
			Buffer = nullptr;
			while (Result == -1)
			{
				BufferSize *= 2;
				Buffer = static_cast<TCHAR*>(std::realloc(Buffer, BufferSize * sizeof(TCHAR)));
				GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, msg, msg, Result);
			};
		}

		Buffer[Result] = 0;

		tstring resultString(Buffer);

		if (BufferSize != 10)
		{
			std::free(Buffer);
		}

		return resultString;
	}

	template<typename ... ARGS>
	static tstring Printf(const TCHAR* msg, ARGS ... arg)
	{
		return PrintfImpl(msg, arg...);
	}
}

using namespace std;


#define debug(msg, ...) printf("%s\n%d\n" msg "\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define MAKESTRING(msg, ...)		\
{									\
TCHAR test[256];					\
wsprintf(test, msg, __VA_ARGS__);	\
wprintf(TEXT("%s"), test);			\
}									

int main()
{
	//debug("failed %s : %d", __FUNCTION__, __LINE__);

	tstring qwer = std::Printf(TEXT("qweqweaewrewraewr %d %f %s"), 32, 3.2f, TEXT("zzzzz"));

	return 0;
}