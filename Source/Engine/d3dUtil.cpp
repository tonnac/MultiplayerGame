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

	
	return TEXT("Failed: ") + FuncName + TEXT("\nFile: ") + FileName + TEXT("\nLine: ") + std::to_tstring(LineNumber) + TEXT("\nError: ") + msg;
}

tstring D3DUtil::PrintfImpl(const TCHAR* msg, ...)
{
	int BufferSize = STARTING_BUFFER_SIZE;
	TCHAR StartingBuffer[STARTING_BUFFER_SIZE] = {};
	TCHAR* Buffer = StartingBuffer;
	int Result = -1;

	GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, msg, msg, Result);

	if (Result == -1)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = static_cast<TCHAR*>(std::realloc(Buffer, BufferSize));
			GET_VARARGS_RESULT(Buffer, BufferSize, BufferSize - 1, msg, msg, Result);
		}
	}

	Buffer[Result] = 0;

	tstring ResultString(Buffer);

	if (BufferSize != STARTING_BUFFER_SIZE)
	{
		std::free(Buffer);
	}
	return ResultString;
}