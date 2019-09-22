#include "RobocatPCH.h"

SocketException::SocketException(const tString& functionName, const tString& fileName, int lineNumber)
	: FunctionName(functionName), FileName(fileName), LineNumber(lineNumber)
{
}

tString SocketException::ToString() const
{
	return FunctionName + L"() failed in " + FileName + L"; line " + std::to_wstring(LineNumber);
}
