#include "EnginePCH.h"
#include <comdef.h>

DxException::DxException(HRESULT hr, const tstring& funcName, const tstring& fileName, int lineNumber)
	: ErrorCode(hr), FuncName(funcName), FileName(fileName), LineNumber(lineNumber)
{
}

tstring DxException::ToString() const
{
	_com_error err(ErrorCode);
	tstring msg = err.ErrorMessage();

	return L"Failed: " + FuncName + L"\nFile: " + FileName + L"\nLine: " + std::to_tstring(LineNumber) + L"\nError: " + msg;
}