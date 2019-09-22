#include "RobocatPCH.h"

tString StringUtils::GetCommandLineArg(int inIndex)
{
	if (inIndex < __argc)
	{
		return tString(__wargv[inIndex]);
	}
	return tString();
}

tString StringUtils::Sprintf(const TCHAR* inFormat, ...)
{
	static TCHAR temp[4096];

	va_list args;
	va_start(args, inFormat);

#ifdef _UNICODE
	_vsnwprintf_s(temp, _countof(temp), _countof(temp), inFormat, args);
#else
	_vsnprintf_s();
#endif

	return tString(temp);
}

//void StringUtils::Log(const TCHAR* inFormat)
//{
//}

tString StringUtils::ConvertToUnicode(const char* inString)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(inString);
}

tString StringUtils::ClassFunctionString(const char* className, const char* funcName)
{
	tString class_Name = ConvertToUnicode(className);
	tString func_Name = ConvertToUnicode(funcName);
	return class_Name + L"::" + func_Name;
}

tString StringUtils::AnsiToWString(const std::string& str)
{
	WCHAR buffer[BUFFER_SIZE] = {};
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, BUFFER_SIZE);
	return tString(buffer);
}

void StringUtils::Log(const TCHAR* inFormat, ...)
{
	// Not thread safe..
	static TCHAR temp[4096];

	va_list args;
	va_start(args, inFormat);

#ifdef _UNICODE
	_vsnwprintf_s(temp, _countof(temp), _countof(temp), inFormat, args);
#else
	_vsnprintf_s();
#endif

	OutputDebugString(temp);
	OutputDebugString(L"\n");
}