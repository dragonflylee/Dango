#ifndef _STDAFX_H_
#define _STDAFX_H_

// ATL 头文件:
#include <atlbase.h>
#include <atlstr.h>

extern CComModule _Module;

#include <atlwin.h>
#include <atlcoll.h>

// Windows 头文件:
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>
#include <wincodec.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#define HR_CHECK(_hr_) hr = _hr_; if (FAILED(hr)) { ATLTRACE(TEXT("0x%.8x\n"), hr); goto exit; }
#define BOOL_CHECK(_hr_) if (!(_hr_)) { hr = HRESULT_FROM_WIN32(::GetLastError()); ATLTRACE(TEXT("0x%.8x\n"), hr); goto exit; }

#include "Resource.h"
#include "Config.h"

#endif // _STDAFX_H_