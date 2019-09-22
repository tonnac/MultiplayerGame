#pragma once

class D3DUtil
{
public:
	static inline tstring AnsiToTString(const string& str)
	{
		TCHAR buffer[512]{};
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return tstring(buffer);
	}
};

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const tstring& funcName, const tstring& fileName, int lineNumber);

	tstring ToString() const;

	HRESULT ErrorCode = S_OK;
	tstring FuncName;
	tstring FileName;
	int LineNumber = -1;
};

#ifndef ThrowDxFail
#define ThrowDxFail(x)									\
{														\
	HRESULT hr__ = (x);									\
	if(FAILED(hr__))									\
	{													\
		tstring wfn = __FILEW__;						\
		tstring wfc = __FUNCTIONW__;					\
		throw DxException(hr__, wfc, wfn, __LINE__);	\
	}													\
}
#endif