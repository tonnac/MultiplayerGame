#pragma once
class SocketException
{
public:
	SocketException() = delete;
	SocketException(const tString& functionName, const tString& fileName, int lineNumber);

	tString ToString() const;

	tString FunctionName;
	tString FileName;
	int LineNumber = -1;
};