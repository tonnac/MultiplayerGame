#pragma once

namespace StringUtils
{
	tString GetCommandLineArg(int inIndex);

	tString Sprintf(const TCHAR* inFormat, ...);

	//void Log(const TCHAR* inFormat);
	void Log(const TCHAR* inFormat, ...);

	tString ConvertToUnicode(const char* inString);
	tString ClassFunctionString(const char* className, const char* funcName);

	tString AnsiToWString(const std::string& str);
}

#define LOG(...) StringUtils::Log(__VA_ARGS__);